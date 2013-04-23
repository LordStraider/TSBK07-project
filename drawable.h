#pragma once

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include "lib/MicroGlut.h"
#endif

// C++ Standard-library imports
#include <algorithm>
#include <math.h>

// Third-party module imports
#include "lib/GL_utilities.h"
#include "lib/VectorUtils3.h"

// Project modules
#include "controller.h"
#include "constants.h"

class LightSource {
public:
	vec3 position, direction, color;
//	~LightSource();
	LightSource();
	LightSource(vec3 position, vec3 direction, vec3 color);
};

class DrawableObject {
public:
	mat4 trans;
	mat4 rot;
	mat4 total;

//	~DrawableObject();

	DrawableObject();

	DrawableObject(GLfloat x, GLfloat yOffset, GLfloat z, GLfloat rotation, GLuint* tex, Model* model, GLuint* program, vec3 dimensions, int collisionMode, bool shadow = false);

	DrawableObject(vec3 position, GLfloat rotation, GLuint* tex, Model* model, GLuint* program, vec3 dimensions, int collisionMode, bool shadow = false);

	DrawableObject(GLfloat x, GLfloat yOffset, GLfloat z, GLfloat rotation, GLfloat scale, GLuint* tex, Model* model, GLuint* program, vec3 dimensions, int collisionMode, bool shadow = false);

	virtual void draw();

	//overload this to add AI behaviour. return true to remove object from public vector.
	virtual bool update();

	//Rotates an object (rotation += angle). See also: setRotation(GLfloat)
	virtual void rotate(GLfloat angle);

	//use 0,1,0 to move only along the y-axis. See also: setCoords()
	virtual void move(GLfloat x, GLfloat y, GLfloat z);

	//SETS rotation (rotation = angle). See also: rotate(GLfloat)
	virtual void setRotation(GLfloat angle);

	//use NULL,0,NULL to set y = 0 while not affecting x or z. See also move()
	virtual void setCoords(GLfloat x, GLfloat y, GLfloat z);
	virtual vec3 getCoords() const { return vec3(this->x, this->y, this->z); }
	virtual vec3 getDimensons() const { return dimensions; }
	virtual GLfloat getYoffset() { return yOffset; }
	virtual int getCollisionMode() const { return collisionMode; }
	virtual bool getDel() const { return del; }
	void setDel(const bool b) { del = b; }

	virtual void collisionHandler(DrawableObject* obj);

	//being lazy
	bool affectedByGravity;
	GLfloat rotation;
protected:
	void updateMatrices();
	void stayInBounds();
	GLfloat x, z, y, yOffset, scale; //yOffset is distance from ground
	vec3 dimensions;
	Model* model;
	GLuint* program;
	GLuint* tex;
	int collisionMode;
	bool shadow;
	bool del;
};

class Tree : public DrawableObject{
public:
	Tree(GLfloat x, GLfloat yOffset, GLfloat z, GLfloat rotation, GLfloat scale,
         GLuint* tex, Model* model, GLuint* program, vec3 dimensions, int collisionMode, bool shadow = false) :
		DrawableObject(x, yOffset, z, rotation, scale, tex, model, program, dimensions, collisionMode, shadow) {};

	//overload this to add AI behaviour. return true to remove object from public vector.
	virtual bool update();
	virtual void collisionHandler(DrawableObject* obj);
	std::vector<DrawableObject*> apples;
};

class Blade : public DrawableObject{
public:
	Blade(GLfloat x, GLfloat yOffset, GLfloat z, GLfloat rotation, GLfloat scale,
         GLuint* tex, Model* model, GLuint* program, vec3 dimensions, int collisionMode, bool shadow = false) :
		DrawableObject(x, yOffset, z, rotation, scale, tex, model, program, dimensions, collisionMode, shadow) {};

	//overload this to add AI behaviour. return true to remove object from public vector.
	virtual bool update();
	virtual void collisionHandler(DrawableObject* obj);
};

class Enemy : public DrawableObject{
public:
	Enemy(GLfloat x, GLfloat yOffset, GLfloat z, GLfloat rotation, GLfloat scale,
          GLuint* tex, Model* model, GLuint* program, vec3 dimensions, int collisionMode, bool shadow = false) :
		DrawableObject(x, yOffset, z, rotation, scale, tex, model, program, dimensions, collisionMode, shadow) {};

	//overload this to add AI behaviour. return true to remove object from public vector.
	virtual bool update();
	virtual void collisionHandler(DrawableObject* obj);
	GLuint* getProgram() { return program; }
};

class Player : public DrawableObject{
public:
	Player(GLfloat x, GLfloat yOffset, GLfloat z, GLfloat rotation, GLfloat scale,
          GLuint* tex, Model* model, GLuint* program, vec3 dimensions, int collisionMode, bool shadow = false) :
		DrawableObject(x, yOffset, z, rotation, scale, tex, model, program, dimensions, collisionMode, shadow), ammo(10), score(0) {};

	//overload this to add AI behaviour. return true to remove object from public vector.
	virtual bool update();
	virtual void collisionHandler(DrawableObject* obj);
	void fireBulletIfAmmo();
	void addAmmo() { ammo++; }
	int getAmmo() { return ammo; }
	void subAmmo() { --ammo; }
	void addScore() { score++; }
	int getScore() { return score; }

private:
	int score;
	int ammo;
};

class Shot : public DrawableObject{
public:
	Shot(GLfloat x, GLfloat yOffset, GLfloat z, GLfloat rotation, GLfloat scale,
          GLuint* tex, Model* model, GLuint* program, vec3 dimensions, int collisionMode, vec3 direction, bool shadow = false) :
		DrawableObject(x, yOffset, z, rotation, scale, tex, model, program, dimensions, collisionMode, shadow), direction(direction) {};

	//overload this to add AI behaviour. return true to remove object from public vector.
	virtual bool update();
	virtual void collisionHandler(DrawableObject* obj);
	vec3 getDirection() const { return direction; }

private:
	const vec3 direction;
};

class Billboard : public DrawableObject{
public:
	Billboard(GLfloat x, GLfloat yOffset, GLfloat z, GLfloat scale, GLuint* tex, GLuint* program, vec3 dimensions, int collisionMode);

	//overload this to add AI behaviour. return true to remove object from public vector.
	virtual bool update();
	virtual void collisionHandler(DrawableObject* obj);
};

class Light : public DrawableObject{
public:
	LightSource* source;
	Light(GLfloat x, GLfloat yOffset, GLfloat z, vec3 rotation, GLfloat scale, GLuint* tex, Model* model, GLuint* program, bool shadow = false);
	//overload this to add AI behaviour. return true to remove object from public vector.
};

void drawObj(DrawableObject* obj);
