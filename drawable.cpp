#include "drawable.h"

LightSource::LightSource(){}
LightSource::LightSource(vec3 position, vec3 direction, vec3 color) : position(position), direction(direction), color(color) {}

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

DrawableObject::DrawableObject() {}

DrawableObject::DrawableObject(GLfloat x, GLfloat yOffset, GLfloat z, GLfloat rotation, GLuint* tex, Model* model, GLuint* program, vec3 dimensions, int collisionMode, bool shadow) :
	rotation(rotation), scale(1), tex(tex), model(model), program(program), dimensions(dimensions), collisionMode(collisionMode), shadow(shadow), del(false), affectedByGravity(false)
{
	setCoords(&x, &yOffset, &z);
//	dimensions = scale * findDimensions(model);
	setRotation(rotation);
}

DrawableObject::DrawableObject(GLfloat x, GLfloat yOffset, GLfloat z, GLfloat rotation, GLfloat scale, GLuint* tex, Model* model, GLuint* program, vec3 dimensions, int collisionMode, bool shadow) :
	rotation(rotation), scale(scale), tex(tex), model(model), program(program), dimensions(dimensions), collisionMode(collisionMode), shadow(shadow), del(false), affectedByGravity(false)
{
	setCoords(&x, &yOffset, &z);
//	dimensions = scale * findDimensions(model);
	setRotation(rotation);
}

DrawableObject::DrawableObject(vec3 position, GLfloat rotation, GLuint* tex, Model* model, GLuint* program, vec3 dimensions, int collisionMode, bool shadow) :
	x(position.x), z(position.z), yOffset(position.y), rotation(rotation), tex(tex), model(model), program(program), dimensions(dimensions), collisionMode(collisionMode), shadow(shadow), del(false), affectedByGravity(false)
{
	setCoords(&x, &yOffset, &z);
//	dimensions = scale * findDimensions(model);
	setRotation(rotation);
}

