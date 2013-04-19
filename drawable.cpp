#include "drawable.h"

DrawableObject::DrawableObject(){

}

DrawableObject::DrawableObject(GLfloat x, GLfloat yOffset, GLfloat z, GLfloat rotation, GLuint* tex, Model* model, GLuint* program, bool shadow) : 
	x(x), yOffset(y), z(z), rotation(rotation), scale(1), tex(tex), model(model), program(program), shadow(shadow) 
{
	trans = T(x, y = yOffset + findY(x,z), z);
	setRotation(rotation);
}

DrawableObject::DrawableObject(GLfloat x, GLfloat yOffset, GLfloat z, GLfloat rotation, GLfloat scale, GLuint* tex, Model* model, GLuint* program, bool shadow) : 
	x(x), yOffset(y), z(z), rotation(rotation), scale(scale), tex(tex), model(model), program(program), shadow(shadow) 
{
	trans = T(x, y = yOffset + findY(x,z), z);
	setRotation(rotation);
}

DrawableObject::DrawableObject(vec3 position, GLfloat rotation, GLuint* tex, Model* model, GLuint* program, bool shadow) :
	x(position.x), z(position.z), yOffset(position.y), rotation(rotation), tex(tex), model(model), program(program), shadow(shadow) 
{
	trans = T(x, y = yOffset + findY(x,z), z);
	setRotation(rotation);
}

void DrawableObject::draw(){
    glUseProgram(*program);
    glUniformMatrix4fv(glGetUniformLocation(*program, "camMatrix"), 1, GL_TRUE, cam.m);

    GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	glBindTexture(GL_TEXTURE_2D, *tex);
	glUniformMatrix4fv(glGetUniformLocation(*program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(model, *program, "inPosition", "inNormal", "inTexCoord");
	
	if (true || shadow) { //temporary
	    glUseProgram(programShadow);
	    glUniformMatrix4fv(glGetUniformLocation(programShadow, "camMatrix"), 1, GL_TRUE, cam.m);

	    mat4 shadowTrans = T(x, y+0.01, z); //doesn't really do anything
	    mat4 sub = Mult(rot, S(scale,0,scale));
	    mat4 shadowTotal = Mult(shadowTrans, sub); //need rotation matrix as well

		glUniformMatrix4fv(glGetUniformLocation(programShadow, "mdlMatrix"), 1, GL_TRUE, shadowTotal.m);
		DrawModel(model, programShadow, "inPosition", "inNormal", "inTexCoord");
	}
}

//returns random float between min and max. 
float randomFloat(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

//overload this to add AI behaviour. return true to remove object from public vector.
bool DrawableObject::update(){
	//just testing various things we might need

	if(rand() > RAND_MAX-10) {
		//just testing. Will need to be able to do this if we want to fire bullets, for instance.
		allObjects.push_back(new DrawableObject(rand() % texWidth, 0, rand() % texHeight, 0, &bunnyTex, sphere, program));
		printf("amount of objects now: %d\n", allObjects.size());
	}
	if(rand() > RAND_MAX-1000) move(randomFloat(-2,2), 0, randomFloat(-2,2));
	rotate(0.05);
	if(rand() > RAND_MAX-10) return true; // randomly remove objects to test deletion of objects (pick ups, dead enemies, etc)
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

	stayInBounds();

	trans = T(x, y = yOffset + findY(x,z), z);
	updateMatrices();
}

vec3 DrawableObject::getCoords(){
	return vec3(this->x, this->y, this->z);
}

void DrawableObject::updateMatrices(){
	mat4 s= S(scale,scale,scale);
	total = Mult(trans, Mult(rot, s));
}

void DrawableObject::stayInBounds(){
	if(x < 0) x = 0;
	if(x > texWidth) x = texWidth;
	if(z < 0) z = 0;
	if(z > texHeight) z = texHeight;
}

bool Tree::update(){
	GLfloat distToCam = sqrt(pow(xValue - x, 2) + pow(zValue - z, 2));
	if(distToCam > 100){
		if(scale < 0.9){
			scale = 0.9;
			updateMatrices();
		}
		//would be awesome to use a billboard version of the high res tree here!
		model = lowResTree;
	}
	else{
		if(scale > 0.1){
			scale = 0.1;
			updateMatrices();
		}
		model = highResTree;
	}
	return false;
}

bool Enemy::update(){
	vec3 direction = Normalize(VectorSub(vec3(xValue, 0, zValue), getCoords()));
	GLfloat distToCam = sqrt(pow(direction.x, 2) + pow(direction.z,2));
	float angle = atan2f(direction.z, direction.x);
	angle -= M_PI / 2; // depends on how the model is rotated.
	if(distToCam < 50){
		setRotation(angle);
		move(direction.x*0.1, 0, direction.z*0.1);
	}
	else{
		this->DrawableObject::update();		
	}
	return false;
}

void drawObj(DrawableObject* obj) {
	obj->draw();
}
