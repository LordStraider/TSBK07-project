#pragma once

#include "VectorUtils3.h"
#include "GL_utilities.h"
#include "controller.h"
#include "constants.h"

class DrawableObject {
public:
	mat4 trans;
	mat4 rot;
	mat4 total;
	~DrawableObject();
	
	DrawableObject();

	DrawableObject(GLfloat x, GLfloat yOffset, GLfloat z, GLfloat rotation, GLuint* tex, Model* model, GLuint* program, bool shadow = false);

	DrawableObject(vec3 position, GLfloat rotation, GLuint* tex, Model* model, GLuint* program, bool shadow = false);

	void draw();

	//overload this to add AI behaviour. return true to remove object from public vector.
	bool update();

	//Rotates an object (rotation += angle). See also: setRotation(GLfloat)
	void rotate(GLfloat angle);
	
	//use 0,1,0 to move only along the y-axis. See also: setCoords()
	void move(GLfloat x, GLfloat y, GLfloat z);

	//SETS rotation (rotation = angle). See also: rotate(GLfloat)
	void setRotation(GLfloat angle);

	//use NULL,0,NULL to set y = 0 while not affecting x or z. See also move()
	void setCoords(GLfloat x, GLfloat y, GLfloat z);
	vec3 getCoords();

private:
	void updateMatrices();
	GLfloat x, z, y, yOffset; //yOffset is distance from ground
	GLfloat rotation;
	Model* model;
	GLuint* program;
	GLuint* tex;
	bool shadow;
};


void drawObj(DrawableObject* obj);