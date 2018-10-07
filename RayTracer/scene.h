/**
*  @file scene.h
*  @poject RayTracer for online courses
*  @brief The scene to render. This file implemented just for executing Berkeley tasks
*  @autor Valery P. (github.com/hww)
*/

#ifndef RayTracer2_reader_h
#define RayTracer2_reader_h

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include "glut/glut.h"
#include "math.h"
#include <omp.h>

#define NUM_THREADS 4

/**
 * @breif
 * Scene can be defined by text based matalaguage. After completition defining scene
 * the scene can be rendered.
 *
 * Methods:
 *
 * The main rendering loop
 *
 *    void render() {
 *        while (!sampler.generateSample(&sample) {
 *            camera.generateRay(sample, &ray);
 *            raytracer.trace(ray, &color);
 *            film.commit(sample, color);
 *        }
 *        film.writeImage();
 *    }
 */
class Scene {


public:

	/// Construct new scene with default parameters
	Scene() : primMaterial(0)
	{
		output = "scene.png";
		width = 600;
		height = 400;
		// maxdepth depth: The maximum depth (number of bounces) for a ray (default should be 5).
		maxdepth = 5;
		// amount of global illumination rays
		globIllumination = 0;
		// antialise level
		antialias = 1;
		// camera
		fovy = 90.0;
		eyeinit = Vector(0.0, 0.0, 5.0); // Initial eye position, also for resets
		upinit = Vector(0.0, 1.0, 0.0);  // Initial up position, also for resets
		center = Vector(0.0, 0.0, 0.0);  // Center look at point
		// attenuation const linear quadratic Sets the constant, linear
		// and quadratic attenuations (default 1,0,0) as in OpenGL.
		// By default there is no attenuation (the constant term is 1,
		// linear and quadratic are 0; that's what we mean by 1,0,0).
		attenuationConst = 1.0;
		attenuationLinear = 0.0;
		attenuationQuadratic = 0.0;
	}

