/**
*  @file triangle.h
*  @poject RayTracer for online courses
*  @brief Single triangle
*  @autor Valery P. (github.com/hww)
*/

#ifndef RayTracer2_triangle_h
#define RayTracer2_triangle_h

#include "vector.h"
#include "ray.h"
#include "matrix.h"

/**
 * @brief Contains three world positions and three normals. Additionally
 * contains normal for planar triangle.
 */
class Triangle : public Shape {
public:
    /// Construct planar triangle
    /// @param v1 point one
    /// @param v2 point two
    /// @param v3 point three
    Triangle(Vector v1, Vector v2, Vector v3) {
        a = v1;
        b = v2;
        c = v3;
        normal = Vector::cross((b - a), (c - a));
        normal.norm();
    }
    /// Construct non planar triangle
    /// @param v1 point one
    /// @param v2 point two
    /// @param v3 point three
    /// @param n1 normal one
    /// @param n2 normal two
    /// @param n3 normal three
    Triangle(Vector v1, Vector v2, Vector v3, Vector n1, Vector n2, Vector n3) {
        a = v1;
        b = v2;
        c = v3;
        normal = Vector::cross((b - a), (c - a));
        normal.norm();
    }
    /// Test intersection between this triangle and ray.
    /// @param ray
    /// @param[out] intersection Intersection
    /// @param cullBack In case if it is true then ignore back side of triangle
    /// @result true for successful intersection
    bool intersect(Ray& ray, Intersection& intersection, bool cullBack) {
        float dist = intersect_triangle(ray, cullBack);
        if (dist >= ray.tmin && dist <= ray.tmax) {
            intersection.surfacePoint.position = ray.pos + ray.dir * dist;
            intersection.surfacePoint.normal = normal; // normalize
            return true;
        }
        else
            return false;
    }
    /// Test intersection between this triangle and ray.
    /// @param ray the ray to intersect
    /// @param cullBack In case if it is true then ignore back side of triangle
    /// @result true for successful intersection
    bool intersect(Ray& ray, bool cullBack) {
        double dist = intersect_triangle(ray, cullBack);
        return (dist >= ray.tmin && dist <= ray.tmax);
    }
    /// Pre transform the triangle
    /// @param obj2word transform matrix
    /// @result always true
    virtual bool pretransform(Transform& obj2world) {
        a = obj2world.transform(a);
        b = obj2world.transform(b);
        c = obj2world.transform(c);
        normal = Vector::cross((b - a), (c - a));
        normal.norm();
        return true;
    }
    /// Test if light ray's direction hit front face of this triangle
    /// @param direction the light's direction
    /// @result true for front face of this triangle
    virtual bool isFront(const Vector& dir) {
        return Vector::dot(dir, -normal) >= 0;
    }

private:

    Vector a, b, c;     //< vertices
    Vector an, bn, cn;  //< normals for non planar triangle
    Vector normal;      //< normal for planar triangle

private:

    /// Test intersection of this triangle with given ray.
    /// In case if @param backCull is true then ignore back side
    /// of triangle
    float intersect_triangle(const Ray & ray, bool cullBack)
    {
        const float EPSILON = 0.000001f;
        /* ignore second side */
        if (cullBack && !isFront(ray.dir))
            return -1;
        /* find vectors for two edges sharing vert0 */
        Vector edge1 = b - a;
        Vector edge2 = c - a;
        /* begin calculating determinant - also used to calculate U parameter */
        Vector pvec = Vector::cross(ray.dir, edge2);
        /* if determinant is near zero, ray lies in plane of triangle */
        float det = Vector::dot(edge1, pvec);
        if (det > -EPSILON && det < EPSILON)
            return -1;
        float inv_det = 1.0f / det;
        /* calculate distance from vert0 to ray origin */
        Vector tvec = ray.pos - a;
        /* calculate U parameter and test bounds */
        float u = Vector::dot(tvec, pvec) * inv_det;
        if (u < 0.0f || u > 1.0f)
            return -1.0f;
        /* prepare to test V parameter */
        Vector qvec = Vector::cross(tvec, edge1);
        /* calculate V parameter and test bounds */
        float v = Vector::dot(ray.dir, qvec) * inv_det;
        if (v < 0.0f || u + v > 1.0f)
            return -1.0f;
        /* calculate t, ray intersects triangle */
        float t = Vector::dot(edge2, qvec) * inv_det;
        return t;
    }

    /// Test intersection of this triangle with given ray
    double intersect_triangle_alt(const Ray & ray)
    {
        mat3 m = mat3(a - b, a - c, ray.dir);
        float determinant = glm::determinant(m);
        // compute 't' value
        float t = glm::determinant(mat3(a - b, a - c, a - ray.pos)) / determinant;
        if (t < ray.tmin || t > ray.tmax)
            return -1;
        float gamma = glm::determinant(mat3(a - b, a - ray.pos, ray.dir)) / determinant;
        if (gamma < 0 || gamma > 1)
            return -1;
        float beta = glm::determinant(mat3(a - ray.pos, a - c, ray.dir)) / determinant;
        if (beta < 0 || beta > 1 - gamma)
            return -1;
        return t;
    }

};

#endif
