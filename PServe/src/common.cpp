#include "common.h"


float random_float() {
    return rand() / (RAND_MAX + 1.0f);
}

float random_float(float min, float max) {
    return min + (max - min) * random_float();
}


glm::vec3 random_in_unit_sphere() {
    while (true) {
        glm::vec3 p = glm::vec3(random_float(-1, 1), random_float(-1, 1), 0);
        if (glm::length2(p) >= 1) continue;
        return p;
    }
}

glm::vec3 random_in_unit_disk() {
    while (true) {
        glm::vec3 p = glm::vec3(random_float(-1, 1), random_float(-1, 1), random_float(-1, 1));
        if (glm::length2(p) >= 1) continue;
        return p;
    }
}

glm::vec3 random_unit_vector() {
    float a = random_float(0, 2 * pi);
    float z = random_float(-1, 1);
    float r = glm::sqrt(1 - z * z);
    return glm::vec3(r * cos(a), r * sin(a), z);
}