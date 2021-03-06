#pragma once

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include "lib/MicroGlut.h"
#endif

// C++ Standard-library imports
#include <vector>
#include <algorithm>
#include <iterator>

// Third-party module imports
#include "lib/GL_utilities.h"
#include "lib/loadobj.h"
#include "lib/LoadTGA2.h"
#include "lib/VectorUtils3.h"

// Project modules
#include "drawable.h"
//#include "frustum.h"


#ifndef M_PI
    #define M_PI           3.14159265358979323846
#endif
//Forward declarations
class DrawableObject;
class Player;
class LightSource;
class FrustumG;

extern mat4 rot, trans, shear, total, cam, proj, tmp;

extern TextureData ttex; // terrain

    #define BOX 0
    #define SPHERE 1
    #define NONE 100
    #define near 1.0
    #define far 200.0
    #define right 1.0
    #define left -1.0
    #define top 1.0
    #define bottom -1.0
    #define frustumRadius 95 // far / 2 + a bit offset

extern GLfloat camPos, yCamPos, camMod, camModY, xModify, xValue, yFind, yModify, yValue, zModify, zValue, teaY, windY;
extern GLfloat kingX, kingY, kingZ;

extern GLuint texWidth, texHeight;
extern GLfloat *vertexArray;
extern GLuint *indexArray;

extern float gravity, angle, angleMod, bunnyRotation, speed, kingRotation;
extern bool menuPressed, gameOver;

extern Point3D p, l;
extern GLuint program, programNoLight, programShadow, programSingleColor, programInvisible, programTerrain;

extern GLuint dirtTex, bunnyTex, skyBoxTex, grassTex;

extern int direction;

extern Model *kingKong, *bunny, *bunnyShadow, *teapot, *teapotShadow, *cube, *skyBox, *blade, *windmillWalls, *windmillRoof, *windmillBalcony, *terrain, *sphere, *lowResTree, *highResTree, *billBoard;
//extern Model *windmill2;

extern FrustumG* frustumG;

extern std::vector<GLuint*> programs;
extern std::vector<DrawableObject*> allObjects;

extern std::vector<vec3> lightDirections;
extern std::vector<vec3> lightColors; 
extern std::vector<vec3> lightPositions; 

extern Player* bunnyObj;

void init_constants();
