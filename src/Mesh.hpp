#pragma once

#include "gl.hpp"
#include "Vertex.hpp"
#include "obj.hpp"

struct Mesh
{
    gl::VertexArray vao;
    gl::Texture2D texture;
    gl::Texture2D normal_map;
    std::vector<Vertex> vertices;
};

inline std::vector<Mesh> getMeshesFromObj(const std::filesystem::path& file_path)
{
    std::vector<Mesh> ret;
    auto obj = obj::getObj(file_path);
    for (const auto& o : obj)
    {
        gl::VertexArray vao = gl::VertexArray(o.vertices);
        vao.setVertexAttribPointer(
            0, sizeof(Vertex::position)/sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position));
        vao.setVertexAttribPointer(
            1, sizeof(Vertex::normal)/sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, normal));
        vao.setVertexAttribPointer(
            2, sizeof(Vertex::tangent)/sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, tangent));
        vao.setVertexAttribPointer(
            3, sizeof(Vertex::texture_coordinate)/sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, texture_coordinate));

        gl::Texture2D texture = gl::Texture2D(o.material.diffuse_map);
        gl::Texture2D normal_map = gl::Texture2D(o.material.normal_map);

        ret.emplace_back();
        ret.back().vao = vao;
        ret.back().texture = texture;
        ret.back().normal_map = normal_map;
        ret.back().vertices = o.vertices;
    }
    return ret;
}