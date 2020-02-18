#include "Renderer.hpp"

Renderer::Renderer(
    const int width,
    const int height,
    const int max_num_lights
) :
    m_max_num_lights(max_num_lights), m_camera(),
    m_width(width), m_height(height), m_aspect_ratio((float) width / (float) height)
{
    m_program = gl::Program(
        {
            {"./shader/hyper.vert", GL_VERTEX_SHADER},
            {"./shader/hyper.frag", GL_FRAGMENT_SHADER}
        },
        {{"MAX_NUM_LIGHTS", m_max_num_lights}}
    );

    m_depth_buffer = gl::Renderbuffer(GL_DEPTH_COMPONENT32F, m_width, m_height);

    m_target_texture = gl::Texture2D(GL_RGBA, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE);
    m_target_texture.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_target_texture.setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_target_texture.setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    m_target_texture.setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

    m_target_buffer = gl::Framebuffer();
    m_target_buffer.attach(m_target_texture, GL_COLOR_ATTACHMENT0);
    m_target_buffer.attach(m_depth_buffer, GL_DEPTH_ATTACHMENT);
    m_target_buffer.setDrawBuffer({GL_COLOR_ATTACHMENT0});
}

void Renderer::render()
{
    glClearColor(m_fog_color.r, m_fog_color.g, m_fog_color.b, 1.0);
    gl::renderPass(
        m_mesh_vector,
        &MeshData::vao,
        &m_target_buffer,
        0, 0, m_width, m_height,
        GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
        GL_TRIANGLES,
        m_program,
        std::make_tuple(
            std::tuple("camera_hypersphere_orientation", m_camera.hypersphere_orientation),
            std::tuple("camera_orientation", m_camera.orientation),
            std::tuple("field_of_view", m_camera.field_of_view),
            std::tuple("aspect_ratio", m_aspect_ratio),
            std::tuple("far_plane", m_camera.far_plane),
            std::tuple("radius", m_hypershpere_radius),
            std::tuple("fog_color", m_fog_color),
            std::tuple("light_position", m_light_coords.size(), m_light_coords.data()),
            std::tuple("light_color", m_light_colors.size(), m_light_colors.data()),
            std::tuple("num_lights", (int) m_light_colors.size())
        ),
        std::make_tuple(
            std::tuple("model_hypersphere_orientation", &MeshData::hypersphere_orientation),
            std::tuple("model_orientation", &MeshData::model_orientation),
            std::tuple("diffuse_texture", &MeshData::texture),
            std::tuple("normal_map", &MeshData::normal_map)
        )
    );
    m_mesh_vector.clear();
    m_light_colors.clear();
    m_light_coords.clear();
}