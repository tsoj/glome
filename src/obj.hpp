#pragma once

#include <vector>
#include <filesystem>
#include <string>
#include <map>
#include "Vertex.hpp"

namespace obj
{
    struct Material
    {
        //TODO: Class 3: use PBR, best if integrable with blender
        std::filesystem::path diffuse_map;
        std::filesystem::path normal_map;

    private:

        inline static const std::map<std::string, std::filesystem::path Material::*> mtl_mapping =
            {
                {"map_Kd",     &Material::diffuse_map},
                {"map_normal", &Material::normal_map}
            };

    public:

        static std::map<std::string, Material> getMtl(
            const std::filesystem::path& file_path,
            std::map<std::string, Material> materials = std::map<std::string, Material>()
        );
    };


    struct Object
    {
        std::vector<Vertex> vertices;
        Material material;
    };

    std::vector<Object> getObj(const std::filesystem::path& file_path);
}