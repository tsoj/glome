#pragma once

#include <bitset>
#include <vector>
#include <memory>
#include <functional>
#include <cassert>
#include <any>
#include <algorithm>

namespace ec_system
{
    class Entity
    {
        friend class EntityManager;

    public:

        Entity() : m_id(SIZE_MAX)
        {}

        bool operator==(const Entity& b) const
        {
            return (this->m_id == b.m_id);
        }

        bool operator!=(const Entity& b) const
        {
            return !((*this) == b);
        }

        [[nodiscard]] size_t getId() const
        {
            return m_id;
        }

    private:

        explicit Entity(size_t id) : m_id(id)
        {}

        size_t m_id;
    };

    class EntityManager
    {
#ifndef MAX_NUM_COMPONENT_TYPES
#define MAX_NUM_COMPONENT_TYPES 64
#endif

        template<typename... Targs>
        class Iterator;

        template<typename C, typename... Targs>
        class Each;

    public:

        EntityManager()
        {
            num_components.fill(0);
        }

        EntityManager(const EntityManager&) = delete;

        void operator=(const EntityManager&) = delete;

        Entity createEntity()
        {
            size_t id;
            if (unused_ids.empty())
            {
                id = has_mask.size();
                has_mask.emplace_back(1);
            }
            else
            {
                id = unused_ids.back();
                unused_ids.pop_back();
                assert(has_mask[id].none());
                has_mask[id] = std::bitset<MAX_NUM_COMPONENT_TYPES>(1);
            }
            assert(!(id >= has_mask.size()) && has_mask[id].test(0));
            return Entity(id);
        }

        [[nodiscard]] bool hasEntity(const Entity entity) const
        {
            if (entity.m_id >= has_mask.size())
            {
                return false;
            }
            return has_mask[entity.m_id].test(0);
        }

        void removeEntity(const Entity entity)
        {
            if (!hasEntity(entity))
            {
                // TODO: replace exceptions with assert variant
                throw std::runtime_error(
                    std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " +
                    "Can't remove entity. Entity doesn't exists.");
            }
            has_mask[entity.m_id] = std::bitset<MAX_NUM_COMPONENT_TYPES>(0);
            unused_ids.emplace_back(entity.m_id);
        }

        template<typename T, typename... Args>
        void createComponent(const Entity entity, Args&& ... args)
        {
            if (!hasEntity(entity))
            {
                throw std::runtime_error(
                    std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " +
                    "Can't create component. Entity doesn't exists.");
            }
            if (has<T>(entity))
            {
                throw std::runtime_error(
                    std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " +
                    "Can't create component. Component already exists.");
            }

            if (component_vectors.get<T>().size() <= entity.m_id)
            {
                component_vectors.get<T>().resize(entity.m_id + 1);
            }

            has_mask[entity.m_id] |= bit_id<T>;

            component_vectors.get<T>()[entity.m_id] = T{std::forward<Args>(args)...};

            num_components[type_id < T > ] += 1;

            component_type_list[type_id < T > ].push_back(entity);
        }

        template<typename T>
        void removeComponent(const Entity entity)
        {
            if (!hasEntity(entity))
            {
                throw std::runtime_error(
                    std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " +
                    "Can't remove component. Entity doesn't exists.");
            }
            if (!has<T>(entity))
            {
                throw std::runtime_error(
                    std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " +
                    "Can't remove component. Component doesn't exists.");
            }
            has_mask[entity.m_id] &= ~bit_id<T>;

            num_components[type_id < T > ] -= 1;

            for (size_t i = 0; i < component_type_list[type_id < T > ].size(); ++i)
            {
                if (component_type_list[type_id < T > ][i] == entity)
                {
                    component_type_list[type_id < T > ].erase(component_type_list[type_id < T > ].begin() + i);
                    break;
                }
            }
        }

