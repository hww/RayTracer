/**
*  @file color.h
*  @poject RayTracer for online courses
*  @brief The color data container
*  @autor Valery P. (github.com/hww)
*/

#ifndef MyRayTracer_color_h
#define MyRayTracer_color_h

#include "vector.h"

/**
 * The Color class. Contains r,g,b components only.
 */
class Color {
public:
	/// Construct default, black color
    Color() {
        r = 0;
        g = 0;
        b = 0;
    }
	/// Construct color with given componen's values
	Color(float ri, float gi, float bi) {
        r = ri;
        g = gi;
        b = bi;
    }
	/// Construct color from the vector value
    Color(const Vector& v) {
        r = v.x;
        g = v.y;
        b = v.z;
    }
	
	/* -- binary operations -- */

	/// Set color 
	/// @param ri component
	/// @param gi component
	/// @param bi component
	void Set(float ri, float gi, float bi)
	{
		r = ri;
		g = gi;
		b = bi;
	}
	/// Magnitude the color 
	/// @param c scalar value 
    Color operator* (float c) const {
        return Color(r * c, g * c, b * c);
    }
	/// Increase brigness of this color (aka ambient light)
	/// @param c scalar value 
	Color operator+ (float c) const {
        return Color(r + c, g + c, b + c);
    }
	/// Decrease brigness of this color (aka ambient light)
	/// @param c scalar value 
	Color operator- (float c) const {
        return Color(r - c, g - c, b - c);
    }
	/// Multiply two colors
	Color operator* (const Color& c) const {
        return Color(r * c.r, g * c.g, b * c.b);
    }
	/// Summ of two colors
	Color operator+ (const Color& c) const {
        return Color(r + c.r, g + c.g, b + c.b);
    }
	/// Difference of two colors
	Color operator- (const Color& c) const {
        return Color(r - c.r, g - c.g, b - c.b);
    }
	
	/* -- unary mutable -- */

	/// Multiply two colors
	Color& operator*= (const Color& c) {
        r *= c.r, g *= c.g, b *= c.b;
        return *this;
    }
	/// Summ of two colors
	Color& operator+= (const Color& c) {
        r += c.r, g += c.g, b += c.b;
        return *this;
    }
	/// Difference of two colors
	Color& operator-= (const Color& c) {
        r -= c.r, g -= c.g, b -= c.b;
        return *this;
    }
	/// Multiply color by scalar value
    Color& operator*= (float c) {
        r *= c, g *= c, b *= c;
        return *this;
    }
	/// @result true if this color fully black
    bool isZero() {
        return r < 0.0001 && g < 0.0001 && b < 0.0001;
    }

	/// The color component
	///@{
	float r, g, b;
	///@}
};


#endif
