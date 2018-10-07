/**
*  @file transform.cpp
*  @poject RayTracer for online courses
*  @brief Type casting and other functions
*  @Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
*  @autor Valery P. (github.com/hww)
*/

#include "transform.h"
#include "ray.h"

// Transform vector
Vector Transform::transform(Vector v) {
	vec4 nv = m * vec4(v.x, v.y, v.z, 1);
	return Vector(nv.x, nv.y, nv.z);
}
// Transform ray
void Transform::transform(Ray& ray) {
	ray.pos = m * ray.pos.toVec4(1);
	ray.dir = m * ray.dir.toVec4(0);
	// we asume that befor transformation, the dir was normalized
	float magn = ray.dir.magnitude();
	// if it increase lenght then we have to increase this values
	ray.tmin *= magn;
	ray.tmax *= magn;
	// do not forget normalize dir again
	ray.dir /= magn;
}
// Transform surface point
void Transform::transform(SurfacePoint &surfPoint) {
	surfPoint.position = m * surfPoint.position.toVec4(1);
	surfPoint.normal = minvt * surfPoint.normal.toVec4(0);
	surfPoint.normal.norm();
}
// Create rotation matrix
mat3 Transform::rotate(const vec3& axis, const float angle)
{
	float angleRad = DEG2RAD(angle);
	float cosA = cos(angleRad);
	float sinA = sin(angleRad);

	vec3 axisn = glm::normalize(axis);
	float x = axisn.x;
	float y = axisn.y;
	float z = axisn.z;

	mat3 m1(1, 0, 0, 0, 1, 0, 0, 0, 1);
	mat3 m2(x*x, x*y, x*z,
		x*y, y*y, y*z,
		x*z, y*z, z*z);
	mat3 m3(0, z, -y,
		-z, 0, x,
		y, -x, 0);

	return cosA * m1 + (1 - cosA) * m2 + sinA * m3;
}
// create lookAt matrix
mat4 Transform::lookAt(const vec3 &camPosition, const vec3 &targPosition, const vec3 &up)
{
	vec3 e = camPosition - targPosition; // this order because camera looks at -Z
	vec3 zaxis = glm::normalize(e);
	vec3 xaxis = glm::normalize(glm::cross(up, zaxis));
	vec3 yaxis = glm::cross(zaxis, xaxis);

	double tx = Vector::dot(xaxis, -e);
	double ty = Vector::dot(yaxis, -e);
	double tz = Vector::dot(zaxis, -e);

	mat4 rot;
	rot[0] = vec4(xaxis.x, yaxis.x, zaxis.x, 0);
	rot[1] = vec4(xaxis.y, yaxis.y, zaxis.y, 0);
	rot[2] = vec4(xaxis.z, yaxis.z, zaxis.z, 0);
	rot[3] = vec4(tx, ty, tz, 1);

	return rot;
}
// create perspective matrix
mat4 Transform::perspective(float fovy, float aspect, float near, float far)
{
	float d = 1.0f / tan(DEG2RAD(fovy) / 2.0f);
	float a = -((far + near) / (far - near));
	float b = -(2.0f * far * near) / (far - near);
	mat4 ret(d / aspect, 0, 0, 0,
		0, d, 0, 0,
		0, 0, a, -1,
		0, 0, b, 0);
	return ret;
}
// Set new scale
mat4 Transform::scale(const float &sx, const float &sy, const float &sz)
{
	mat4 ret(
		sx, 0, 0, 0,
		0, sy, 0, 0,
		0, 0, sz, 0,
		0, 0, 0, 1);
	return ret;
}
// Set new translation
mat4 Transform::translate(const float &tx, const float &ty, const float &tz)
{
	mat4 ret(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		tx, ty, tz, 1);
	return ret;
}