	/// Render scene to file
	void render() {
		omp_set_num_threads(NUM_THREADS);
		time_t startTime = time(NULL);
		srand((unsigned)time(0));

		Sampler sampler = Sampler(width, height, antialias);
		Picture finalPicture(width, height);
		Camera cam(fovy, (float)width / (float)height, 0.1, 1000);
		cam.lookAt(eyeinit, center, upinit);
		RayTracer raytracer(cam, primitiveList, lightList, maxdepth, globIllumination);

		cout << endl << endl;
		printf("Raytracing... (%dx-antialias, %d-maxdepth, %d-global-illumination)\n", antialias, maxdepth, globIllumination);

		int counter, opercent;
		opercent = counter = 0;
		int numSamples = antialias * antialias;
#pragma omp parallel for // OpenMP
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				Color* color = finalPicture.getPixef(x, y);
				for (int a = 0; a < numSamples; a++)
				{
					Vector newsample = sampler.getGridSample(a, x, y);
					Ray ray = cam.GenerateRay(newsample);
					raytracer.trace(ray, 1, color);
				}
			}
			int percent = ++counter * 20 / height;
			if (opercent != percent) {
				printf("\n%d%% completed (%ds elapsed) ", percent * 5, (int)(time(NULL) - startTime));
				opercent = percent;
			}
			else {
				printf(".");
			}
		}
		// Save file
		cout << endl << "Now Saving " << output << "..." << endl;
		finalPicture.writeImage(output.c_str(), numSamples);

		// Timing report
		time_t elapsed = time(NULL) - startTime;
		int  elapsedTime = (int)elapsed;
		printf("\nTime Elapsed: %ds | %dm %ds\n", elapsedTime, elapsedTime / 60, elapsedTime % 60);
	}

	/// The function below applies the appropriate transform to a 4-vector
	void matransform(std::stack<mat4> &transfstack, GLfloat* values)
	{
		mat4 transform = transfstack.top();
		vec4 valvec = vec4(values[0], values[1], values[2], values[3]);
		vec4 newval = valvec * transform;
		for (int i = 0; i < 4; i++) values[i] = newval[i];
	}

	/// Right matrix multiplication 
	void rightmultiply(const mat4 & M, std::stack<mat4> &transfstack)
	{
		mat4 &T = transfstack.top();
		T = T * M;
	}

	/// Function to read the input data values
	/// Use is optional, but should be very helpful in parsing.
	bool readvals(stringstream &s, const int numvals, GLfloat* values)
	{
		for (int i = 0; i < numvals; i++) {
			s >> values[i];
			if (s.fail()) {
				cout << "Failed reading value " << i << " will skip\n";
				return false;
			}
		}
		return true;
	}

	/// Check if primitive material is null then make it
	Material* GetPrimitiveMaterial() {
		if (primMaterial == NULL) {
			primMaterial = new Material();
			primMaterial->diffuse = materialState.diffuse;
			primMaterial->specular = materialState.specular;
			primMaterial->ambient = materialState.ambient;
			primMaterial->shininess = materialState.shininess;
			primMaterial->emission = materialState.emission;
			primMaterial->transparency = materialState.transparency;
			primMaterial->refraction = materialState.refraction;
		}
		return primMaterial;
	}

	/// Read source file 
	void readfile(string filename) {
		int linenum = 0;
		string str, cmd;
		ifstream in;
		in.open(filename.c_str());
		if (in.is_open()) {
			// I need to implement a matrix stack to store transforms.
			// This is done using standard STL Templates
			stack <mat4> transfstack;
			transfstack.push(mat4(1.0));  // identity
			primMaterial = NULL; // Not yet material definded

			getline(in, str); linenum++;
			while (in) {

				if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
					// Ruled out comment and blank lines
					stringstream s(str);
					s >> cmd;
					// Position and color for light, colors for others
					// Up to 10 params for cameras.
					float values[10];	
					bool validinput; // Validity of input
					cout << cmd << "\n";

					if (cmd[0] == '#')
					{
						// # comments This is a line of comments. Ignore any line starting with a #.
						continue;
					}
					else if (cmd == "output") {
						// output filename: The output file to which the image should be written.
						// You can either require this to be specified in the input, or you can
						// use a suitable default like stdout or raytrace.png (The final files
						// should be .png, with the specific names the autograder requests).
						// getline(inFile,line);
						s >> output;
					}
					else if (cmd == "size") {
						// size width height: The size command must be the first command of the
						// file, which controls the image size.
						validinput = readvals(s, 2, values);
						if (validinput) {
							width = (int)values[0]; height = (int)values[1];
						}
					}
					else if (cmd == "maxdepth") {
						// maxdepth depth: The maximum depth (number of bounces) for a ray
						// (default should be 5).
						validinput = readvals(s, 1, values);
						if (validinput) {
							maxdepth = values[0];
						}
					}
					else if (cmd == "antialias") {
						// maxdepth depth: The maximum depth (number of bounces) for a ray
						// (default should be 5).
						validinput = readvals(s, 1, values);
						if (validinput) {
							antialias = values[0];
						}
					}
					else if (cmd == "illumination") {
						// illumination rays: Amount of global ilumination rays
						// (default should be 0).
						validinput = readvals(s, 1, values);
						if (validinput) {
							globIllumination = values[0];
						}
					}
					else if (cmd == "directional") {
						// Process the light, add it to database.
						// Lighting Command
						// directional x y z r g b
						// The direction to the light source, and the color, as in OpenGL.
						validinput = readvals(s, 6, values);
						if (validinput) {
							Light* light = new Light(values[0], values[1], values[2],
								values[3], values[4], values[5], false);
							light->attenuationConst = attenuationConst;
							light->attenuationLinear = attenuationLinear;
							light->attenuationQuadratic = attenuationQuadratic;
							lightList.push_back(light);
						}
					}
					else if (cmd == "point") {
						// point x y z r g b
						// The location of a point source and the color, as in OpenGL.
						validinput = readvals(s, 6, values);
						if (validinput) {
							Light* light = new Light(values[0], values[1], values[2],
								values[3], values[4], values[5], true);
							light->attenuationConst = attenuationConst;
							light->attenuationLinear = attenuationLinear;
							light->attenuationQuadratic = attenuationQuadratic;
							lightList.push_back(light);
						}
					}
					else if (cmd == "attenuation") {
						// attenuation const linear quadratic
						// Sets the constant, linear
						// and quadratic attenuations (default 1,0,0) as in OpenGL. By
						// default there is no attenuation (the constant term is 1,
						// linear and quadratic are 0; that's what we mean by 1,0,0).
						validinput = readvals(s, 3, values);
						if (validinput) {
							attenuationConst = values[0];
							attenuationLinear = values[1];
							attenuationQuadratic = values[2];
						}
					}
					else if (cmd == "ambient") {
						// ambient r g b
						// The global ambient color to be added for each object (default is .2,.2,.2).
						validinput = readvals(s,3,values);
						if (validinput) {
							materialState.ambient.r = values[0];
							materialState.ambient.g = values[1];
							materialState.ambient.b = values[2];
						}
					}
					else if (cmd == "ambient") {
						// Material: ambient r g b
						// The global ambient color to be added for each object (default is .2,.2,.2).
						validinput = readvals(s, 3, values); // colors
						if (validinput) {
							materialState.ambient = Color(values[0], values[1], values[2]);
						}
						primMaterial = NULL; // Forget old material
					}
					else if (cmd == "diffuse") {
						// Material: diffuse r g b 
						validinput = readvals(s, 3, values);
						if (validinput) {
							materialState.diffuse = Color(values[0], values[1], values[2]);
						}
						primMaterial = NULL; // Forget old material
					}
					else if (cmd == "specular") {
						// Material: specular r g b 
						validinput = readvals(s, 3, values);
						if (validinput) {
							materialState.specular = Color(values[0], values[1], values[2]);
						}
						primMaterial = NULL; // Forget old material
					}
					else if (cmd == "emission") {
						// Material: emission r g b 
						validinput = readvals(s, 3, values);
						if (validinput) {
							materialState.emission = Color(values[0], values[1], values[2]);
						}
						primMaterial = NULL; // Forget old material
					}
					else if (cmd == "shininess") {
						// Material: shininess value
						validinput = readvals(s, 1, values);
						if (validinput) {
							materialState.shininess = values[0];
						}
						primMaterial = NULL; // Forget old material
					}
					else if (cmd == "transparency") {
						// Material: transparency value
						validinput = readvals(s, 1, values);
						if (validinput) {
							materialState.transparency = values[0];
						}
						primMaterial = NULL; // Forget old material
					}
					else if (cmd == "refraction") {
						// Material: refraction value
						validinput = readvals(s, 1, values);
						if (validinput) {
							materialState.refraction = values[0];
						}
						primMaterial = NULL; // Forget old material
					}
					else if (cmd == "camera") {
						// camera lookfromx lookfromy lookfromz lookatx lookaty lookatz upx upy upz fov
						// specifies the camera in the standard way, as in homework 2.
						// Note that fov stands for the field of view in the y direction.
						// The field of view in the x direction will be determined by the image
						// size. The world aspect ratio (distinct from the width and height that
						// determine image aspect ratio) is always 1; a sphere at the center of
						// a screen will look like a circle, not an ellipse, independent of the
						// image aspect ratio. This is a common convention but different
						// from some previous specifications like the raytracing
						// journal handout.
						validinput = readvals(s, 10, values); // 10 values eye cen up fov
						if (validinput) {
							eyeinit.x = values[0];
							eyeinit.y = values[1];
							eyeinit.z = values[2];
							center.x = values[3];
							center.y = values[4];
							center.z = values[5];
							upinit.x = values[6];
							upinit.y = values[7];
							upinit.z = values[8];
							fovy = values[9];
						}
					}
					else if (cmd == "sphere") {
						// sphere x y z radius
						// Defines a sphere with a given position and radius.
						validinput = readvals(s, 4, values);
						if (validinput) {
							Sphere* sphere = new Sphere(Vector(values[0], values[1], values[2]), values[3]);
							Primitive *prim = new Primitive(sphere);
							prim->material = GetPrimitiveMaterial();
							prim->setTransform(transfstack.top());
							prim->sourceLine = linenum;
							primitiveList.push_back(prim);
						}
					}
					else if (cmd == "tri") {
						// tri v1 v2 v3
						// Create a triangle out of the vertices involved (which have
						// previously been specified with the vertex command). The
						// vertices are assumed to be specified in counter-clockwise order.
						// Your code should internally compute a face normal for this triangle.
						validinput = readvals(s, 3, values);
						if (validinput) {
							int ia = values[0];
							int ib = values[1];
							int ic = values[2];
							Triangle* tri = new Triangle(vertices[ia], vertices[ib], vertices[ic]);
							Primitive* prim = new Primitive(tri);
							prim->material = GetPrimitiveMaterial();
							prim->setTransform(transfstack.top(), true);
							prim->sourceLine = linenum;
							primitiveList.push_back(prim);
						}
					}
					else if (cmd == "trinormal") {
						// trinormal v1 v2 v3
						// Same as above but for vertices specified with normals. In this case,
						// each vertex has an associated normal, and when doing shading, you
						// should interpolate the normals for intermediate points on the triangle.
						validinput = readvals(s, 1, values);
						if (validinput) {
							int ia = values[0];
							int ib = values[1];
							int ic = values[2];
							Triangle* tri = new Triangle(vertices[ia], vertices[ib], vertices[ic],
								normals[ia], normals[ib], normals[ic]);
							Primitive* prim = new Primitive(tri);
							prim->material = GetPrimitiveMaterial();
							prim->setTransform(transfstack.top());
							prim->sourceLine = linenum;
							primitiveList.push_back(prim);
						}
					}
					else if (cmd == "maxverts") {
						// maxverts number Defines a maximum number of vertices for later triangle
						// specifications. It must be set before vertices are defined.
						// (Your program may not need this; it is simply a convenience to
						// allocate arrays accordingly. You can ignore this command
						// [but still parse it] if you don't need it).
						validinput = readvals(s, 1, values);
						if (validinput) {
							maxverts = values[0];
							vertices.clear();
							normals.clear();
						}
					}
					else if (cmd == "maxvertnorms") {
						// maxvertnorms number
						// Defines a maximum number of vertices with normals for later
						// specifications. It must be set before vertices with normals
						// are defined. (same discussion as above)
						validinput = readvals(s, 1, values);
						if (validinput) {
							maxvertnorms = values[0];
							vertices.clear();
							normals.clear();
						}
					}
					else if (cmd == "vertex") {
						// vertex x y z
						// Defines a vertex at the given location. The vertex is put into
						// a pile, starting to be numbered at 0.
						validinput = readvals(s, 3, values);
						if (validinput) {
							vertices.push_back(vec3(values[0], values[1], values[2]));
						}
					}
					else if (cmd == "vertexnormal") {
						// vertexnormal x y z nx ny nz
						// Similar to the above, but define a surface normal with each
						// vertex. The vertex and vertexnormal set of vertices are
						// completely independent (as are maxverts and maxvertnorms).
						validinput = readvals(s, 6, values);
						if (validinput) {
							vertices.push_back(vec3(values[0], values[1], values[2]));
							normals.push_back(vec3(values[3], values[4], values[5]));
						}
					}
					else if (cmd == "translate") {
						validinput = readvals(s, 3, values);
						if (validinput) {
							// Think about how the transformation stack is affected
							// You might want to use helper functions on top of file.
							// Also keep in mind what order your matrix is!
							rightmultiply(Transform::translate(values[0], values[1], values[2]), transfstack);
						}
					}
					else if (cmd == "scale") {
						validinput = readvals(s, 3, values);
						if (validinput) {
							// Think about how the transformation stack is affected
							// You might want to use helper functions on top of file.
							// Also keep in mind what order your matrix is!
							rightmultiply(Transform::scale(values[0], values[1], values[2]), transfstack);
						}
					}
					else if (cmd == "rotate") {
						validinput = readvals(s, 4, values);
						if (validinput) {
							// values[0..2] are the axis, values[3] is the angle.
							// You may want to normalize the axis (or in Transform::rotate)
							// See how the stack is affected, as above.
							// Note that rotate returns a mat3.
							// Also keep in mind what order your matrix is!
							Vector axis(values[0], values[1], values[2]);
							float angle = values[3];
							if (!axis.isZero()) {
								// only if axes defined
								mat4 rotateMat(Transform::rotate(axis, angle));
								rightmultiply(rotateMat, transfstack);
							}
							else
								cerr << "Rotation have wrong axes\n";

						}
					}
					// -- Include the basic push/pop code for matrix stacks
					else if (cmd == "pushTransform") {
						transfstack.push(transfstack.top());
					}
					else if (cmd == "popTransform") {
						if (transfstack.size() <= 1) {
							cerr << "Stack has no elements.  Cannot Pop\n";
						}
						else {
							transfstack.pop();
						}
					}

					else {
						cerr << "Unknown Command: " << cmd << " Skipping \n";
					}

				}
				getline(in, str); linenum++;
			}
		}
		else {
			cerr << "Unable to Open Input Data File " << filename << "\n";
			throw 2;
		}
	}

	/// The output file to which the image should be written. You can either require this
	/// to be specified in the input, or you can use a suitable default like stdout or
	/// raytrace.png (The final files should be .png, with the specific names
	/// the autograder requests).
	std::string output; // output filename

	/// The size command must be the first command of the file, which controls the image size.
	//@{
	int width;
	int height;
	//@}

	/// The maximum depth (number of bounces) for a ray (default should be 5).
	int maxdepth;
	/// how many global illumination rays
	int globIllumination;
	/// Antialis level
	int antialias;
	/// Materials (read from file)
	//@{
	/// With multiple objects, these are colors for each.
	Material materialState;
	/// This is material
	Material *primMaterial;
	//@}

	// Camera
	//@{
	Vector eyeinit; //< Initial eye position, also for resets
	Vector upinit;  //< Initial up position, also for resets
	Vector center;  //< Center look at point
	float fovy;     //< For field of view
	//@}

	/// Primitives
	std::vector<Primitive*> primitiveList;
	// Lights
	std::vector<Light*> lightList;
	/// attenuation const linear quadratic Sets the constant, linear
	/// and quadratic attenuations (default 1,0,0) as in OpenGL.
	/// By default there is no attenuation (the constant term is 1,
	/// linear and quadratic are 0; that's what we mean by 1,0,0).
	float attenuationConst;
	float attenuationLinear;
	float attenuationQuadratic;
	/// Defines a maximum number of vertices for later triangle specifications.
	/// It must be set before vertices are defined. (Your program may not need this;
	/// it is simply a convenience to allocate arrays accordingly. You can ignore
	/// this command [but still parse it] if you don't need it).
	int maxverts;
	/// Defines a maximum number of vertices with normals for later specifications.
	/// It must be set before vertices with normals are defined. (same discussion as above)
	int maxvertnorms;
	/// Array of vertices
	std::vector<Vector> vertices;
	/// Array of normals
	std::vector<Vector> normals;

};

#endif
