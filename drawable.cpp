#include "drawable.h"

struct CollisionChecker
{
public:
	CollisionChecker(DrawableObject* obj1) : obj1(obj1) {}

	void operator()(DrawableObject* obj2) {
		if (obj1 == obj2) {
			return;
		}

		int mode1 = obj1->getCollisionMode();
		int mode2 = obj2->getCollisionMode();

		if (mode1 == BOX && mode2 == BOX) {
			if (checkCollisionBB(obj1, obj2)) {
				obj1->collisionHandler(obj2);
				obj2->collisionHandler(obj1);
			} 
		} else if (mode1 == SPHERE && mode2 == BOX) {
			if (checkCollisionBS(obj2, obj1)) {
				obj1->collisionHandler(obj2);
				obj2->collisionHandler(obj1);
			}
		} else if (mode1 == BOX && mode2 == SPHERE) {
			if (checkCollisionBS(obj1, obj2)) {
				obj1->collisionHandler(obj2);
				obj2->collisionHandler(obj1);
			}
		} else if (mode1 == SPHERE && mode2 == SPHERE) {
			if (checkCollisionSS(obj1, obj2)) {
				obj1->collisionHandler(obj2);
				obj2->collisionHandler(obj1);
			}
		}
	}

private:
	DrawableObject* obj1;
};

vec3 findDimensions(Model* m) {
/*	int i;
	float maxx = -1e10, maxy = -1e10, maxz = -1e10, minx = 1e10, miny = 1e10, minz = 1e10;
	

	
	printf("maxz %f minz %f \n", m->radius, m->radiusXZ);
*/
	return vec3(1,1,1);//maxx-minx, maxy-miny, maxz-minz);
}

DrawableObject::DrawableObject() {

}

DrawableObject::DrawableObject(GLfloat x, GLfloat yOffset, GLfloat z, GLfloat rotation, GLuint* tex, Model* model, GLuint* program, vec3 dimensions, int collisionMode, bool shadow) : 
	rotation(rotation), scale(1), tex(tex), model(model), program(program), dimensions(dimensions), collisionMode(collisionMode), shadow(shadow), del(false) 
{
	setCoords(x,yOffset,z);
//	dimensions = scale * findDimensions(model);
	setRotation(rotation);
}

DrawableObject::DrawableObject(GLfloat x, GLfloat yOffset, GLfloat z, GLfloat rotation, GLfloat scale, GLuint* tex, Model* model, GLuint* program, vec3 dimensions, int collisionMode, bool shadow) : 
	rotation(rotation), scale(scale), tex(tex), model(model), program(program), dimensions(dimensions), collisionMode(collisionMode), shadow(shadow), del(false)
{
	setCoords(x,yOffset,z);
//	dimensions = scale * findDimensions(model);
	setRotation(rotation);
}

DrawableObject::DrawableObject(vec3 position, GLfloat rotation, GLuint* tex, Model* model, GLuint* program, vec3 dimensions, int collisionMode, bool shadow) :
	x(position.x), z(position.z), yOffset(position.y), rotation(rotation), tex(tex), model(model), program(program), dimensions(dimensions), collisionMode(collisionMode), shadow(shadow), del(false) 
{
	setCoords(x,yOffset,z);
//	dimensions = scale * findDimensions(model);
	setRotation(rotation);
}

