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
//        else 
//            delete obj;

		return erase;
	}
};


//This is an attempt to minimize the amount of unnecessary work that is done in DrawableObject::draw()
struct updateProgram{
	updateProgram(){}
	void operator()(GLuint* program){
		glUseProgram(*program);
		glUniformMatrix4fv(glGetUniformLocation(*program, "camMatrix"), 1, GL_TRUE, cam.m);
		//we have to specify amount of lights at compile time
		//a way around this would be upgrading to some newer opengl version or passing these arrays as textures.
/*		glUniform3fv(glGetUniformLocation(*program, "lightSourcesDirArr"), 8, &lightDirections[0].x);
		glUniform3fv(glGetUniformLocation(*program, "lightSourcesColorArr"), 8, &lightColors[0].x);	
		glUniform3fv(glGetUniformLocation(*program, "lightSourcesPosArr"), 8, &lightPositions[0].x);	*/
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
    //yValue += yModify;
    zValue += zModify * speed;

    /*if (checkBoundaries() || checkCollisionBS()) {
        xValue -= zModify * speed;
        zModify = -xModify;
        zValue -= xModify * speed;
        xModify = -zModify;
    }*/

    yFind = findY(xValue, zValue);
    if (yFind >= 2) {
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

 //   displaySingleColor(t);

	for_each(programs.begin(), programs.end(), updateProgram());

    /* Display all objects */
	allObjects.erase(remove_if(allObjects.begin(), allObjects.end(), ObjectUpdater()), allObjects.end());
    //for_each(allObjects.begin(), allObjects.end(), ObjectUpdater());

    displayTerrain();

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
    glUseProgram(programTerrain);

    // Build matrix

    trans = IdentityMatrix();
    total = Mult(cam, trans);
    tmp = IdentityMatrix();
    glUniformMatrix4fv(glGetUniformLocation(programTerrain, "camMatrix"), 1, GL_TRUE, tmp.m);
    glUniformMatrix4fv(glGetUniformLocation(programTerrain, "mdlMatrix"), 1, GL_TRUE, total.m);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, grassTex);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, dirtTex);
    DrawModel(terrain, program, "inPosition", "inNormal", "inTexCoord");
    glActiveTexture(GL_TEXTURE0);


    glUseProgram(programInvisible);
    glEnable(GL_BLEND);

    //GLfloat p_array[] = {p.x,p.y+=14,p.z};
    //glUniform3fv(glGetUniformLocation(programInvisible, "camPos"), 1, p_array);

    trans = T(texHeight / 2, 2, texWidth / 2);
    shear = S(texHeight, 0, texWidth);
    total = Mult(trans, shear);
    glUniformMatrix4fv(glGetUniformLocation(programInvisible, "camMatrix"), 1, GL_TRUE, cam.m);
    glUniformMatrix4fv(glGetUniformLocation(programInvisible, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(cube, programInvisible, "inPosition", "inNormal", "inTexCoord");
    glDisable(GL_BLEND);
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

    glActiveTexture(GL_TEXTURE0);
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
