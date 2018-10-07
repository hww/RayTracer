/**
*  @file group_primitives.h
*  @poject RayTracer for online courses
*  @brief Basic material
*  @autor Valery P. (github.com/hww)
*/

#ifndef RayTracer2_group_primitive_h
#define RayTracer2_group_primitive_h

#include <vector>
#include <float.h>
#include "primitive.h"
#include "intersection.h"
#include "matrix.h"
#include "camera.h"
#include "base_primitive.h"
/**
 * GroupPrimitives
 * 
 * Methods:
 * 
 *   GroupPrimitives(vector<Primitive*> list);
 *   
 *   bool intersect(Ray& ray, float* thit, Intersection* in)
 *   
 *   bool intersectP(Ray& ray)
 *   
 *   void getBRDF(LocalGeo& local, BRDF* brdf) {
 *       exit(1);
 *       // This should never get called, because in->primitive will
 *       // never be an aggregate primitive
 *   }
 * 
 * Notes:
 * 
 *   Constructor store the STL vector of pointers to primitives.
 *   Intersect just loops through all the primitives in the list and
 *   call the intersect routine. Compare thit and return that of the nearest
 *   one (because we want the first hit).
 *   
 *   Also, the in->primitive should be set to the pointer to that primitive.
 *   When you implement acceleration structure, it will replace this class.
 */
class GroupPrimitives : public BasePrimitive {
public:
	/// Constructor with collection of primitives
	/// @param primitivesList the collection of primitives
	GroupPrimitives(std::vector<Primitive*> primitivesList) {
		primitives = primitivesList;
	}
    /// Constructor this with collection of primitives. The method will use only faces oriented to the given dirrection
	/// @param primitivesList the collection of primitives
	/// @param lightDir direction of light
    GroupPrimitives(std::vector<Primitive*> primitivesList, const Vector& lightDir)  {
        setPrimitives(primitivesList, lightDir);
	}
	/// Set primitives. The method will use only faces oriented to the given dirrection
	/// @param primitivesList the collection of primitives
	/// @param lightDir direction of light
    void setPrimitives(std::vector<Primitive*> primitivesList, const Vector& lightDir) {
        int size = (int)primitivesList.size();
		for(int i=0; i<size; i++) {
            if (primitivesList[i]->isFront(lightDir)) {
                primitives.push_back(primitivesList[i]);
            }
        }
    }
	/// Test intersection between ray and this primitive
	/// @param ray 
	/// @param cull back face culling
	/// @result true for sucessfull intersection
	virtual bool intersect(Ray& ray, bool cullBack) {
        int size = (int)primitives.size();
		for(int i=0; i<size; i++)
			if (primitives[i]->intersect(ray, cullBack))
				return true;
		return false;
	}
	/// Test intersection between ray and this primitive
	/// @param ray 
	/// @param[in,out] intersection will contain result of intersection
	/// @param cull back face culling
	/// @result true for sucessfull intersection
	virtual bool intersect(Ray& ray, Intersection& intersection, bool cullBack) {
		bool found = false;
        int size = (int)primitives.size();
		for(int i=0; i<size; i++) {
			Intersection pintersect;
			if (primitives[i]->intersect(ray, pintersect, cullBack)) {
				if (pintersect.distance < intersection.distance) {
					intersection = pintersect;
                    found = true;
				}
			}
		}
		return found;
	}
	/// The collection of primitive
	std::vector<Primitive*> primitives;
};

#endif
