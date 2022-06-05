#pragma once

#include <glm/glm.hpp>
#include "ec_system.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#include "json.hpp"
#include "physics_units.hpp"
#include "types.hpp"
#include "Mesh.hpp"
#include <memory>

using namespace physics_units;
using json = nlohmann::json;

class World
{
public:

    void init();

    void loop();

private:

    ec_system::EntityManager m_entity_manager;

    std::shared_ptr<Window> m_window;// = Window(1000, 800, "glome");
    std::shared_ptr<Renderer> m_renderer;// = Renderer(m_window.width(), m_window.height(), 5);

    Metre<float> m_radius;
    Metre<float> m_far_plane;

    glm::vec4 m_fog_color;

    struct Camera
    {
        Radian<float> field_of_view;
    };

    ec_system::Entity m_camera_entity;

    std::vector<std::string> m_ascii_framebuffer_debug_name_list;
    json m_ascii_framebuffer_json;
    static constexpr int printFramebufferFrameFrequencey = 15;
    int lastFramebufferPrint = 0;

    void addComponentFromJsonOrientation3D(const json& object, const ec_system::Entity& entity);

    void addComponentFromJsonAngularVelocity3D(const json& object, const ec_system::Entity& entity);

    void addComponentFromJsonHypersphereOrientation(const json& object, const ec_system::Entity& entity);

    void addComponentFromJsonVelocity3D(const json& object, const ec_system::Entity& entity);

    void addComponentFromJsonMeshVector(const json& object, const ec_system::Entity& entity);

    void addComponentFromJsonName(const json& object, const ec_system::Entity& entity);

    void addComponentFromJsonLight(const json& object, const ec_system::Entity& entity);

    void addComponentFromJsonCamera(const json& object, const ec_system::Entity& entity);

    const std::map<std::string, std::function<void(const json&, const ec_system::Entity&)>>
        m_json_component_mapping = {
        {"name",             [&](const auto& j, const auto& e)
                             { addComponentFromJsonName(j, e); }},
        {"mesh_file",        [&](const auto& j, const auto& e)
                             { addComponentFromJsonMeshVector(j, e); }},
        {"velocity",         [&](const auto& j, const auto& e)
                             { addComponentFromJsonVelocity3D(j, e); }},
        {"position",         [&](const auto& j, const auto& e)
                             { addComponentFromJsonHypersphereOrientation(j, e); }},
        {"angular_velocity", [&](const auto& j, const auto& e)
                             { addComponentFromJsonAngularVelocity3D(j, e); }},
        {"orientation",      [&](const auto& j, const auto& e)
                             { addComponentFromJsonOrientation3D(j, e); }},
        {"light",            [&](const auto& j, const auto& e)
                             { addComponentFromJsonLight(j, e); }},
        {"camera",           [&](const auto& j, const auto& e)
                             { addComponentFromJsonCamera(j, e); }}
    };
};