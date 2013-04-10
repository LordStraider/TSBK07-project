#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include "MicroGlut.h"
#endif
#import <ApplicationServices/ApplicationServices.h>
#include "GL_utilities.h"
#include "loadobj.h"
#include "VectorUtils3.h"



#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

extern mat4 rot, trans, shear, total, cam;

#define near 1.0
#define far 300.0
#define right 1.0
#define left -1.0
#define top 1.0
#define bottom -1.0

extern GLfloat projMatrix[];
extern GLfloat camPos;
extern GLfloat yCamPos;
extern GLfloat camMod;
extern GLfloat xModify;
extern GLfloat xValue;
extern GLfloat yModify;
extern GLfloat yValue;
extern GLfloat zModify;
extern GLfloat zValue;

extern float gravity;
extern float angle;
extern float angleMod;
extern float rotate;
extern float speed;
extern bool menuPressed;

extern Point3D p,l, v;
extern GLuint program;
extern GLuint programNoLight;
extern GLuint programShadow;
extern GLuint programSingleColor;
extern GLuint programInvisible;

extern GLuint bunnyTex;
extern GLuint dirtTex;
extern GLuint cubeTex;
extern GLuint skyBoxTex;

extern Model *bunny;
extern Model *bunnyShadow;
extern Model *teapot;
extern Model *teapotShadow;
extern Model *cube;
extern Model *skyBox;
extern Model *blade;
//extern Model *windmill2;
extern Model *windmillWalls;
extern Model *windmillRoof;
extern Model *windmillBalcony;

void init();