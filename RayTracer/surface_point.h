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
 *   Point pos
 *   Normal normal
 * 
 * Notes:
 *   Store the local geometry at the intersection point. 
 * ToDo:
 *   May need to store other quantities (eg. texture coordinate) in 
 *   a more complicated raytracer.
 */
class SurfacePoint {
public:
    SurfacePoint() {}
    Vector position;
    Vector normal;
};

#endif
