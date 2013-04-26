#include "draw.h"
//#include <string>

using namespace std;

struct ObjectUpdater
{
	ObjectUpdater() {}

	bool operator()(DrawableObject* obj) const
	{
		bool erase = obj->update();
		if(!erase)
            obj->draw();
        else {
            delete obj;
            printf("deleted\n");
        }

		return erase;
	}
};


//This is an attempt to minimize the amount of unnecessary work that is done in DrawableObject::draw()
struct updateProgram{
	updateProgram(){}
	void operator()(GLuint* program){
		glUseProgram(*program);
		//glEnable(GL_LIGHTING);
		glUniformMatrix4fv(glGetUniformLocation(*program, "camMatrix"), 1, GL_TRUE, cam.m);
		int i = 0;
		//for_each(lightSources.begin(), lightSources.end(), [program, &i](DrawableObject* obj){
			/*this would (maybe) work if we:
				* Fixed this vector to be vector<Light*> or vector<LightSource*>
				* used actual gl* methods to pass parameters to shaders (glVertex, glPerspectiveMatrix)
			*/
			/*Light* light = (Light*)obj;
			LightSource* source = light->source;
			vec3 color = source->color;
			vec3 position = source->position;
			vec3 direction = source->direction;
			printf("###test: %d, %d, %d\n", position.x, position.y, position.z);*/
		/*	GLfloat pos[] = {20, 10, 10, 1.0};
			int k = 1;
			if(i % 2 == 0) k = -1;
			GLfloat dir[] = {0, k, 0, 1.0};
			glEnable(GL_LIGHT0 + i);
			glLightfv(GL_LIGHT0 + i, GL_POSITION, pos);
			glLightfv(GL_LIGHT0 + i++, GL_SPOT_DIRECTION, dir);
		});*/

	}
};



int frameCount = 0;
int previousTime = 0;
int fps = 0;
//please move this to some suitable place
int calculateFPS()
{
    //  Increase frame count
    frameCount++;

    //  Get the number of milliseconds since glutInit called
    //  (or first call to glutGet(GLUT ELAPSED TIME)).
    int currentTime = glutGet(GLUT_ELAPSED_TIME);

    //  Calculate time passed
    int timeInterval = currentTime - previousTime;

    if(timeInterval > 1000)
    {
        //  calculate the number of frames per second
        fps = frameCount / (timeInterval / 1000.0f);

        //  Set time
        previousTime = currentTime;

        //  Reset frame count
        frameCount = 0;
    }
	return fps;
}

void display(void) {
	GLfloat t;
    vec3 v;
	printError("pre display");

    /* clear the screen*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

    camPos += camMod;
    yCamPos += camModY;

    xValue += xModify * speed;
    yValue += yModify;
    zValue += zModify * speed;

    /*if (checkBoundaries() || checkCollisionBS()) {
        xValue -= zModify * speed;
        zModify = -xModify;
        zValue -= xModify * speed;
        xModify = -zModify;
    }*/

    yFind = findY(xValue, zValue);
    if (yFind != 1.5) {
        yValue += yModify;
    }


    Point3D mod, player, king;
    player = SetVector(xValue, yValue, zValue);
    king = SetVector(kingX, kingY, kingZ);
    mod = VectorSub(player, king);
    mod = Normalize(mod);
    kingRotation = DotProduct(mod, king);
    mod /= 6.5;
    king = VectorAdd(king, mod);
    kingX = king.x;
    kingY = findY(king.x, king.z);
    kingZ = king.z;


    p = SetVector(xValue + 9 * cos(camPos), yFind + 3 * sin(yCamPos), zValue + 9 * sin(camPos));
    l = SetVector(xValue, yFind + 3.7 + 2, zValue);

    v = SetVector(0.0, 1.0, 0.0);
    cam = lookAtv(p, l, v);
//    frustumG->setCamDef(&p, &l, &v); 

    printError("pre light");

    /* Special displays */
    displayNoLight(t);
	displayTerrain();
 //   displaySingleColor(t);

	for_each(programs.begin(), programs.end(), updateProgram());

    /* Display all objects */
	allObjects.erase(remove_if(allObjects.begin(), allObjects.end(), ObjectUpdater()), allObjects.end());
    //for_each(allObjects.begin(), allObjects.end(), ObjectUpdater());

    displayPlayerStatus();

    printError("display");

    #if defined(_WIN32)
		glutSwapBuffers();
    #endif

    glFlush();
}

void displayPlayerStatus() {
    stringstream s1;
    s1 << calculateFPS();//add number to the stream
    string fps = s1.str() + " FPS";
    sfDrawString(20, 20, (char*)fps.c_str());

    stringstream s2;
    s2 << bunnyObj->getAmmo();
    string ammo = "Ammo: " + s2.str();
    sfDrawString(-20, -30, (char*)ammo.c_str());

    stringstream s3;
    s3 << bunnyObj->getScore();
    string score = "Enemies killed: " + s3.str();
    sfDrawString(-20, -10, (char*)score.c_str());

    if (gameOver) {
        stringstream s4;
        s4 << bunnyObj->getScore();
        score = "Game Over! Enemies killed: " + s4.str();
        sfDrawString(155, 155, (char*)score.c_str());
    }
}

void displayTerrain() {
	GLfloat b = 1;
	GLfloat p_array[] = {p.x,p.y+=14,p.z};
	glUseProgram(programTerrain);

	glUniform3fv(glGetUniformLocation(programTerrain, "camPos"), 1, p_array);
    glUniform1fv(glGetUniformLocation(programTerrain, "mode"), 1, &b);

    // Build matrix

    trans = IdentityMatrix();
    total = Mult(cam, trans);
    tmp = IdentityMatrix();
    glUniformMatrix4fv(glGetUniformLocation(programTerrain, "camMatrix"), 1, GL_TRUE, tmp.m);
    glUniformMatrix4fv(glGetUniformLocation(programTerrain, "mdlMatrix"), 1, GL_TRUE, total.m);

    glBindTexture(GL_TEXTURE_2D, grassTex);
    DrawModel(terrain, program, "inPosition", "inNormal", "inTexCoord");


}

void displayNoLight(GLfloat t) {
	mat4 tmp;
    glUseProgram(programNoLight);

    /* Making skybox */
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    trans = T(0, 0, 0);
    rot = Ry(t/50000);
    total = Mult(trans, rot);

    glBindTexture(GL_TEXTURE_2D, skyBoxTex);

    tmp = cam;
    tmp.m[3] = 0;
    tmp.m[7] = 0;
    tmp.m[11] = 0;
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "camMatrix"), 1, GL_TRUE, tmp.m);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(skyBox, programNoLight, "inPosition", "inNormal", "inTexCoord");


    tmp = cam;
    tmp.m[3] = 0;
    tmp.m[7] = 0;
    tmp.m[11] = 0;
    rot = Rz(M_PI);
    total = Mult(trans, rot);
    rot = Ry(-t/50000);
    total = Mult(total, rot);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "camMatrix"), 1, GL_TRUE, tmp.m);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(skyBox, programNoLight, "inPosition", "inNormal", "inTexCoord");


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "camMatrix"), 1, GL_TRUE, cam.m);
}
