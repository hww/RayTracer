/**
*  @file camera.h
*  @poject RayTracer for online courses
*  @brief Camera class
*  @autor Valery P. (github.com/hww)
*/

#ifndef RayTracer2_camera_h
#define RayTracer2_camera_h

#include <cmath>
#include "vector.h"
#include "ray.h"
#include "transform.h"
#include "group_primitives.h"

/**
 * Simple camera for ray tracer
 */
class Camera {
public:
	/// Construct new camera
	/// @param fovy vertical field of view
	/// @param aspect aspect ratio
	/// @param near distance to near plane
	/// @param far distance to far plane
	Camera(float fovy, float aspect, float near, float far) {
        this->aspect = aspect;
        this->near = near;
        this->far = far;
        this->fov = fovy;
        this->tanfov = tanf(glm::radians(fov)*0.5f);
        projection = Transform::perspective(fovy, aspect, near, far);
    }
    /// Set camera transform to position and 'look at' orientation
	/// @param campos camera position
	/// @param lookat look at point
	/// @param desiredup vector up
	void lookAt(const Vector& campos, const Vector& lookat, const Vector& desiredup) {
        this->pos = campos;
        this->dir = lookat - campos;
        // look at returns object to camera transformation as
        // camera in the center of the world and looking to -Z
        worldToCamera = Transform::lookAt(campos, lookat, desiredup);
        cameraToWorld = glm::inverse(worldToCamera.m);
    }
    /// Generate camera ray
	/// @param sample coordinate of sample
	/// @result new ray for this sample point
    Ray GenerateRay(Vector& sample) 
	{
        float x = (float)sample.x;
        float y = (float)sample.y;
        float dx = tanfov * (-x) * aspect;
        float dy = tanfov * (-y);
        vec4 p1 = vec4(dx * -near, dy * -near, -near, 1);
        vec4 p2 = vec4(dx * -far,  dy * -far,  -far, 1);
        vec3 w1 = dehomog(cameraToWorld.m * p1);
        vec3 w2 = dehomog(cameraToWorld.m * p2);
        vec3 dir3 = glm::normalize(w2 - w1);
        Ray ray;
        ray.pos = pos;
        ray.dir = dir3;
		ray.dir.norm();
        ray.tmin = near; //1;
        ray.tmax = far; //DBL_MAX;
        return ray;
    }
public:
    float aspect;				//< aspect ratio
    float fov;					//< field of view
    float near;					//< near plane
    float far;					//< far plane
    float tanfov;				//< tangent of the fov (acceleration)
    Vector pos;					//< camera position
    Vector dir;					//< camera direction
    mat4 worldToUV;				//< world to UV matrix
    mat4 uvToWorld;				//< UV to world matrix
    Transform projection;		//< projection matrix
    Transform worldToCamera;	//< world to camera transform
    Transform cameraToWorld;	//< camera to worl transform
private:
	/// Convert vec4 to vec3
	inline vec3 dehomog(vec4 v) {
		return vec3(v.x / v.w, v.y / v.w, v.z / v.w);
	}
};

#endif
