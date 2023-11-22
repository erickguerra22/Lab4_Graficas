#pragma once
#include <glm/glm.hpp>
#include <string>

struct Planet {
    float rotation;
    glm::vec3 translation;
    glm::vec3 scale;
    bool hasMoon = false;
};