        template<typename T>
        T& get(const Entity entity)
        {
            if (!hasEntity(entity))
            {
                throw std::runtime_error(
                    std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " +
                    "Can't get component. Entity doesn't exists.");
            }
            if (!has<T>(entity))
            {
                throw std::runtime_error(
                    std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " +
                    "Can't access component. Component doesn't exists.");
            }
            assert(component_vectors.get<T>().size() > entity.m_id);
            assert(component_vectors.get<T>()[entity.m_id].has_value());
            return *(component_vectors.get<T>()[entity.m_id]);
        }

        template<typename T>
        const T& get(const Entity entity) const
        {
            if (!hasEntity(entity))
            {
                throw std::runtime_error(
                    std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " +
                    "Can't get component. Entity doesn't exists.");
            }
            if (!has<T>(entity))
            {
                throw std::runtime_error(
                    std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " +
                    "Can't access component. Component doesn't exists.");
            }
            assert(component_vectors.get<T>().size() > entity.m_id);
            assert(component_vectors.get<T>()[entity.m_id].has_value());
            return *(component_vectors.get<T>()[entity.m_id]);
        }

        template<typename... Targs>
        [[nodiscard]] bool has(const Entity entity) const
        {
            if (entity.m_id >= has_mask.size())
            {
                return false;
            }
            return (has_mask[entity.m_id] & BitTypeId::get<Targs...>()) == BitTypeId::get<Targs...>();
        }

        template<typename... Targs>
        Iterator<Targs...> iterator() const
        {
            return Iterator<Targs...>(*this);
        }

        template<typename... Targs>
        Iterator<Targs...> iterator(const Entity entity) const
        {
            return Iterator<Targs...>(*this, entity);
        }

        template<typename... Targs>
        Each<EntityManager, Targs...> each()
        {
            return Each<EntityManager, Targs...>(*this);
        }

        template<typename... Targs>
        Each<const EntityManager, Targs...> each() const
        {
            return Each<const EntityManager, Targs...>(*this);
        }

    private:

        class TypeId
        {
        public:

            TypeId() = delete;

            template<typename T>
            static size_t get()
            {
                static const size_t id = getUniqueId();
                return id;
            }

        private:

            static size_t getUniqueId()
            {
                const size_t unique_id = counter;
                counter += 1;
                return unique_id;
            }

            static inline size_t counter = 0;
        };

        template<typename T>
        const static inline auto type_id = TypeId::get<T>();

        class BitTypeId
        {
        public:

            BitTypeId() = delete;


            template<typename... T>
            static const auto& get()
            {
                static const auto bit_id = getBitIdUnion<T...>();
                return bit_id;
            }

        private:

            template<typename T>
            static inline auto getBitIdUnion()
            {
                static const auto unique_bit_id = std::bitset<MAX_NUM_COMPONENT_TYPES>(0b10 << type_id<T>);
                if (unique_bit_id.none())
                {
                    throw std::runtime_error(
                        std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " +
                        "Too many component types.\nSolution: Set makro MAX_NUM_COMPONENT_TYPES to a bigger number.\n");
                }
                return unique_bit_id;
            }

            template<typename T1, typename T2, typename... Targs>
            static inline auto getBitIdUnion()
            {
                return getBitIdUnion<T1>() | getBitIdUnion<T2, Targs...>();
            }
        };

        template<typename T>
        const static inline auto bit_id = BitTypeId::get<T>();

        class ComponentVectors
        {
        public:
            template<typename T>
            std::vector<std::optional<T>>& get()
            {
                if (!(component_vectors.size() > type_id < T > ))
                {
                    component_vectors.resize(type_id < T > +1);
                }
                if (!component_vectors[type_id < T > ].has_value())
                {
                    component_vectors[type_id < T > ] = std::vector<std::optional<T>>();
                }
                return std::any_cast<std::vector<std::optional<T>>&>(component_vectors[type_id < T > ]);
            }

