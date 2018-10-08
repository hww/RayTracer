/**
*  @file types.cpp
*  @poject RayTracer for online courses
*  @brief Type casting and other functions
*  @autor Valery P. (github.com/hww)
*/

#include "types.h"

/// Generate random value in given range between
inline float randomInRange(float min, float max)
{
    return (min + 1) + (((float)rand()) / (float)RAND_MAX) * (max - (min + 1));
}
