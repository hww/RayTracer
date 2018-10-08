/**
*  @file ray.h
*  @poject RayTracer for online courses
*  @brief Single Ray with position and direction
*  @autor Valery P. (github.com/hww)
*/

#ifndef MyRayTracer_ray_h
#define MyRayTracer_ray_h

#include "vector.h"
#include "debug.h"
#include "transform.h"

/**
 * Members:
 *   Point pos
 *   Vector dir
 *   float t_min, t_max
 *
 * Notes:
 *   It represent the ray ray(t) = pos + t*dir, where t_min <= t <= t_max
 */
class Ray {
public:
    /// Create new ray
    Ray() {

    }
    /// Create new ray in possition with direction
    Ray(const Vector& pos, const Vector& dir) : pos(pos), dir(dir) {}
    /// Convert ray to the string
    std::string toString() {
        return format("[%f,%f,%f]-[%f,%f,%f]", pos.x,pos.y,pos.z, dir.x,dir.y,dir.z);
    }
    /// Get ray's end point
    Vector getPoint(float dist) {
        return pos + dir * dist;
    }
    /// Reflect vector from surface with normal
    /// @param collision vector's collision point
    /// @result vector after reflection
    Ray reflect(const SurfacePoint& surfPoint) {
        Ray ray;
        ray.pos = surfPoint.position;
        ray.dir = reflect(dir, surfPoint.normal);
        ray.dir.norm();
        ray.tmin = 0.001f;
        ray.tmax = FLT_MAX;
        return ray;
    }
    /// Refract vector by surface with normal
    /// @param collision collision point
    /// @param eta refraction value
    /// @result vector after refraction
    Ray refract(const SurfacePoint& surfPoint, float eta) {
        Ray ray;
        Vector tmpnormal = surfPoint.normal;
        if (Vector::dot(dir, tmpnormal) > 0)
            tmpnormal = -tmpnormal;
        ray.pos = surfPoint.position;
        ray.dir = refract(dir, tmpnormal, eta);
        ray.dir.norm();
        ray.tmin = 0.001f;
        ray.tmax = FLT_MAX;
        return ray;
    }
    /// Reflect vector from surface with normal
    /// @param dir vector direction
    /// @param normal surface normal
    /// @result vector after reflection
    inline Vector reflect(const Vector &dir, const Vector &normal)
    {
        return dir - (normal * (2.0f * Vector::dot(dir, normal)));
    }
    /// Refract vector by surface with normal
    /// @param dir vector direction
    /// @param eta refraction value
    /// @result vector after refraction
    inline Vector refract(const Vector &dir, const Vector &normal, float eta)
    {
        float ndang = (float)Vector::dot(normal, dir);
        float k = 1 - eta * eta * (1 - ndang * ndang);
        if (k < 0)
            return Vector(0, 0, 0);
        else
            return dir * eta - normal * (eta * ndang + sqrtf(k));
    }
    /// Generate random ray from collision point
    /// @param surfPoint
    /// @param inacuracy
    Ray random(const SurfacePoint& surfPoint, float inacuracy) {
        Ray ray;
        Vector rnd = Vector::random(inacuracy);
        if (Vector::dot(rnd, surfPoint.normal)<0)
            rnd = -rnd;
        ray.pos = surfPoint.position;
        ray.dir = rnd;
        ray.dir.norm();
        ray.tmin = 0.001f;
        ray.tmax = FLT_MAX;
        return ray;
    }

    Vector pos;     ///< Ray position
    Vector dir;     ///< Ray direction
    float tmin;     ///< Ray minimum distance
    float tmax;     ///< Ray maximum distance
    Vector target;  ///< debugging purposes
};

#endif