void DrawableObject::draw() {
	if (!objectVisible(getCoords(), 20)) {
		return;
	}


    glUseProgram(*program);

	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	glBindTexture(GL_TEXTURE_2D, *tex);
	glUniformMatrix4fv(glGetUniformLocation(*program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(model, *program, "inPosition", "inNormal", "inTexCoord");

	vec3 coords = getCoords();
	if (shadow && findY(coords.x, coords.z) != 1.5) {
	    glUseProgram(programShadow);
	    mat4 shadowTrans = T(x, findY(x,z) + 0.1, z); //are you sure that findY(x,y) is necessary here. y - yOffset should work.
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
    dim = getDimensions();

    mat4 trans = T(pos.x, pos.y, pos.z);
    mat4 shear = S(dim.x, dim.y, dim.z);
    mat4 total = Mult(trans, shear);
    glUniformMatrix4fv(glGetUniformLocation(programInvisible, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(cube, programInvisible, "inPosition", "inNormal", "inTexCoord");

    glDisable(GL_BLEND);
    */
}

//returns random float between min and max.
float randomFloat(float Min, float Max) {
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

//Rotates an object (rotation += angle). See also: setRotation(GLfloat)
void DrawableObject::rotate(GLfloat angle) {
	rotation += angle;
	if(rotation < 0) rotation = M_PI * 2 - rotation;
	if(rotation > M_PI * 2) rotation -= M_PI * 2;
	rot = Mult(Ry(angle), rot);
	updateMatrices();
}

//use 0,1,0 to move only along the y-axis
void DrawableObject::move(GLfloat x, GLfloat y, GLfloat z) {
	this->x += x;
	this->yOffset += y;
	this->z += z;
	setCoords(&this->x, &this->yOffset, &this->z);
}

//SETS rotation (rotation = angle). See also: rotate(GLfloat)
void DrawableObject::setRotation(GLfloat angle) {
	rotation = angle;
	if(rotation < 0) rotation = M_PI * 2 - rotation;
	if(rotation > M_PI * 2) rotation -= M_PI * 2;
	rot = Ry(angle);
	updateMatrices();
}

//use -1 if you don't want to change a value. Example: -1,0,-1 to set y = 0 while not affecting x or z
void DrawableObject::setCoords(GLfloat *x, GLfloat *y, GLfloat *z) {
	stayInBounds(x, z);

	if(*x != -1)
		this->x = *x;
	if(*y != -1)
		this->yOffset = *y;
	if(*z != -1)
		this->z = *z;

	trans = T(*x, this->y = yOffset + findY(*x, *z), *z);
	updateMatrices();
}

void DrawableObject::updateMatrices() {
	mat4 s= S(scale,scale,scale);
	total = Mult(trans, Mult(rot, s));
}

void DrawableObject::stayInBounds(GLfloat *x, GLfloat *z) {
	if(*x < 5) *x = 5;
	if(*x > texWidth-5) *x = texWidth-5;
	if(*z < 5) *z = 5;
	if(*z > texHeight-5) *z = texHeight-5;
}

//overload this to add AI behaviour. return true to remove object from public vector.
bool DrawableObject::update() {
	//just testing various things we might need

	if(affectedByGravity){
		if(yOffset<=0) yOffset = 0;
		else move(0,-0.25,0);
	}

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

bool checkInterval(float x) {
	bool r = true;
	if (x - 15 < 0)
		r = false;
	else if (x + 15 > texWidth)
		r = false;
	else if (x + 15 > texHeight)
		r = false;
	return r;
}

bool Tree::update() {
	if (findY(x, z) == 1.5) {
		return true;
	}

	if(checkInterval(this->x) && checkInterval(this->z) && rand() > RAND_MAX - 1200000 && apples.size() < 6){
		//spawn an apple!
		DrawableObject* apple = new DrawableObject(this->x + randomFloat(-10,10), 20, this->z + randomFloat(-10,10), 0, 0.4, &dirtTex, sphere, &programSingleColor, vec3(0.4, 0.4, 0.4), SPHERE);
		apples.push_back(apple);
		allObjects.push_back(apple);
	}

	GLfloat distToCam = sqrt(pow(xValue - x, 2) + pow(zValue - z, 2));
	if(distToCam > 100){
		if(scale < 1.5){
			scale = 1.5;
			updateMatrices();
		}
		//would be awesome to use a billboard version of the high res tree here!
		model = lowResTree;
	}
	else{
		if(scale > 0.3){
			scale = 0.3;
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

void Player::fireBulletIfAmmo(){
    if(this->getAmmo() <= 0) return;

    subAmmo();
	float rot1 = rotation + M_PI / 2;
	Shot* shot = new Shot(x, 1, z, rot1, 0.4, &dirtTex, sphere, &programSingleColor, vec3(0.4, 0.4, 0.4), SPHERE, vec3(cos(rot1),0,sin(rot1)));
	allObjects.push_back(shot);
	//cooldown?
}

bool Player::update() {
	direction = 1;
	setRotation(bunnyRotation + angle);

   	setCoords(&xValue, &yValue, &zValue);

	for_each(allObjects.begin(), allObjects.end(), CollisionChecker(this));

	return gameOver;
}

bool Shot::update() {
	move(cos(rotation), 0, sin(rotation));
	if(x <= 1 || x >= texWidth-1 || z <= 1 || z >= texHeight-1){
		return true;
	}
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

Light::Light(GLfloat x, GLfloat yOffset, GLfloat z, vec3 rotation, GLfloat scale, GLuint* tex, Model* model, GLuint* program, bool shadow) :
		DrawableObject(x, yOffset, z, 0, scale, tex, model, program, vec3(0,0,0), NONE, shadow)
	{
		rot = Mult(Mult(Rx(rotation.x), Ry(rotation.y)), Ry(rotation.z)); //not this simple, is it?
		source = new LightSource(vec3(x, y = findY(x,z) + yOffset, z), rotation, vec3(1,1,1));
	};

void DrawableObject::collisionHandler(DrawableObject* obj) {

}


struct AppleDropper
{
	AppleDropper() {}
	bool operator()(DrawableObject* apple) {
		if(rand() % 4 == 0) return false;
		apple->affectedByGravity = true;
		return true;
	}
};

void Tree::collisionHandler(DrawableObject* obj) {
	apples.erase(remove_if(apples.begin(), apples.end(), AppleDropper()), apples.end());
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
//        allObjects.push_back(new DrawableObject(rand() % texWidth, 0, rand() % texHeight, 0, 1, &dirtTex, sphere, &programSingleColor, vec3(1, 1, 1), SPHERE));
	    addAmmo(3);
	} else {
	    xValue -= zModify * speed;
	    zModify = -xModify;
	    zValue -= xModify * speed;
	    xModify = -zModify;

	    direction = -1;

		setCoords(&xValue, &yValue, &zValue);
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
		bunnyObj->addScore();
		obj->setDel(true);
		/* Adding 2 new monsters */
	    allObjects.push_back(new Enemy(rand() % texWidth, 0, rand() % texHeight, 0, 1, &bunnyTex, kingKong, e->getProgram(), vec3(5.1, 40, 5.1), BOX, true));
	    allObjects.push_back(new Enemy(rand() % texWidth, 0, rand() % texHeight, 0, 1, &bunnyTex, kingKong, e->getProgram(), vec3(5.1, 40, 5.1), BOX, true));
	}
}

void Billboard::collisionHandler(DrawableObject* obj) {

}

void drawObj(DrawableObject* obj) {
	obj->draw();
}
