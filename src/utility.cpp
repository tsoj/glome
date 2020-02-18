
#include "utility.hpp"
#include <sstream>
#include <fstream>


namespace utility
{
    std::vector<std::string> split(const std::string& s, const char delimiter)
    {
        std::vector<std::string> elements;
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delimiter))
        {
            if (!item.empty())
            {
                elements.push_back(item);
            }
        }
        return elements;
    }

    std::string readFile(const std::filesystem::path& file_path)
    {
        std::string ret;
        std::string line;
        std::ifstream file;
        file.open(file_path);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file: " + file_path.string());
        }
        while (std::getline(file, line, '\n'))
        {
            ret += line + '\n';
        }
        file.close();
        return ret;
    }

    std::string toString(const glm::vec3& v)
    {
        return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
    }

    std::string toString(const glm::vec4& v)
    {
        return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ", " + std::to_string(v.w) + ")";
    }
}
