/**
*  @file sphere.h
*  @poject RayTracer for online courses
*  @brief Sphere primitive
*  @autor Valery P. (github.com/hww)
*/

#ifndef RayTracer2_sphere_h
#define RayTracer2_sphere_h

#include "vector.h"
#include "ray.h"

/**
 * Sphere primitive
 *
 * Fields:
 *   center position
 *   radius
 */
class Sphere : public Shape {
public:
    /// Construct sphere
    /// @param pos position of center
    /// @param rad sphere radius
    Sphere(const Vector & pos, float rad) {
        center = pos;
        radius = rad;
    }
    /// Test intersection between sphere and ray
    /// @param ray
    /// @param[in,out] oIntersect intersection
    /// @param cullBack to cull back face culling (not used)
    /// @result true for successful intersections
    bool intersect(Ray& ray, Intersection& intersection, bool cullBack) {
        float dist = quadraticForm(ray);
        if (dist<0)
            return false;
        intersection.surfacePoint.position = ray.pos + ray.dir * dist;
        intersection.surfacePoint.normal = (intersection.surfacePoint.position - center) * (float)(1.0/radius); // normalize
        return true;
    }
    /// Test intersection between sphere and ray
    /// @param ray
    /// @param cullBack to cull back face culling (not used)
    /// @result true for successful intersections
    bool intersect(Ray & ray, bool cullBack) {
        double dist = quadraticForm(ray);
        return dist>=0;
    }
    /// Sphere center position
    Vector center;
    /// Sphere's radius
    float radius;

private:
    float quadraticForm(const Ray& ray) {
        Vector tmp = ray.pos - center;
        float a = Vector::dot(ray.dir, ray.dir);
        float b = Vector::dot(ray.dir, tmp) * 2.0f;
        float c = Vector::dot(tmp, tmp) - radius * radius;
        float discriminant = b * b - 4.0f * a * c;
        if (discriminant < 0) {
            // if discriminant is negative there are no real roots, so return
            // false as ray misses sphere
            return -1;
        } else {
            float sqrtd = sqrt(discriminant);
            float dist1 = (-b + sqrtd) / (2.0f * a);
            float dist2 = (-b - sqrtd) / (2.0f * a);
            float mindist = min(dist1,dist2);
            if (mindist >= ray.tmin && mindist <= ray.tmax) {
                return mindist;
            } else {
                return -1;
            }
        }
    }

};


#endif
