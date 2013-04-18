#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include "MicroGlut.h"
    #import <ApplicationServices/ApplicationServices.h>
#endif

#include "GL_utilities.h"
#include "LoadTGA2.h"
#include <math.h>
#include "loadobj.h"
#include "VectorUtils3.h"
#include <stdio.h>
#include "constants.h"
#include "controller.h"

void displayTerrain();
void displaySingleColor(GLfloat t);
void displayInvisible(GLfloat t);
void displayModels(GLfloat t);
void displayShadows(GLfloat t);
void displayNoLight(GLfloat t);
void display();
