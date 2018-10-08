/**
*  @file collision_point.h
*  @poject RayTracer for online courses
*  @brief Main renderer's header file
*  @autor Valery P. (github.com/hww)
*/

#ifndef MyRayTracer_surface_point_h
#define MyRayTracer_surface_point_h


#include "vector.h"
/**
 * SurfacePoint
 *   Contains the collision information between ray and geometry or primitive
 *
 * Members:
 *   Vector position
 *   Vector normal
 *
 * Notes:
 *   Store the local geometry at the intersection point.
 * ToDo:
 *   May need to store other quantities (aka texture coordinate) in
 *   a more complicated ray-tracer.
 */
class SurfacePoint {
public:
    SurfacePoint() {}
    Vector position;
    Vector normal;
};

#endif
