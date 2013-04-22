#include "draw.h"
//#include <string>

using namespace std;

struct ObjectUpdater
{
	ObjectUpdater() {}

	bool operator()(DrawableObject* obj) const
	{
		bool erase = obj->update();
		if(!erase) obj->draw();
		return erase;
	} 
};

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

    printError("pre light");

    /* Special displays */
    displayNoLight(t);
	displayTerrain();
 //   displaySingleColor(t);

	for_each(programs.begin(), programs.end(), [](GLuint* program){
		glUseProgram(*program);
		//glEnable(GL_LIGHTING);
		glUniformMatrix4fv(glGetUniformLocation(*program, "camMatrix"), 1, GL_TRUE, cam.m);
		int i = 0;
		for_each(lightSources.begin(), lightSources.end(), [program, &i](DrawableObject* obj){
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
			GLfloat pos[] = {20, 10, 10, 1.0};
			int k = 1;
			if(i % 2 == 0) k = -1; 
			GLfloat dir[] = {0, k, 0, 1.0};
			glEnable(GL_LIGHT0 + i);
			glLightfv(GL_LIGHT0 + i, GL_POSITION, pos);
			glLightfv(GL_LIGHT0 + i++, GL_SPOT_DIRECTION, dir);
		});
	});
	
    /* Display all objects */
	allObjects.erase(remove_if(allObjects.begin(), allObjects.end(), ObjectUpdater()), allObjects.end());

    printError("display");

    #if defined(_WIN32)
		glutSwapBuffers();
    #endif

    glFlush();
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

/*    string s1 = "inPosition";
    string s2 = "inNormal";
    string s3 = "inTexCoord";*/

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
