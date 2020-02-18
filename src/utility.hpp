#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <filesystem>
#include "ascii_framebuffer.hpp"

namespace utility
{
    std::vector<std::string> split(const std::string& s, char delimiter);

    std::string readFile(const std::filesystem::path& file_path);

    std::string toString(const glm::vec3& v);

    std::string toString(const glm::vec4& v);

#ifdef USE_ASCII_FRAMEBUFFER
    using namespace ascii_framebuffer;
    inline Framebuffer text_buffer;
#endif

}
