#pragma once

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include "lib/MicroGlut.h"
#endif

// C++ Standard-library imports
#include <math.h>
#include <stdio.h>

// Third-party module imports
#include "lib/GL_utilities.h"
#include "lib/loadobj.h"
#include "lib/LoadTGA2.h"
#include "lib/VectorUtils3.h"

// Project modules
#include "constants.h"
#include "drawable.h"

class DrawableObject;

Model* GenerateTerrain(TextureData *tex);
bool SameSide(Point3D p1, Point3D p2, Point3D a, Point3D b);
bool PointInTriangle(Point3D p, Point3D a, Point3D b, Point3D c);
GLfloat findY(int x, int z);
void keyController();
bool checkBoundaries(int x, int z);
bool checkBoundaries();
bool checkCollisionBB(DrawableObject* obj1, DrawableObject* obj2);
bool checkCollisionBS(DrawableObject* obj1, DrawableObject* obj2);
bool checkCollisionSS(DrawableObject* obj1, DrawableObject* obj2);
