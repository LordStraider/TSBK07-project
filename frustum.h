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
//#include "drawable.h"
//#include "plane.h"
/*
class DrawableObject;
class Plane;

class FrustumG {
public:
	enum {OUTSIDE, INTERSECT, INSIDE};

	Plane pl[6];


	vec3 ntl,ntr,nbl,nbr,ftl,ftr,fbl,fbr;
	float nearD, farD, ratio, angle,tang;
	float nw,nh,fw,fh;

	void setCamInternals(float angle, float ratio, float nearD, float farD);
	void setCamDef(vec3 *p, vec3 *l, vec3 *u);
	int pointInFrustum(vec3 p);
	int sphereInFrustum(vec3 p, float raio);
	int boxInFrustum(DrawableObject *b);

	void drawPoints();
	void drawLines();
	void drawPlanes();
	void drawNormals();
private:
	enum {
		TOP = 0,
		BOTTOM,
		LEFT,
		RIGHT,
		NEARP,
		FARP
	};
};
*/
/*
    #define BOX 0
    #define SPHERE 1
    #define NONE 100
    #define near 1.0
    #define far 300.0
    #define right 1.0
    #define left -1.0
    #define top 1.0
    #define bottom -1.0
*/

bool objectVisible(const vec3 pos, const GLfloat radius);
//bool objectVisible2(const vec3 pos, const GLfloat &radius);
//bool objectVisible(const vec3 pos, const GLfloat &radius);