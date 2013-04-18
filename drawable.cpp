#include "drawable.h"

DrawableObject::DrawableObject(){

}

DrawableObject::DrawableObject(GLfloat x, GLfloat yOffset, GLfloat z, GLfloat rotation, GLuint* tex, Model* model, GLuint* program) : 
	x(x), yOffset(y), z(z), rotation(rotation), tex(tex), model(model), program(program) 
{
	//allocate space for model.
	trans = T(x, y = yOffset + findY(x,z), z);
	setRotation(rotation);
}

DrawableObject::DrawableObject(vec3 position, GLfloat rotation, GLuint* tex, Model* model, GLuint* program) :
	x(position.x), z(position.z), yOffset(position.y), rotation(rotation), tex(tex), model(model), program(program) 
{
	trans = T(x, y = yOffset + findY(x,z), z);
	setRotation(rotation);
}

void DrawableObject::draw(GLfloat t){
    glUseProgram(*program);
    glUniformMatrix4fv(glGetUniformLocation(*program, "camMatrix"), 1, GL_TRUE, cam.m);


    /* Making the bunny */
    trans = T(x, y, z);


	glBindTexture(GL_TEXTURE_2D, *tex);
	glUniformMatrix4fv(glGetUniformLocation(*program, "mdlMatrix"), 1, GL_TRUE, trans.m);
	DrawModel(model, *program, "inPosition", "inNormal", "inTexCoord");
}

//overload this to add AI behaviour. return true to remove object from public vector.
bool DrawableObject::update(GLfloat t){
	return false;
}

//Rotates an object (rotation += angle). See also: setRotation(GLfloat)
void DrawableObject::rotate(GLfloat angle){
	rot = Mult(Ry(angle), rot);
	updateMatrices();
}
	
//use 0,1,0 to move only along the y-axis
void DrawableObject::move(GLfloat x, GLfloat y, GLfloat z){
	setCoords(this->x+x, this->yOffset+y, this->z+z);
}

//SETS rotation (rotation = angle). See also: rotate(GLfloat)
void DrawableObject::setRotation(GLfloat angle){
	rot = Ry(angle);
	updateMatrices();
}

//use -1 if you don't want to change a value. Example: -1,0,-1 to set y = 0 while not affecting x or z
void DrawableObject::setCoords(GLfloat x, GLfloat y, GLfloat z){
	if(x!=-1)
		this->x = x;
	if(y!=-1)
		this->yOffset = y;
	if(z!=-1)
		this->z = z;
	trans = T(x, y = yOffset + findY(x,z), z);
	updateMatrices();
}

vec3 DrawableObject::getCoords(){
	return vec3(this->x, this->y, this->z);
}

void DrawableObject::updateMatrices(){
	total = Mult(trans, rot);
}
