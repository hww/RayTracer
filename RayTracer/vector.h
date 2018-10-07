/**
 *  @file vector.h
 *  @poject RayTracer for online courses
 *  @brief The vector class represent single point in the 3D world or direction vector
 *  @autor Valery P. (github.com/hww)
 */

#ifndef MyRayTracer_Vector_h
#define MyRayTracer_Vector_h

#include <string>
#include "types.h"
#include "debug.h"

/**
 * Single point in 3D world or direction vector
 *  Members:
 *    float x, y, z
 */
struct Vector {
public:

	/* -- Constructors -- */

	/// Constructor
	Vector(float x_ = 0, float y_ = 0, float z_ = 0) { x = x_; y = y_; z = z_; }
	/// Constructor
	Vector(const vec3 & r) { x = r.x; y = r.y; z = r.z; }
	/// Constructor
	Vector(const vec4 & r) { x = r.x; y = r.y; z = r.z; }

	/* -- binary operations -- */

	/// Summ of vectors and return result
	Vector operator+(const Vector &b) const { return Vector(x + b.x, y + b.y, z + b.z); }
	/// Difference betweeb vectors
	Vector operator-(const Vector &b) const { return Vector(x - b.x, y - b.y, z - b.z); }
	/// Multiply vector by scalar value
	Vector operator*(float b) const { return Vector(x*b, y*b, z*b); }

	/* -- unary mutable -- */

	/// Divide vector by scalar value
	Vector& operator/= (float b) { x /= b, y /= b, z /= b; return *this; }
	/// Divide vector's components by components of other vector
	Vector& operator/= (const Vector& b) { x /= b.x, y /= b.y, z /= b.z; return *this; }
	/// Multiply vector's components by components of other vector
	Vector& operator*= (const Vector& b) { x *= b.x, y *= b.y, z *= b.z; return *this; }
	/// Summ of vectors
	Vector& operator+= (const Vector& b) { x += b.x, y += b.y, z += b.z; return *this; }
	/// Difference betweeb vectors
	Vector& operator-= (const Vector& b) { x -= b.x, y -= b.y, z -= b.z; return *this; }
	/// Normilize vector
	Vector& norm() { return *this = *this * (1 / sqrt(x*x + y * y + z * z)); }

	/* -- unary unmutable -- */

	/// Change vector's direction
	Vector operator- () { return Vector(-x, -y, -z); }
	/// Multiply vector's components by components of other vector
	Vector mult(const Vector &b) const { return Vector(x*b.x, y*b.y, z*b.z); }
	/// Dot product of thwo vectors
	static float dot(const Vector &a, const Vector &b) { return a.x*b.x + a.y*b.y + a.z*b.z; }

	/* -- extra -- */
	
	/// Return magnitude of vector
	float magnitude() const { return sqrt(x*x + y * y + z * z); }
	/// Return magnitude of vector without square root
	float magnitudeSqrt() const { return (x*x + y * y + z * z); }
	/// Return true if vector is zero
	bool isZero() { return x == 0.0 && y == 0.0 && z == 0.0; }

	/* -- casting -- */

	/// Dehomohenize vec4
	Vector dehomogenize(vec4 v) { return Vector(v.x / v.w, v.y / v.w, v.z / v.w); }
	/// Convert to vec3 type
	operator vec3() const { return vec3(x, y, z); }
	/// Convert to vec4 type
	vec4 toVec4(double w) { return vec4(x, y, z, w); }
	/// Convert vector to string form 
	std::string toString() { return format("[%f,%f,%f]", x, y, z); }

	/* -- static members -- */

	/// Normalize this given vector
	static Vector& norm(const Vector& v) { Vector v1 = v; return v1.norm(); }
	/// Cross product of vectors @a and @b
	static Vector cross(const Vector&a, const Vector&b) {
		return Vector(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
	}
	/// Get random vector in given @angle cone
	static Vector random(double angle) {
		float theta = randomInRange(0.0, DEG2RAD((float)angle));
		float phi = randomInRange(0, 2 * PI);
		return vec3(cos(phi)*sin(theta), sin(phi)*sin(theta), cos(theta));
	}
	/// Component of this vector 
	//@{
	float x, y, z;
	//@}
};

#endif
