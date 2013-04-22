#ifndef CONSTANTS_H
#define CONSTANTS_H

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include "MicroGlut.h"
#endif
#include <vector>
#include <algorithm>
#include <iterator>
#include "GL_utilities.h"
#include "loadobj.h"
#include "LoadTGA2.h"
#include "VectorUtils3.h"
#include "drawable.h"

#ifndef M_PI
    #define M_PI           3.14159265358979323846
#endif

class DrawableObject;

extern mat4 rot, trans, shear, total, cam, proj, tmp;

extern TextureData ttex; // terrain

    #define near 1.0
    #define far 300.0
    #define right 1.0
    #define left -1.0
    #define top 1.0
    #define bottom -1.0

extern GLfloat camPos, yCamPos, camMod, xModify, xValue, yFind, yModify, yValue, zModify, zValue, teaY, windY;
extern GLfloat kingX, kingY, kingZ;

extern GLuint texWidth, texHeight;
extern GLfloat *vertexArray;
extern GLuint *indexArray;

extern float gravity, angle, angleMod, bunnyRotation, speed, kingRotation;
extern bool menuPressed;

extern Point3D p, l;
extern GLuint program, programNoLight, programShadow, programSingleColor, programInvisible, programTerrain;

extern GLuint dirtTex, bunnyTex, skyBoxTex, grassTex;

extern Model *batmobil, *kingKong, *bunny, *bunnyShadow, *teapot, *teapotShadow, *cube, *skyBox, *blade, *windmillWalls, *windmillRoof, *windmillBalcony, *terrain, *sphere, *lowResTree, *highResTree, *billBoard;
//extern Model *windmill2;

extern std::vector<DrawableObject*> allObjects;
extern DrawableObject* bunnyObj;

void init();

#endif
