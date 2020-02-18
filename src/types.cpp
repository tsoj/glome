#include "types.hpp"
#include "shared_glm_glsl.h"

Plane::operator glm::mat2x4() const
{
    if (glm::abs(glm::dot(vectors[0], vectors[1])) > bias)
    {
        throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " +
                                 "Vectors of plane are not orthogonal: " + "glm::dot(vectors[0], vectors[1])) = " +
                                 std::to_string(glm::dot(vectors[0], vectors[1]))
        );
    }
    return vectors;
}
