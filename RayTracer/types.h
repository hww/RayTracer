/**
*  @file types.h
*  @poject RayTracer for online courses
*  @brief Commonly used data types and macro definitions
*  @autor Valery P. (github.com/hww)
*/

#ifndef RayTracer3_types_h
#define RayTracer3_types_h

#include <cmath>
#include <float.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/random.hpp" // vecRand3

const float PI = 3.14159265f; // N.B. For portability across platforms

#define DEG2RAD(degrees) (degrees * (PI/180))
#define RAD2DEG(radians) (radians * (180/PI))
#define max(left,right) (left > right ? left : right)
#define min(left,right) (left < right ? left : right)

typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef glm::mat3 mat3;
typedef glm::mat4 mat4;

/// Generate random value in given range
extern float randomInRange(float a, float b);

#endif
