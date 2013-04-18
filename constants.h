#ifndef CONSTANTS_H
#define CONSTANTS_H

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include "MicroGlut.h"
    #import <ApplicationServices/ApplicationServices.h>
#endif

#include "GL_utilities.h"
#include "loadobj.h"
#include "LoadTGA2.h"
#include "VectorUtils3.h"
#include "controller.h"

#ifndef bool
    #define bool int
    #define false ((bool)0)
    #define true  ((bool)1)
#endif

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

extern mat4 rot, trans, shear, total, cam, proj, tmp;

extern TextureData ttex; // terrain

#ifndef near
    #define near 1.0
    #define far 300.0
    #define right 1.0
    #define left -1.0
    #define top 1.0
    #define bottom -1.0
#endif

extern GLfloat camPos, yCamPos, camMod, xModify, xValue, yFind, yModify, yValue, zModify, zValue, teaY, windY;

extern GLuint texWidth, texHeight;
extern GLfloat *vertexArray;
extern GLuint *indexArray;

extern float gravity, angle, angleMod, rotate, speed;
extern bool menuPressed;

extern Point3D p, l;
extern GLuint program, programNoLight, programShadow, programSingleColor, programInvisible, programTerrain;

extern GLuint bunnyTex, dirtTex, cubeTex, skyBoxTex, tex1;

extern Model *bunny, *bunnyShadow, *teapot, *teapotShadow, *cube, *skyBox, *blade, *windmillWalls, *windmillRoof, *windmillBalcony, *terrain;
//extern Model *windmill2;

void init();

#endif