void DrawableObject::draw() {
    glUseProgram(*program);
    glUniformMatrix4fv(glGetUniformLocation(*program, "camMatrix"), 1, GL_TRUE, cam.m);

	glBindTexture(GL_TEXTURE_2D, *tex);
	glUniformMatrix4fv(glGetUniformLocation(*program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(model, *program, "inPosition", "inNormal", "inTexCoord");

	vec3 coords = getCoords();
	if (shadow && findY(coords.x, coords.z) != 1.5) {
	    glUseProgram(programShadow);
	    glUniformMatrix4fv(glGetUniformLocation(programShadow, "camMatrix"), 1, GL_TRUE, cam.m);

	    mat4 shadowTrans = T(x, findY(x,z) + 0.1, z); //doesn't work for all models
	    mat4 sub = Mult(rot, S(scale,0,scale));
	    mat4 shadowTotal = Mult(shadowTrans, sub);

		glUniformMatrix4fv(glGetUniformLocation(programShadow, "mdlMatrix"), 1, GL_TRUE, shadowTotal.m);
		DrawModel(model, programShadow, "inPosition", "inNormal", "inTexCoord");
	}

	/*
	glUseProgram(programInvisible);
    glEnable(GL_BLEND);
    glUniformMatrix4fv(glGetUniformLocation(programInvisible, "camMatrix"), 1, GL_TRUE, cam.m);

    vec3 pos, dim;
    pos = getCoords();
    dim = getDimensons();

    mat4 trans = T(pos.x, pos.y, pos.z);
    mat4 shear = S(dim.x, dim.y, dim.z);
    mat4 total = Mult(trans, shear);
    glUniformMatrix4fv(glGetUniformLocation(programInvisible, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(cube, programInvisible, "inPosition", "inNormal", "inTexCoord");

    glDisable(GL_BLEND);
    */
}

//returns random float between min and max. 
float randomFloat(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

//Rotates an object (rotation += angle). See also: setRotation(GLfloat)
void DrawableObject::rotate(GLfloat angle) {
	rot = Mult(Ry(angle), rot);
	updateMatrices();
}
	
//use 0,1,0 to move only along the y-axis
void DrawableObject::move(GLfloat x, GLfloat y, GLfloat z) {
	setCoords(this->x+x, this->yOffset+y, this->z+z);
}

//SETS rotation (rotation = angle). See also: rotate(GLfloat)
void DrawableObject::setRotation(GLfloat angle) {
	rot = Ry(angle);
	updateMatrices();
}

//use -1 if you don't want to change a value. Example: -1,0,-1 to set y = 0 while not affecting x or z
void DrawableObject::setCoords(GLfloat x, GLfloat y, GLfloat z) {
	if(x!=-1)
		this->x = x;
	if(y!=-1)
		this->yOffset = y;
	if(z!=-1)
		this->z = z;

	stayInBounds();

	trans = T(x, this->y = yOffset + findY(x,z), z);
	updateMatrices();
}

void DrawableObject::updateMatrices() {
	mat4 s= S(scale,scale,scale);
	total = Mult(trans, Mult(rot, s));
}

void DrawableObject::stayInBounds() {
	if(x < 0) x = 0;
	if(x > texWidth) x = texWidth;
	if(z < 0) z = 0;
	if(z > texHeight) z = texHeight;
}

//overload this to add AI behaviour. return true to remove object from public vector.
bool DrawableObject::update() {
	//just testing various things we might need



	/*if(rand() > RAND_MAX-10) {
		//just testing. Will need to be able to do this if we want to fire bullets, for instance.
		allObjects.push_back(new DrawableObject(rand() % texWidth, 0, rand() % texHeight, 0, &bunnyTex, sphere, program, dimensions, collisionMode));
		printf("amount of objects now: %d\n", allObjects.size());
	}*/
	//if(rand() > RAND_MAX-1000) move(randomFloat(-2,2), 0, randomFloat(-2,2));
	//rotate(0.05);
	//if(rand() > RAND_MAX-10) return true; // randomly remove objects to test deletion of objects (pick ups, dead enemies, etc)
	return getDel();
}

bool Tree::update() {
	if (findY(x, z) == 1.5) {
		return true;
	}

	GLfloat distToCam = sqrt(pow(xValue - x, 2) + pow(zValue - z, 2));
	if(distToCam > 100) {
		if(scale < 0.9) {
			scale = 0.9;
			updateMatrices();
		}
		//would be awesome to use a billboard version of the high res tree here!
		model = lowResTree;
	}
	else{
		if(scale > 0.1) {
			scale = 0.1;
			updateMatrices();
		}
		model = highResTree;
	}
	return getDel();
}

bool Blade::update() {
	rot = Rx(rotation + (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 1000);
	updateMatrices();

	return getDel();
}

bool Enemy::update() {
	if (!gameOver) {
		vec3 direction = Normalize(VectorSub(vec3(xValue, 0, zValue), getCoords()));
		GLfloat distToCam = sqrt(pow(direction.x, 2) + pow(direction.z,2));
		float angle = atan2f(direction.z, direction.x);
		angle -= M_PI / 2; // depends on how the model is rotated.
		if(distToCam < 50) {
			setRotation(angle);
			move(direction.x*0.1, 0, direction.z*0.1);

			//DrawableObject *obj1 = this;
			//for_each(allObjects.begin(), allObjects.end(), CollisionChecker(this));

		}
		else{
			this->DrawableObject::update();		
		}
	}

	return getDel();
}

bool Player::update() {
	direction = 1;
	setRotation(bunnyRotation + angle);

   	setCoords(xValue, yValue, zValue);

	for_each(allObjects.begin(), allObjects.end(), CollisionChecker(this));
	
	return gameOver;
}

bool Shot::update() {
	vec3 newCoord = vec3(0.1,0,0.1) + getCoords();
   	setCoords(newCoord.x, 1, newCoord.z);

	for_each(allObjects.begin(), allObjects.end(), CollisionChecker(this));
	
	return getDel();
}

//use width and height rather than scale
Billboard::Billboard(GLfloat x, GLfloat yOffset, GLfloat z, GLfloat scale, GLuint* tex, GLuint* program, vec3 dimensions, int collisionMode) :
		DrawableObject(x, yOffset, z, 0, scale, tex, billBoard, program, dimensions, collisionMode, false) {};

bool Billboard::update() {
	vec3 direction = Normalize(VectorSub(vec3(xValue, 0, zValue), getCoords()));
	float angle = atan2f(direction.z, direction.x);
	angle -= M_PI / 2; // depends on how the model is rotated.
	setRotation(angle);
	return false;
}

void DrawableObject::collisionHandler(DrawableObject* obj) {

}

void Tree::collisionHandler(DrawableObject* obj) {

}

void Blade::collisionHandler(DrawableObject* obj) {

}

void Enemy::collisionHandler(DrawableObject* obj) {
	Player *p = dynamic_cast<Player*>(obj);
	if (p != NULL) {
		gameOver = true;
	}
}

void Player::collisionHandler(DrawableObject* obj) {
	Shot *s = dynamic_cast<Shot*>(obj);
	if (s != NULL) {
		return;
	}

	if (obj->getCollisionMode() == SPHERE) {
		obj->setDel(true);
	    addAmmo();
	} else {
	    xValue -= zModify * speed;
	    zModify = -xModify;
	    zValue -= xModify * speed;
	    xModify = -zModify;

	    direction = -1;

		setCoords(xValue, yValue, zValue);
	}
}

void Shot::collisionHandler(DrawableObject* obj) {
	Player *p = dynamic_cast<Player*>(obj);
	if (p != NULL) {
		return;
	}

	setDel(true);
	if (obj->getCollisionMode() == SPHERE) {
		obj->setDel(true);
	}

	Enemy *e = dynamic_cast<Enemy*>(obj);
	if (e != NULL) {
		obj->setDel(true);
	}
}

void Billboard::collisionHandler(DrawableObject* obj) {

}



void drawObj(DrawableObject* obj) {
	obj->draw();
}
