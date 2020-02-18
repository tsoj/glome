#pragma once

#include "gl.hpp"
#include "types.hpp"

class Renderer
{
public:
    Renderer(
        int width,
        int height,
        int max_num_lights
    );

    struct Camera
    {
        HypersphereOrientation hypersphere_orientation;
        Orientation3D orientation;
        Metre<float> far_plane;
        Radian<float> field_of_view;
    };

    void setCamera(const Camera& camera)
    {
        m_camera = camera;
    }

    void setHypersphereRadius(const Metre<float>& hypershpere_radius)
    {
        m_hypershpere_radius = hypershpere_radius;
    }

    void setFogColor(const glm::vec4 fog_color)
    {
        m_fog_color = fog_color;
    }

    struct MeshData
    {
        gl::Texture2D texture;
        gl::Texture2D normal_map;
        gl::VertexArray vao;
        HypersphereOrientation hypersphere_orientation;
        Orientation3D model_orientation;
    };

    void submitMesh(const MeshData& mesh)
    {
        m_mesh_vector.push_back(mesh);
    }

    void submitLight(const glm::vec4& coord, const Light& light)
    {
        m_light_coords.push_back(coord);
        m_light_colors.push_back(light.color);
        //TODO: class 3: make sure that m_light_coords.size() is smaller or equal to m_max_num_lights
    }

    void render();

    gl::Texture2D image()
    {
        return m_target_texture;
    }

private:
    glm::vec4 m_fog_color = glm::vec4{0.0, 0.0, 0.0, 0.0};

    std::vector<MeshData> m_mesh_vector;
    const int m_max_num_lights;
    std::vector<glm::vec4> m_light_coords;
    std::vector<glm::vec3> m_light_colors;

    Metre<float> m_hypershpere_radius;

    Camera m_camera;

    const int m_width;
    const int m_height;
    const float m_aspect_ratio;

    gl::Renderbuffer m_depth_buffer;
    gl::Texture2D m_target_texture;
    gl::Framebuffer m_target_buffer;

    gl::Program m_program;
};