#include "VectorUtils3.h"
#include "GL_utilities.h"
#include "controller.h"

class DrawableObject{
public:
	mat4 trans;
	mat4 rot;
	mat4 total;
	~DrawableObject();
	
	DrawableObject(int x, int yOffset, int z, GLfloat rotation, GLuint* tex, Model* model, GLuint* program) : 
		x(x), z(z), rotation(rotation), tex(tex), model(model), program(program) {
			trans = T(x, findY(x,z), z);
			rotate(rotation);
		}

	DrawableObject(vec3 position, GLfloat rotation, GLuint* tex, Model* model, GLuint* program) :
		x(position.x), z(position.z), yOffset(position.y), rotation(rotation), tex(tex), model(model), program(program) {
			trans = T(x, findY(x,z), z);
			rotate(rotation);
	}

	void draw(int t){
		glBindTexture(GL_TEXTURE_2D, *tex);
		glUniformMatrix4fv(glGetUniformLocation(*program, "mdlMatrix"), 1, GL_TRUE, total.m);
		DrawModel(model, *program, "inPosition", "inNormal", "inTexCoord");
	}

	//Rotates an object (rotation += angle). See also: setRotation(GLfloat)
	void rotate(GLfloat angle){
		rot = Mult(Ry(angle), rot);
		updateMatrices();
	}
	
	//use 0,1,0 to move only along the y-axis
	void move(int x, int y, int z){
		setCoords(this->x+x, this->yOffset+y, this->z+z);
	}

	//SETS rotation (rotation = angle). See also: rotate(GLfloat)
	void setRotation(GLfloat angle){
		rot = Ry(angle);
		updateMatrices();
	}

	//use NULL,0,NULL to set y = 0 while not affecting x or z
	void setCoords(int x, int y, int z){
		if(x!=NULL)
			this->x = x;
		if(z!=NULL)
			this->z = z;
		trans = T(x, findY(x,z), z);
		updateMatrices();
	}
private:
	void updateMatrices(){
		total = Mult(trans, rot);
	}
	int x, z, yOffset; //yOffset is distance from ground
	GLfloat rotation;
	Model* model;
	GLuint* program;
	GLuint* tex;
};
