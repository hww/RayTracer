/**
*  @file light.h
*  @poject RayTracer for online courses
*  @brief The light source
*  @autor Valery P. (github.com/hww)
*/

#ifndef RayTracer2_light_h
#define RayTracer2_light_h

#include "group_primitives.h"

/**
 * Light
 * 
 * Methods:
 * 
 * void generateLightRay(LocalGeo& local, Ray* lray, Color* lcolor);
 * 
 * Notes:
 * 
 * This is an abstract class that will generate a ray starting from
 * the position stored in local to the position of the light source.
 * 
 * You might want to consider creating 2 derived classes for
 * point light source and directional light source.
 * For directional light, the origin of the ray is the same, and
 * the ray points to the light direction, however, t_max is infinity.
 */
class Light {
public:


	/// Create light source
	/// @param x position
	/// @param y position
	/// @param z position
	/// @param r color
	/// @param g color
	/// @param b color
	/// @param is_point is point light? Other case directional
	Light(float x, float y, float z, float r, float g, float b, bool is_point) : 
		pos(x,y,z), 
		color(r,g,b),
        isPoint(is_point),
        attenuationConst(1),
        attenuationLinear(0),
        attenuationQuadratic(0)
    {
    }

    // drop ray from light position to the destination point
	// @param local local geometry
	// @result ray
    Ray getRay(const SurfacePoint& surfPoint) {
        Ray ray;
        ray.pos = surfPoint.position; // source point from pixel
        ray.target = pos;    // target point to light for debugging
        ray.tmin = 0.001f;
        ray.tmax = FLT_MAX;

        if (isPoint) {
            ray.dir = -(surfPoint.position - pos); // dirrecttion to light
            ray.tmax = ray.dir.magnitude();
            ray.dir.norm();
        } else {
            // pos - Is The direction to the light source, and the color, as in OpenGL.
            // the ray must be dirrection to light
            ray.dir = pos; 
            ray.dir.norm();
        }
        return ray;
    }
	/// get color of light for given ray
	/// @param ray to the light
    Color getColor(const Ray& ray) {
        if (isPoint) {
            float linear = attenuationLinear * ray.tmax;
            float quadra = attenuationQuadratic * ray.tmax * ray.tmax;
            float attenuation = (1 / (attenuationConst + linear + quadra));
            return color * attenuation;
        } else {
            return color;
        }
    }

	bool isPoint;   //< is this light is color light
	Vector pos;     //< light position
	Color color;    //< light color

	/// attenuation const linear quadratic Sets the constant, linear
	/// and quadratic attenuations (default 1,0,0) as in OpenGL.
	/// By default there is no attenuation (the constant term is 1,
	/// linear and quadratic are 0; that's what we mean by 1,0,0).
	float attenuationConst;
	float attenuationLinear;
	float attenuationQuadratic;
};

#endif
