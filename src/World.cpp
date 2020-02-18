#include "World.hpp"

#include "../meta/logo.hpp"
#include "shared_glm_glsl.h"
#include <glm/gtx/transform.hpp>

//TODO: class 3: move the json-to-ec_system functions to a own cpp file

namespace glm
{
    void to_json(json& j, const vec2& p)
    {
        j = json{p[0], p[1]};
    }

    void from_json(const json& j, vec2& p)
    {
        j.at(0).get_to(p[0]);
        j.at(1).get_to(p[1]);
    }

    void to_json(json& j, const vec3& p)
    {
        j = json{p[0], p[1], p[2]};
    }

    void from_json(const json& j, vec3& p)
    {
        j.at(0).get_to(p[0]);
        j.at(1).get_to(p[1]);
        j.at(2).get_to(p[2]);
    }

    void to_json(json& j, const vec4& p)
    {
        j = json{p[0], p[1], p[2], p[3]};
    }

    void from_json(const json& j, vec4& p)
    {
        j.at(0).get_to(p[0]);
        j.at(1).get_to(p[1]);
        j.at(2).get_to(p[2]);
        j.at(3).get_to(p[3]);
    }
}

void World::addComponentFromJsonOrientation3D(const json& object, const ec_system::Entity& entity)
{
    m_entity_manager.createComponent<Orientation3D>(entity, Orientation3D{glm::rotate(
        glm::mat4(1.0),
        glm::radians(object.at("angle").get<float>()),
        object.at("axis").get<glm::vec3>()
    )});
}

void World::addComponentFromJsonAngularVelocity3D(const json& object, const ec_system::Entity& entity)
{
    m_entity_manager.createComponent<AngularVelocity3D>(entity, AngularVelocity3D{
        glm::radians(object.at("value").get<float>()) * glm::normalize(object.at("axis").get<glm::vec3>()) * radian / second
    });
}

void World::addComponentFromJsonHypersphereOrientation(const json& object, const ec_system::Entity& entity)
{
    const Position3D position = Position3D{object.get<glm::vec3>()};
    m_entity_manager.createComponent<HypersphereOrientation>(entity, HypersphereOrientation{
        glm::hs::getHypersphereOrientation(
            glm::hs::origin_hypersphere_orientation,
            glm::hs::getHypersphereCoordinate(position, m_radius)
        )
    });
}

void World::addComponentFromJsonVelocity3D(const json& object, const ec_system::Entity& entity)
{
    m_entity_manager.createComponent<Velocity3D>(entity, Velocity3D{object.get<glm::vec3>()});
}

void World::addComponentFromJsonMeshVector(const json& object, const ec_system::Entity& entity)
{
    m_entity_manager.createComponent<std::vector<Mesh>>(entity, getMeshesFromObj(
        object.get<std::filesystem::path>()
    ));
}

void World::addComponentFromJsonName(const json& object, const ec_system::Entity& entity)
{
    m_entity_manager.createComponent<Name>(entity, object.get<std::string>());
}

void World::addComponentFromJsonLight(const json& object, const ec_system::Entity& entity)
{
    m_entity_manager.createComponent<Light>(entity, Light{
        object.at("intensity").get<float>(),
        object.at("color").get<glm::vec3>()
    });
}

void World::addComponentFromJsonCamera(const json& object, const ec_system::Entity& entity)
{
    m_entity_manager.createComponent<World::Camera>(entity, World::Camera{
        glm::radians(object.at("field_of_view").get<float>()) * radian
    });
}

