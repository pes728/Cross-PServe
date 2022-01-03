#include "aabb.h"

aabb surrounding_box(aabb box0, aabb box1) {
    glm::vec3 small(fminf(box0.min().x, box1.min().x),
        fminf(box0.min().y, box1.min().y),
        fminf(box0.min().z, box1.min().z));

    glm::vec3 big(fmaxf(box0.max().x, box1.max().x),
        fmaxf(box0.max().y, box1.max().y),
        fmaxf(box0.max().z, box1.max().z));

    return aabb(small, big);
}