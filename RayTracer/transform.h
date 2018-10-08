/**
*  @file transform.h
*  @poject RayTracer for online courses
*  @brief The transform matrix
*  @autor Valery P. (github.com/hww)
*/

#ifndef MyRayTracer_transformation_h
#define MyRayTracer_transformation_h

#include "types.h"
#include "matrix.h"
#include "surface_point.h"

class Ray;

/**
 * Contains matrix 'm' and its inverse version 'minvt' (for transforming normals)
 */
class Transform {
public:

    /// Construct default transform matrix
    Transform() : m(1.0f), minvt(1.0f) {
    }
    /// Construct transform from given matrix
    /// @param m the transformation matrix
    Transform(mat4 m) {
        setTransformation(m);
    }
    /// Set transform to given matrix
    /// @params m_ new transformation matrix
    void setTransformation(mat4 m_) {
        m = m_;
        minvt = glm::transpose(glm::inverse(m_));
    }

    /* -- Transform primitives - */

    /// Transform vector
    /// @params v input vector
    /// @result is transformed vector
    Vector transform(Vector v);
    /// Transform ray
    /// @param[in,out] ray will be updated
    void transform(Ray& ray);
    /// Transform geometry
    /// @param[in,out] ray will be updated
    void transform(SurfacePoint &surfPoint);

    /* -- Create new matrices - */

    /// Create look at matrix
    /// @param camPosition camera position
    /// @param targPosition target point position
    /// @param up desired up vector
    static mat4 lookAt(const vec3& camPosition, const vec3 &targPosition, const vec3& up);
    /// Create perspective matrix
    /// @param fovy vertical field of view
    /// @param aspect the aspect ratio
    /// @param near plane
    /// @param far plane
    static mat4 perspective(float fovy, float aspect, float near, float far);
    /// Create rotation matrix
    /// @param angle the rotation angle
    /// @param axis trotate aroutd this axis
    static mat3 rotate(const vec3& axis, const float angle) ;
    /// Create scale matrix
    /// @param sx scale
    /// @param sy scale
    /// @param sz scale
    static mat4 scale(const float &sx, const float &sy, const float &sz) ;
    /// Create translation matrix
    /// @param tx translate position
    /// @param ty translate position
    /// @param tz translate position
    static mat4 translate(const float &tx, const float &ty, const float &tz);

    mat4 m;     // matrix
    mat4 minvt; // inverse/transpose matrix
};


#endif