void World::init()
{
    m_ascii_framebuffer_json = json::parse(utility::readFile("configs/ascii_framebuffer.json"));
    for (auto& s : m_ascii_framebuffer_json["debug_name_list"])
    {
        m_ascii_framebuffer_debug_name_list.push_back(s.get<std::string>());
    }

    const auto world_json = json::parse(utility::readFile("configs/world.json"));

    m_window = std::make_shared<Window>(
        world_json["window"]["width"].get<int>(),
        world_json["window"]["height"].get<int>(),
        "glome"
    );
    m_renderer = std::make_shared<Renderer>(m_window->width(), m_window->height(), 5);

    m_radius = world_json["hypersphere_radius"].get<float>() * metre;
    m_far_plane = 2.0f * m_radius * glm::hs::pi();

    m_fog_color = glm::vec4{
        world_json["fog"]["color"].get<glm::vec3>(),
        world_json["fog"]["density"].get<float>()
    };

    m_renderer->setHypersphereRadius(m_radius);
    m_renderer->setFogColor(m_fog_color);

    for (const json& object : world_json["objects"])
    {
        auto entity = m_entity_manager.createEntity();
        for (const auto& tmp : object.items())
        {
            if (m_json_component_mapping.count(tmp.key()) != 0)
            {
                m_json_component_mapping.at(tmp.key())(tmp.value(), entity);
            }
        }
    }
}

