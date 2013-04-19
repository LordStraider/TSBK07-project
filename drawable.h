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

	DrawableObject(GLfloat x, GLfloat yOffset, GLfloat z, GLfloat rotation, GLfloat scale, GLuint* tex, Model* model, GLuint* program, bool shadow = false);

	virtual void draw();

	//overload this to add AI behaviour. return true to remove object from public vector.
	virtual bool update();

	//Rotates an object (rotation += angle). See also: setRotation(GLfloat)
	void rotate(GLfloat angle);
	
	//use 0,1,0 to move only along the y-axis. See also: setCoords()
	void move(GLfloat x, GLfloat y, GLfloat z);

	//SETS rotation (rotation = angle). See also: rotate(GLfloat)
	void setRotation(GLfloat angle);

	//use NULL,0,NULL to set y = 0 while not affecting x or z. See also move()
	void setCoords(GLfloat x, GLfloat y, GLfloat z);
	vec3 getCoords();

protected:
	void updateMatrices();
	void stayInBounds();
	GLfloat x, z, y, yOffset, rotation, scale; //yOffset is distance from ground
	Model* model;
	GLuint* program;
	GLuint* tex;
	bool shadow;
};

class Tree : public DrawableObject{
public:
	Tree(GLfloat x, GLfloat yOffset, GLfloat z, GLfloat rotation, GLfloat scale, GLuint* tex, Model* model, GLuint* program, bool shadow = false) :
		DrawableObject(x, yOffset, z, rotation, scale, tex, model, program, shadow) {};

	//overload this to add AI behaviour. return true to remove object from public vector.
	virtual bool update() override;
};

void drawObj(DrawableObject* obj);