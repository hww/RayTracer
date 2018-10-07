/**
*  @file intersection.h
*  @poject RayTracer for online courses
*  @brief Main renderer's header file
*  @autor Valery P. (github.com/hww)
*/

#ifndef MyRayTracer_intersection_h
#define MyRayTracer_intersection_h

#include "surface_point.h"
#include "types.h"

class Primitive;

/**
 * Intersection
 * 
 * Members:
 * CollisionPoint localGeo
 * Primitive* primitive
 */
class Intersection {
public:
	/// Construct intersection
    Intersection() : distance(DBL_MAX), primitive(NULL) {}
	/// Pinter to primitive
    Primitive* primitive;
	/// Collision point
    SurfacePoint surfacePoint;
	/// Distance to intersection
    double distance;
};

#endif
