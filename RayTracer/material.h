/**
*  @file camera.h
*  @poject RayTracer for online courses
*  @brief Basic material
*  @autor Valery P. (github.com/hww)
*/

#ifndef MyRayTracer_brdf_h
#define MyRayTracer_brdf_h

#include "color.h"

/**
 * BRDF
 *   Storing information enough for shading (it is not the actual BRDF function
 *   in the rendering equation that will be covered later in the semester)
 *
 *   Members:
 *   kd, ks, ka - are diffuse, specular and ambient component respectively
 *   kr - is the mirror reflection coefficient
 *   Color kd, ks, ka, kr
 */
class Material {
public:
    Color diffuse;          //< diffuse color
    Color specular;         //< specular color
    Color ambient;          //< ambient color
    Color reflection;       //< reflection color
    Color emission;         //< emission color
    float shininess;        //< shininess value
    float transparency;     //< transparency value
    float refraction;       //< refraction value

    /// Construct default BRDF data
    Material ()
        : ambient(0.2f,0.2f,0.2f)
        , shininess(20.0f)
        , transparency(1.0f)
        , refraction(1.0f)
    {}

};

#endif
