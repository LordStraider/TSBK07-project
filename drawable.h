#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "VectorUtils3.h"
#include "GL_utilities.h"
#include "controller.h"

class DrawableObject{
public:
	mat4 trans;
	mat4 rot;
	mat4 total;
	~DrawableObject();
	
	DrawableObject(int x, int yOffset, int z, GLfloat rotation, GLuint* tex, Model* model, GLuint* program);

	DrawableObject(vec3 position, GLfloat rotation, GLuint* tex, Model* model, GLuint* program);

	void draw(GLfloat t);

	//Rotates an object (rotation += angle). See also: setRotation(GLfloat)
	void rotate(GLfloat angle);
	
	//use 0,1,0 to move only along the y-axis. See also: setCoords()
	void move(int x, int y, int z);

	//SETS rotation (rotation = angle). See also: rotate(GLfloat)
	void setRotation(GLfloat angle);

	//use NULL,0,NULL to set y = 0 while not affecting x or z. See also move()
	void setCoords(int x, int y, int z);
private:
	void updateMatrices();
	int x, z, yOffset; //yOffset is distance from ground
	GLfloat rotation;
	Model* model;
	GLuint* program;
	GLuint* tex;
};

#endif DRAWABLE_H
