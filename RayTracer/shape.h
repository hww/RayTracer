/**
*  @file shape.h
*  @poject RayTracer for online courses
*  @brief Shape primitive
*  @autor Valery P. (github.com/hww)
*/

#ifndef MyRayTracer_shape_h
#define MyRayTracer_shape_h

#include "intersection.h"
#include "ray.h"
#include "transform.h"

/**
 * Shape
 * 
 * Methods:
 * 
 *   Test if ray intersects with the shape or not (in object space), if so,
 *   return intersection point and normal
 * 
 *   bool intersect(Ray& ray, float* thit, LocalGeo* local)
 * 
 *   Same as intersect, but just return whether there is any intersection or
 *   not
 * 
 *   bool intersectP(Ray& ray)
 * 
 * Notes:
 *   Triangle and Sphere are probably best implemented here
 *   The intersection with the ray at t outside the range [t_min, t_max]
 *   should return false.
 */
class Shape {
public:
    Shape()
    { }
	/// Test intersection between shape and ray
	/// @param ray 
	/// @param[in,out] in intersection
	/// @param cull back face culling
	/// @result true for sucessfull intersection
	virtual bool intersect(Ray& ray, Intersection& intersection, bool cullBack) { return false; }
	/// Test intersection between shape and ray
	/// @param ray 
	/// @param cull back face culling
	/// @result true for sucessfull intersection
	virtual bool intersect(Ray& ray, bool cullBack) { return false; }
	/// Test intersection between shape and ray
	virtual bool pretransform(Transform& obj2world) { return false; }
	/// Test if ray hit front side of shape
	virtual bool isFront(const Vector& dir) { return true; }

};

#endif
