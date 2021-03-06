/**
*  @file main.h
*  @poject RayTracer for online courses
*  @brief The light source
*  @autor Valery P. (github.com/hww)
*/

#include <iostream>
#include <float.h>
#include <time.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <stdlib.h>
#include "main.h"
#include "scene.h"

// Main Method
int main(int argc, const char * argv[])
{
    string filePath = "";
    if (argc >= 2)
        filePath = argv[1];
    else
        filePath = "../RayTracer/TestFiles/scene6-alt.test";
    Scene scene;
    scene.output = "./Scene.png";
    scene.readfile(filePath);
    scene.render();
    system("pause");
    return 0;
}