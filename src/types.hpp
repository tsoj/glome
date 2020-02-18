
#pragma once

#include <glm/glm.hpp>
#include <string>
#include "physics_units.hpp"

//TODO: class 3: use own namespace

using namespace physics_units;

#define STRONG_TYPEDEF(base, new_type)\
struct new_type : std::remove_const<typename std::remove_reference<base>::type>::type\
{\
  template<typename... Targs>\
  constexpr new_type(Targs&&... args) :\
  std::remove_const<typename std::remove_reference<base>::type>::type{std::forward<Targs>(args)...}{}\
};


STRONG_TYPEDEF(glm::vec3, Direction3D)

STRONG_TYPEDEF(glm::vec4, Direction4D)

STRONG_TYPEDEF(glm::mat3, Orientation3D)

STRONG_TYPEDEF(std::string, Name)

STRONG_TYPEDEF(decltype(glm::vec3{1.0f} * physics_units::metre / physics_units::second), Velocity3D)

STRONG_TYPEDEF(physics_units::Metre<glm::vec3>, Position3D)

STRONG_TYPEDEF(decltype(glm::vec3{1.0} * physics_units::radian / physics_units::second), AngularVelocity3D)

#undef STRONG_TYPEDEF

class HypersphereOrientation
{
public:
    template<typename... Targs>
    explicit HypersphereOrientation(Targs&& ... args) : m_orientation({args...})
    {}

    [[nodiscard]] const glm::vec4& coord() const
    {
        return m_orientation.m4[3];
    }

    [[nodiscard]] const glm::mat3x4& orientation() const
    {
        return m_orientation.m3x4;
    }

    [[nodiscard]] glm::vec4& coord()
    {
        return m_orientation.m4[3];
    }

    [[nodiscard]] glm::mat3x4& orientation()
    {
        return m_orientation.m3x4;
    }

    operator glm::mat4() const
    {
        return m_orientation.m4;
    }

private:

    union
    {
        glm::mat4 m4;
        glm::mat3x4 m3x4;
    } m_orientation;
};

class Plane
{
public:
    template<typename... Targs>
    constexpr Plane(Targs&& ... args) :
        vectors{std::forward<Targs>(args)...}
    {}

    operator glm::mat2x4() const;

private:
    constexpr static float bias = 0.0000001;
    glm::mat2x4 vectors;
};

struct Light
{
    Candela<float> intensity;
    glm::vec3 color;
};




