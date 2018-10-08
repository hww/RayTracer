/**
*  @file raytrace.h
*  @poject RayTracer for online courses
*  @brief The raytracer class
*  @autor Valery P. (github.com/hww)
*/

#ifndef RayTracer2_raytracer_h
#define RayTracer2_raytracer_h

#include <vector>
#include <cmath>
#include "group_primitives.h"
#include "light.h"
#include "vector.h"
#include "ray.h"

/**
 * RayTracer
 *
 * Methods:
 *    void trace(Ray& ray, int depth)
 *
 * Notes:
 *
 *    Shading is similar to hw2
 *    Beware when you generate reflection ray, make sure the ray don’t start
 *    exactly on the surface, or the intersection routine may return
 *    intersection point at the starting point of the ray. (This apply to light
 *    ray generation as well)
 */
class RayTracer {
public:
    /// Constructor
    RayTracer(const Camera& camera,
        std::vector<Primitive*> primitives,
        std::vector<Light*> lights,
        int maxdepth,
        int illumination) :
        maxdepth(maxdepth),
        allPrimitives(primitives),
        camPrimitives(primitives, camera.dir),
        lights(lights),
        globalIllumination(illumination)
    {

    }
    /// Trace ray
    void trace(Ray& ray, int depth, Color* color) {

        assert(depth > 0);

        bool collide;
        Intersection intersect;
        if (depth == 1)
            collide = camPrimitives.intersect(ray, intersect, false);
        else
            collide = allPrimitives.intersect(ray, intersect, true);

        if (!collide)
            return;

        Material* material = intersect.primitive->getMaterial(intersect.surfacePoint);

        // Apply ambient and emission
        *color = *color + material->ambient + material->emission;

        // position of painted point
        Vector mypos = intersect.surfacePoint.position;

        // yes it can reach the target
        Vector normal = intersect.surfacePoint.normal;

        // direction to the eye (eyepoint - pixelpoint)
        Vector eyedirn = Vector::norm(ray.pos - mypos);

        Color lighjtcolor;
        Color lightfilter(1, 1, 1);
        float lightpower = 1;
        int numLights = (int)lights.size();
        for (int i = 0; i < numLights; i++) {

            // drop ray from light position to the destination point
            Intersection lightIntersect;
            Ray lightRay = lights[i]->getRay(intersect.surfacePoint);
            // check if this ray could rich the target
            if (Vector::dot(lightRay.dir, normal) < 0) {
                // if light direction in other direction then
                // we ignore this light
                continue;
            }
            else {
                while (lightpower != 0) {
                    if (allPrimitives.intersect(lightRay, lightIntersect, false)) {
                        Material* oclmaterial = lightIntersect.primitive->getMaterial(lightIntersect.surfacePoint);
                        // someone block the object
                        if (oclmaterial->transparency == 1) {
                            // totally blocked
                            lightpower = 0;
                            break;
                        }
                        else {
                            // slightly blocked
                            lightfilter *= oclmaterial->diffuse;
                            lightpower *= oclmaterial->transparency;
                        }
                    }
                    else break;
                }
            }
            if (lightpower != 0) {
                Color lightColor = lights[i]->getColor(lightRay);
                Vector ldir = lightRay.dir;
                Vector halfv = Vector::norm(ldir + eyedirn);
                Color lcol = computeLight(ldir, lightColor, normal, halfv, material);
                if (lightpower == 1)
                    lighjtcolor += lcol;
                else
                    lighjtcolor += ((lcol * lightfilter) * lightpower);
            }
        }

        *color += (lighjtcolor * material->transparency);

        if (true && material->transparency < 1) {
            // some transparency here
            if (depth < maxdepth)
            {
                Ray tray = ray.refract(intersect.surfacePoint, material->refraction);
                tray.pos = intersect.surfacePoint.position;
                Color tcolor = Color();
                trace(tray, depth + 1, &tcolor);
                *color += (tcolor * (1 - material->transparency));
            }
        }

        if (!material->specular.isZero()) {
            // reflection
            if (depth < maxdepth)
            {
                Ray refray = ray.reflect(intersect.surfacePoint);
                Color refcolor = Color();
                trace(refray, depth + 1, &refcolor);
                *color += (refcolor * material->specular);
            }
        }

        if (globalIllumination > 0 && depth < maxdepth) {
            // global illumination
            Color icolor(0, 0, 0);
            for (int i = 0; i < globalIllumination; i++) {
                Ray iray = ray.random(intersect.surfacePoint, 60);
                trace(iray, 100, &icolor); // just single times
            }
            *color += ((icolor * material->diffuse) * (1.0f / globalIllumination));
        }
    }

    int maxdepth;                       //< Max depth of the ray
    int globalIllumination;             //< Quantity of global illumination rays
    GroupPrimitives allPrimitives;      //< All primitives
    GroupPrimitives camPrimitives;      //< Visible by camera primitives
    std::vector<Light*> lights;         //< All light sources

private:
    /// Main light computation function
    /// @param direction Light direction
    /// @param lightcolor Light color
    /// @param normal Surface normal
    /// @param halfvec The half vector used for lighting equation
    /// @param material Surface material
    inline Color computeLight(const Vector& direction,
        const Color& lightcolor,
        const Vector& normal,
        const Vector& halfvec,
        const Material* material)
    {
        float nDotL = Vector::dot(normal, direction);
        Color diffuse = material->diffuse * lightcolor * max(nDotL, 0.0f);

        float nDotH = Vector::dot(normal, halfvec);
        Color specular = material->specular * lightcolor * pow(max(nDotH, 0.0f), material->shininess);

        Color color = diffuse + specular;
        return color;
    }

};

#endif
