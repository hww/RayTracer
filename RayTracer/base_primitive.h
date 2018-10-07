/**
*  @file base_primitive.h
*  @poject RayTracer for online courses
*  @brief Basic material
*  @autor Valery P. (github.com/hww)
*/

#ifndef RayTracer2_base_primitive_h
#define RayTracer2_base_primitive_h

#include "ray.h"
#include "intersection.h"

class BasePrimitive
{
public:
	/// Constructor
	BasePrimitive()
	{
	}
	/// Test intersection between ray and this primitive
	/// @param ray 
	/// @param cull back face culling
	/// @result true for sucessfull intersection
	virtual bool intersect(Ray& ray, bool cullBack) = 0;
	/// Test intersection between ray and this primitive
	/// @param ray 
	/// @param[in,out] intersection will contain result of intersection
	/// @param cull back face culling
	/// @result true for sucessfull intersection
	virtual bool intersect(Ray& ray, Intersection& intersection, bool cullBack) = 0;
};

#endif

