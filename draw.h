#ifndef DRAW_H
#define DRAW_H

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include "MicroGlut.h"
#endif

#include "GL_utilities.h"
#include "LoadTGA2.h"
#include <math.h>
#include <vector>
#include <math.h>
#include "loadobj.h"
#include "VectorUtils3.h"
#include <stdio.h>
#include <algorithm>
#include <iterator>
#include "drawable.h"
#include "constants.h"

void displayTexture();
void displayTerrain();
// void displaySingleColor(GLfloat t);
void displayInvisible(GLfloat t);
void displayModels(GLfloat t);
void displayShadows(GLfloat t);
void displayNoLight(GLfloat t);
void display();

#endif
