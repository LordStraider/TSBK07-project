#ifndef DRAW_H
#define DRAW_H

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include "lib/MicroGlut.h"
#endif

// C++ Standard-library imports
#include <math.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <iterator>

// Third-party module imports
#include "lib/GL_utilities.h"
#include "lib/loadobj.h"
#include "lib/LoadTGA2.h"
#include "lib/VectorUtils3.h"

// Project modules
#include "constants.h"
#include "drawable.h"

void displayTexture();
void displayTerrain();
// void displaySingleColor(GLfloat t);
void displayInvisible(GLfloat t);
void displayModels(GLfloat t);
void displayShadows(GLfloat t);
void displayNoLight(GLfloat t);
void display();

#endif