void World::loop()
{
    std::chrono::microseconds rendering_time;
    while (!m_window->shouldClose())
    {
        static auto last = std::chrono::high_resolution_clock::now();
        auto now = std::chrono::high_resolution_clock::now();
        Second<float> delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count() * milli * second;
        last = now;

        //TODO: class 3: move all these loops to separate functions
        for (const auto e : m_entity_manager.iterator<World::Camera, Orientation3D, Velocity3D, AngularVelocity3D>())
        {
            //TODO: class 2: design proper input system (using event_system?)
            auto& velocity = m_entity_manager.get<Velocity3D>(e);
            velocity = Velocity3D{0.0, 0.0, 0.0};
            constexpr auto max_camera_velocity = 30.0f * metre / second;

            if (m_window->isKeyPressed(GLFW_KEY_LEFT))
            {
                velocity += glm::normalize(m_entity_manager.get<Orientation3D>(e)[0]) * max_camera_velocity;
            }
            if (m_window->isKeyPressed(GLFW_KEY_RIGHT))
            {
                velocity += glm::normalize(m_entity_manager.get<Orientation3D>(e)[0]) * -max_camera_velocity;
            }
            if (m_window->isKeyPressed(GLFW_KEY_PAGE_UP))
            {
                velocity += glm::normalize(m_entity_manager.get<Orientation3D>(e)[1]) * max_camera_velocity;
            }
            if (m_window->isKeyPressed(GLFW_KEY_PAGE_DOWN))
            {
                velocity += glm::normalize(m_entity_manager.get<Orientation3D>(e)[1]) * -max_camera_velocity;
            }
            if (m_window->isKeyPressed(GLFW_KEY_UP))
            {
                velocity += glm::normalize(m_entity_manager.get<Orientation3D>(e)[2]) * max_camera_velocity;
            }
            if (m_window->isKeyPressed(GLFW_KEY_DOWN))
            {
                velocity += glm::normalize(m_entity_manager.get<Orientation3D>(e)[2]) * -max_camera_velocity;
            }

            auto& angular_velocity = m_entity_manager.get<AngularVelocity3D>(e);
            angular_velocity = AngularVelocity3D{0.0, 0.0, 0.0};
            constexpr auto max_camera_angular_velocity = glm::radians(120.0f) * radian / second;

            if (m_window->isKeyPressed(GLFW_KEY_W))
            {
                angular_velocity += glm::normalize(m_entity_manager.get<Orientation3D>(e)[0]) * max_camera_angular_velocity;
            }
            if (m_window->isKeyPressed(GLFW_KEY_S))
            {
                angular_velocity += glm::normalize(m_entity_manager.get<Orientation3D>(e)[0]) * -max_camera_angular_velocity;
            }
            if (m_window->isKeyPressed(GLFW_KEY_A))
            {
                angular_velocity += glm::normalize(m_entity_manager.get<Orientation3D>(e)[1]) * max_camera_angular_velocity;
            }
            if (m_window->isKeyPressed(GLFW_KEY_D))
            {
                angular_velocity += glm::normalize(m_entity_manager.get<Orientation3D>(e)[1]) * -max_camera_angular_velocity;
            }
            if (m_window->isKeyPressed(GLFW_KEY_E))
            {
                angular_velocity += glm::normalize(m_entity_manager.get<Orientation3D>(e)[2]) * max_camera_angular_velocity;
            }
            if (m_window->isKeyPressed(GLFW_KEY_Q))
            {
                angular_velocity += glm::normalize(m_entity_manager.get<Orientation3D>(e)[2]) * -max_camera_angular_velocity;
            }

        }
        for (const auto e : m_entity_manager.iterator<HypersphereOrientation, Velocity3D>())
        {
            auto& hypersphere_orientation = m_entity_manager.get<HypersphereOrientation>(e);
            const auto& velocity = m_entity_manager.get<Velocity3D>(e);
            if (glm::length(velocity.value) > 0.0)
            {
                hypersphere_orientation = HypersphereOrientation{glm::hs::getHypersphereOrientation(
                    hypersphere_orientation, glm::mat3(1.0), (velocity * delta).value, m_radius
                )};
            }
        }
        for (const auto e : m_entity_manager.iterator<Orientation3D, AngularVelocity3D>())
        {
            const Radian<float> delta_value = glm::length(m_entity_manager.get<AngularVelocity3D>(e).value) * radian / second * delta;
            if (delta_value > 0.0f)
            {
                const auto axis = glm::normalize(m_entity_manager.get<AngularVelocity3D>(e).value);

                m_entity_manager.get<Orientation3D>(e) = Orientation3D{glm::rotate(
                    glm::mat4(1.0),
                    delta_value.value,
                    axis
                )} * m_entity_manager.get<Orientation3D>(e);
            }
        }
        for (const auto e : m_entity_manager.iterator<std::vector<Mesh>, Orientation3D, HypersphereOrientation>())
        {
            for (const auto& mesh : m_entity_manager.get<std::vector<Mesh>>(e))
            {
                m_renderer->submitMesh(
                    {
                        mesh.texture, mesh.normal_map, mesh.vao,
                        m_entity_manager.get<HypersphereOrientation>(e),
                        m_entity_manager.get<Orientation3D>(e)
                    });
            }
        }
        for (const auto e : m_entity_manager.iterator<HypersphereOrientation, Light>())
        {
            m_renderer->submitLight(
                m_entity_manager.get<HypersphereOrientation>(e).coord(),
                m_entity_manager.get<Light>(e)
            );
        }
        for (const auto e : m_entity_manager.iterator<World::Camera, Orientation3D, HypersphereOrientation>())
        {
            m_renderer->setCamera({
                                      m_entity_manager.get<HypersphereOrientation>(e),
                                      m_entity_manager.get<Orientation3D>(e),
                                      m_far_plane,
                                      m_entity_manager.get<World::Camera>(e).field_of_view
                                  });
            m_camera_entity = e;
        }

        auto frame_start = std::chrono::high_resolution_clock::now();
        m_renderer->render();
        glFinish();
        auto frame_end = std::chrono::high_resolution_clock::now();
        rendering_time = std::chrono::duration_cast<std::chrono::microseconds>(frame_end - frame_start);
        m_window->drawImage(m_renderer->image());

        gl::CHECK_FOR_GL_ERRORS();

        glfwPollEvents();


#ifdef USE_ASCII_FRAMEBUFFER

        utility::text_buffer.setChar(utility::text_buffer.width() - (logo_width + 12), 2, logo);
        utility::text_buffer.setTextColor(utility::text_buffer.width() - (logo_width + 12), 2,
                                          logo_width, logo_height, {220, 220, 50});
        utility::text_buffer.setStyle(utility::text_buffer.width() - (logo_width + 12), 2,
                                      logo_width, logo_height, ascii_framebuffer::bold);
        utility::text_buffer.setChar(utility::text_buffer.width() - (logo_width + 12) + 26, logo_height + 4,
                                     {"Copyright (c) 2020 Jost Triller"}
        );

        int current_row = 0;
        for (const auto entity : ((const ec_system::EntityManager&) m_entity_manager).iterator<Name, Orientation3D, HypersphereOrientation>())
        {
            const std::string& name = m_entity_manager.get<Name>(entity);
            if (
                std::find(m_ascii_framebuffer_debug_name_list.begin(), m_ascii_framebuffer_debug_name_list.end(), name) !=
                m_ascii_framebuffer_debug_name_list.end()
                )
            {
                const auto id = entity.getId();
                const HypersphereOrientation& entity_hypersphere_orientation = m_entity_manager.get<HypersphereOrientation>(entity);
                const auto& orientation = m_entity_manager.get<Orientation3D>(entity);

                const auto& camera_hypersphere_orientation = m_entity_manager.get<HypersphereOrientation>(m_camera_entity);
                const auto& camera_orientation = m_entity_manager.get<Orientation3D>(m_camera_entity);
                const auto& field_of_view = m_entity_manager.get<World::Camera>(m_camera_entity).field_of_view;
                const float aspect_ratio = (float) m_window->width() / (float) m_window->height();

                const Metre<float> camera_distance = glm::hs::distanceOnHypersphere(
                    entity_hypersphere_orientation.coord(), camera_hypersphere_orientation.coord(), m_radius);
                const Radian<float> camera_angle = glm::hs::angleDistance(m_radius, camera_distance) * radian;

                const glm::vec3 screen_coord = glm::hs::getViewSpaceCoords(
                    glm::hs::getViewAngles(camera_orientation, camera_hypersphere_orientation, entity_hypersphere_orientation.coord(), m_radius),
                    field_of_view, aspect_ratio, m_far_plane
                );

                const std::vector<std::string> box = {
                    "name: " + name,
                    "id: " + std::to_string(id),
                    "coord: " + utility::toString(entity_hypersphere_orientation.coord()),
                    "camera distance: " + std::to_string(camera_distance) + " metre",
                    "camera angle: " + std::to_string(glm::degrees(camera_angle.value)) + " deg",
                    "screen coord: " + utility::toString(screen_coord),
                    "orientation:",
                    "    x: " + utility::toString(orientation[0]),
                    "    y: " + utility::toString(orientation[1]),
                    "    z: " + utility::toString(orientation[2])
                };
                utility::text_buffer.setChar(0, current_row, box);
                current_row += box.size() + 1;
            }
        }

        utility::text_buffer.setChar(0, current_row, {
            "rendering time: " + std::to_string(rendering_time.count() / 1000.0) + " ms"
        });
        current_row += 1;

        const int cout_width = utility::text_buffer.width() - (logo_width + 12 + 2);
        const int cout_height = utility::text_buffer.height() - current_row - 4;
        utility::text_buffer.setCoutHistory(2, utility::text_buffer.height() - (cout_height + 1), cout_width, cout_height);
        utility::text_buffer.setBackgroundColor(
            0, utility::text_buffer.height() - (cout_height + 2), cout_width + 4, cout_height + 2,
            {100, 100, 100});
        utility::text_buffer.setBackgroundColor(2, utility::text_buffer.height() - (cout_height + 1), cout_width, cout_height,
                                                {50, 50, 50});
        utility::text_buffer.setTextColor(2, utility::text_buffer.height() - (cout_height + 1), cout_width, cout_height,
                                          {50, 220, 60});
        utility::text_buffer.setStyle(2, utility::text_buffer.height() - (cout_height + 1), cout_width, cout_height,
                                      ascii_framebuffer::italic);

        utility::text_buffer.print();
        utility::text_buffer.clear({20, 20, 40}, {220, 220, 220}, ' ');
#endif
    }
}