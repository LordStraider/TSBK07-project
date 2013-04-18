#ifndef DRAW_H
#define DRAW_H

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include "MicroGlut.h"
    #import <ApplicationServices/ApplicationServices.h>
#endif

#include "GL_utilities.h"
#include "LoadTGA2.h"
#include <math.h>
#include <vector>

#include "loadobj.h"
#include "VectorUtils3.h"
#include <stdio.h>

#include "drawable.h"
#include "constants.h"
#include "controller.h"


void displayTexture();
void displaySingleColor(GLfloat t);
void displayInvisible();
void displayModels(GLfloat t);
void displayShadows(GLfloat t);
void displayNoLight(GLfloat t);
void display();

#endif