            template<typename T>
            const std::vector<std::optional<T>>& get() const
            {
                static const std::vector<std::optional<T>> empty_vector;
                if (
                    !(component_vectors.size() > type_id < T > ) ||
                    !component_vectors[type_id < T > ].has_value()
                    )
                {
                    return empty_vector;
                }
                return std::any_cast<const std::vector<std::optional<T>>&>(component_vectors[type_id < T > ]);
            }

        private:
            std::vector<std::any> component_vectors;
        };

        ComponentVectors component_vectors;

        std::vector<std::bitset<MAX_NUM_COMPONENT_TYPES>> has_mask;

        std::array<size_t, MAX_NUM_COMPONENT_TYPES> num_components{};

        std::array<std::vector<Entity>, MAX_NUM_COMPONENT_TYPES> component_type_list;

        std::vector<size_t> unused_ids;

        template<typename... Targs>
        [[nodiscard]] size_t getRarestComponentId() const
        {
            return std::min({type_id < Targs > ...}, [&](auto a, auto b)
            {
                return num_components[a] < num_components[b];
            });
        }

        template<typename... Targs>
        class Iterator
        {
        private:

            const EntityManager* m_entity_manager;
            Entity m_current_entity;
            size_t i = 0;
            const size_t rarest_component_id = m_entity_manager->getRarestComponentId<Targs...>();
            const Entity end_entity = Entity(m_entity_manager->has_mask.size());

            Entity nextValidEntity()
            {
                for (; i < m_entity_manager->component_type_list[rarest_component_id].size(); ++i)
                {
                    auto tmp_entity = m_entity_manager->component_type_list[rarest_component_id][i];
                    if (
                        m_current_entity.m_id <= tmp_entity.m_id &&
                        m_entity_manager->has<Targs...>(tmp_entity)
                        )
                    {
                        return tmp_entity;
                    }
                }
                return end_entity;
            }

        public:

            explicit Iterator(const EntityManager& entity_manager) : m_entity_manager(&entity_manager), m_current_entity(0)
            {}

            Iterator(const EntityManager& entity_manager, const Entity& entity) : m_entity_manager(&entity_manager), m_current_entity(entity)
            {}

            Iterator<Targs...> begin()
            {
                return Iterator<Targs...>(*m_entity_manager, nextValidEntity());
            }

            [[nodiscard]] Iterator<Targs...> end() const
            {
                return Iterator<Targs...>(*m_entity_manager, end_entity);
            }

            bool operator==(const Iterator<Targs...>& a) const
            {
                return this->m_current_entity == a.m_current_entity;
            }

            bool operator!=(const Iterator<Targs...>& a) const
            {
                return !(*this == a);
            }

            Entity operator*() const
            {
                return m_current_entity;
            }

            Iterator<Targs...> operator++() // Prefix Increment
            {
                m_current_entity.m_id += 1;
                i += 1;
                m_current_entity = nextValidEntity();
                return *this;
            }
        };

        template<typename C, typename... Targs>
        class Each
        {
        private:
            C& em;
        public:
            explicit Each(C& entity_manager) : em(entity_manager)
            {}

            Each(const Each&) = delete;

            void operator=(const Each&) = delete;

            template<typename F>
            void run(F f)
            {
                for (auto entity : em.template iterator<Targs...>())
                {
                    f(em.template get<Targs>(entity)...);
                }
            }

            template<typename F>
            void run(F f) const
            {
                for (auto entity : em.template iterator<Targs...>())
                {
                    f(em.template get<Targs>(entity)...);
                }
            }
        };
    };

    template<>
    inline bool EntityManager::has<void>(const Entity entity) const
    {
        return hasEntity(entity);
    }

    template<>
    inline Entity EntityManager::Iterator<void>::nextValidEntity()
    {
        Entity next = m_current_entity;
        while (end().m_current_entity.m_id > next.m_id)
        {
            if (m_entity_manager->has<void>(next))
            {
                return next;
            }
            next.m_id += 1;
        }
        return end().m_current_entity;
    }

}
