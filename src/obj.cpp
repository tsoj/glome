#include "obj.hpp"
#include "utility.hpp"
#include <glm/glm.hpp>
#include <sstream>
#include <fstream>

namespace obj
{
    std::map<std::string, Material> Material::getMtl(
        const std::filesystem::path& file_path,
        std::map<std::string, Material> materials
    )
    {
        if (!file_path.has_filename())
        {
            throw std::runtime_error("Not a path to a file: " + file_path.string());
        }

        std::ifstream file;
        file.open(file_path);

        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file: " + file_path.string());
        }

        std::string current_material_name;

        std::string line;
        while (getline(file, line))
        {
            auto words = utility::split(line, ' ');
            if (words.empty())
            {
                continue;
            }
            else if (words[0] == "newmtl")
            {
                current_material_name = words[1];
                if (materials.count(current_material_name) != 0)
                {
                    throw std::runtime_error("Multiple material definitions: " + current_material_name);
                }
                materials[current_material_name] = Material{};
            }
            else if (Material::mtl_mapping.count(words[0]) != 0)
            {
                auto& material_property = materials.at(current_material_name).*(Material::mtl_mapping.at(words[0]));
                material_property = file_path;
                material_property.replace_filename({words[1]});
            }
        }
        return materials;
    }

    std::vector<Object> getObj(const std::filesystem::path& file_path)
    {
        if (!file_path.has_filename())
        {
            throw std::runtime_error("Not a path to a file: " + file_path.string());
        }

        std::ifstream file;
        file.open(file_path);

        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file: " + file_path.string());
        }

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> texture_coordinates;
        std::vector<glm::vec3> normals;
        std::map<std::string, Material> materials;
        std::vector<Object> objects;

        std::string line;
        while (std::getline(file, line))
        {
            auto words = utility::split(line, ' ');
            if (words.empty())
            {
                continue;
            }
            if (words[0] == "o")
            {
                objects.emplace_back();
            }
            else if (words[0] == "mtllib")
            {
                std::filesystem::path mtl_path = file_path;
                mtl_path.replace_filename({words[1]});
                materials = Material::getMtl(mtl_path, materials);
            }
            else if (words[0] == "usemtl")
            {
                objects.back().material = materials.at(words[1]);
            }
            else if (words[0] == "v")
            {
                if (words.size() != 4)
                {
                    throw std::runtime_error("Only three-dimensional position vectors are supported: " + line);
                }
                positions.emplace_back(glm::vec3(
                    std::stof(words[1]),
                    std::stof(words[2]),
                    std::stof(words[3])
                ));
            }
            else if (words[0] == "vt")
            {
                if (words.size() != 3)
                {
                    throw std::runtime_error("Only two-dimensional texture coordinates are supported: " + line);
                }
                texture_coordinates.emplace_back(glm::vec2(
                    std::stof(words[1]),
                    1.0 - std::stof(words[2])
                ));
            }
            else if (words[0] == "vn")
            {
                if (words.size() != 4)
                {
                    throw std::runtime_error("Only three-dimensional normal vectors are supported: " + line);
                }
                normals.emplace_back(glm::vec3(
                    std::stof(words[1]),
                    std::stof(words[2]),
                    std::stof(words[3])
                ));
            }
            else if (words[0] == "f")
            {
                if (words.size() != 4)
                {
                    throw std::runtime_error("Only triangular faces are supported: " + line);
                }
                for (size_t i = 1; i < 4; ++i)
                {
                    objects.back().vertices.emplace_back();
                    auto triplets = utility::split(words[i], '/');
                    if (triplets.size() != 3)
                    {
                        throw std::runtime_error("Only faces of the form 'f v/vt/vn v/vt/vn v/vt/vn v/vt/vn' are supported: " + line);
                    }
                    if (triplets[0].empty())
                    {
                        objects.back().vertices.back().position = glm::vec3(0.0, 0.0, 0.0);
                    }
                    else
                    {
                        size_t v = std::stol(triplets[0]) - 1;
                        objects.back().vertices.back().position = positions[v];
                    }
                    if (triplets[1].empty())
                    {
                        objects.back().vertices.back().texture_coordinate = glm::vec2(-1.0, -1.0);
                    }
                    else
                    {
                        size_t vt = std::stol(triplets[1]) - 1;
                        objects.back().vertices.back().texture_coordinate = texture_coordinates[vt];
                    }
                    if (triplets[2].empty())
                    {
                        objects.back().vertices.back().normal = glm::vec3(0.0, 1.0, 0.0);
                    }
                    else
                    {
                        size_t vn = std::stol(triplets[2]) - 1;
                        objects.back().vertices.back().normal = normals[vn];
                    }
                }
                const auto size = objects.back().vertices.size();
                if (objects.back().vertices[size - 1].texture_coordinate == glm::vec2(-1.0, -1.0))
                {
                    objects.back().vertices[size - 1].texture_coordinate = glm::vec2(1.0, 1.0);
                    objects.back().vertices[size - 2].texture_coordinate = glm::vec2(0.0, 1.0);
                    objects.back().vertices[size - 3].texture_coordinate = glm::vec2(0.0, 0.0);
                }

                // calculate the tangent
                glm::vec3 v_0 = objects.back().vertices[size - 1].position;
                glm::vec3 v_1 = objects.back().vertices[size - 2].position;
                glm::vec3 v_2 = objects.back().vertices[size - 3].position;

                glm::vec3 dv_0 = v_1 - v_0;
                glm::vec3 dv_1 = v_2 - v_0;

                glm::vec2 t_0 = objects.back().vertices[size - 1].texture_coordinate;
                glm::vec2 t_1 = objects.back().vertices[size - 2].texture_coordinate;
                glm::vec2 t_2 = objects.back().vertices[size - 3].texture_coordinate;

                glm::vec2 dt_0 = t_1 - t_0;
                glm::vec2 dt_1 = t_2 - t_0;

                glm::vec3 tmp_tangent = (dv_0 * dt_1.y - dv_1 * dt_0.y) / (dt_0.x * dt_1.y - dt_0.y * dt_1.x);

                for (size_t i = 1; i < 4; i++)
                {
                    glm::vec3 bi_tangent = glm::cross(tmp_tangent, objects.back().vertices[size - i].normal);
                    objects.back().vertices[size - i].tangent = glm::cross(objects.back().vertices[size - i].normal, bi_tangent);
                }
            }
        }

        return objects;
    }
}