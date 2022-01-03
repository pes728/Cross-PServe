//
//  common.h
//  PRender
//
//  Created by Peter Sandquist on 10/1/20.
//

#ifndef common_h
#define common_h
#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include "glm/glm.hpp"
#include <glm/gtx/norm.hpp>



const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385f;

inline float random_float();

inline float random_float(float min, float max);


glm::vec3 random_in_unit_sphere();

glm::vec3 random_in_unit_disk();

glm::vec3 random_unit_vector();

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

#endif /* common_h */
