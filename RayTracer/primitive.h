/**
*  @file primitive.h
*  @poject RayTracer for online courses
*  @brief Basic primitive
*  @autor Valery P. (github.com/hww)
*/

#ifndef MyRayTracer_primitive_h
#define MyRayTracer_primitive_h

#include "shape.h"
#include "material.h"
#include "ray.h"
#include "intersection.h"
#include "transform.h"

/**
 * Primitive
 *
 * Methods:
 *   bool intersect(const Ray ray,  Intersection& intersection, bool cullBack) 
 *   bool intersect(Ray ray, bool cullBack)
 *   Material* getMaterial(const SurfacePoint& surfPoint)
 *
 * Notes:
 *   Abstract class for primitives in the scene
 */
class Primitive {
public:
    /// Construct primitive with given shape
    /// @param shape will be used as shape of this primitive
    Primitive(Shape* obj) {
        shape = obj;
        isTransformed = false;
    }

    void setTransform(Transform transform) {
        objToWorld = transform;
        worldToObj = Transform(glm::inverse(transform.m));
        isTransformed = true;
    }

    void setTransform(Transform transform, bool pretransform) {
        objToWorld = transform;
        worldToObj = Transform(glm::inverse(transform.m));
        if (pretransform) {
            isTransformed = !shape->pretransform(objToWorld);
        } else
            isTransformed = true;
    }
    /// Test intersection between shape and ray
    /// @param ray
    /// @param[in,out] in intersection
    /// @param cullBack back face culling
    /// @result true for successful intersection
    bool intersect(const Ray ray,  Intersection& intersection, bool cullBack) {
        Ray tray = ray;
        if(isTransformed)
            worldToObj.transform(tray);
        if(shape->intersect(tray, intersection, cullBack)) {
            intersection.primitive = this;
            if(isTransformed)
                objToWorld.transform(intersection.surfacePoint);
            // now calculate new distance
            intersection.distance = (intersection.surfacePoint.position - ray.pos).magnitude();
            return true;
        }
        return false;
    }
    /// Test intersection between shape and ray
    /// @param ray
    /// @param cullBack back face culling
    /// @result true for successful intersection
    bool intersect(Ray ray, bool cullBack) {
        Ray tray = ray;
        if(isTransformed)
            worldToObj.transform(tray);
        return shape->intersect(tray, cullBack);
    }

    /// Get material of this primitive
    /// @param local can be used for procedural materials
    /// @result BRDF of this material
    Material* getMaterial(const SurfacePoint& surfPoint) {
        return material;
    }

    /// Test if this ray hit front surface of primitive
    bool isFront(const Vector& dir) { return shape->isFront(dir); }

    /// Object to world transform
    Transform objToWorld;
    /// World to object transform
    Transform worldToObj;
    /// The material of surface
    Material* material;
    /// Is this primitive already transformed (performance optimization)
    bool isTransformed;
    /// Pointer to the shape
    Shape *shape;
    /// Line in the source code
    int sourceLine;
};

#endif
