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

class Plane {
public:

	vec3 normal,point;
	float d;


	Plane(vec3 v1, vec3 v2, vec3 v3);
	Plane(void) {}
	~Plane() {}

	void set3Points(vec3 v1, vec3 v2, vec3 v3);
	void setNormalAndPoint(vec3 normal, vec3 point);
	void setCoefficients(float a, float b, float c, float d);
	float distance(vec3 p);
};
