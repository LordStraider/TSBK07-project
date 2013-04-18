#include "draw.h"
//#include <string>
#include <math.h>


void display(void) {
	GLfloat t;
    vec3 v;
	printError("pre display");

    /* clear the screen*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

    camPos += camMod;

    xValue += xModify * speed;
    yValue += yModify;
    zValue += zModify * speed;

    if (checkBoundaries() || checkCollisionBS()) {
        xValue -= zModify * speed;
        zModify = -xModify;
        zValue -= xModify * speed;
        xModify = -zModify;
    }

    yFind = findY(xValue, zValue);
    yValue += yModify;


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


    p = SetVector(xValue + 9 * cos(camPos), yFind + 3, zValue + 9 * sin(camPos));
    l = SetVector(xValue, yFind + 3.7 + 2, zValue);

    v = SetVector(0.0, 1.0, 0.0);
    cam = lookAtv(p, l, v);

    printError("pre light");

    displayNoLight(t);

//	printError("drawing no light");
	displayTerrain();
//	printError("drawing texture");
    displaySingleColor(t);
//	printError("drawing single");
	displayModels(t);
//	printError("drawing models");
    displayShadows(t);
//	printError("drawing shadows");
    displayInvisible(t);
//	printError("drawing invisible");

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

void displaySingleColor(GLfloat t) {
	int i;
    glUseProgram(programSingleColor);
    glUniformMatrix4fv(glGetUniformLocation(programSingleColor, "camMatrix"), 1, GL_TRUE, cam.m);

    trans = T(60, windY, 30);
    shear = S(0.8, 0.8, 0.8);
    total = Mult(trans, shear);
    glUniformMatrix4fv(glGetUniformLocation(programSingleColor, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(windmillRoof, programSingleColor, "inPosition", "inNormal", "inTexCoord");

    trans = T(60, windY, 30);
    shear = S(0.8, 0.8, 0.8);
    total = Mult(trans, shear);
    glUniformMatrix4fv(glGetUniformLocation(programSingleColor, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(windmillBalcony, programSingleColor, "inPosition", "inNormal", "inTexCoord");

    trans = T(60, windY, 30);
    shear = S(0.8, 0.8, 0.8);
    total = Mult(trans, shear);
    glUniformMatrix4fv(glGetUniformLocation(programSingleColor, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(windmillWalls, programSingleColor, "inPosition", "inNormal", "inTexCoord");


    for (i = 0; i < 4; i++) {
        trans = T(64, windY + 7.4, 30);
        shear = S(0.5, 0.5, 0.5);
        total = Mult(trans, shear);
        rot = Rx(i * M_PI / 2 + t/1000);
        total = Mult(total, rot);
        glUniformMatrix4fv(glGetUniformLocation(programSingleColor, "mdlMatrix"), 1, GL_TRUE, total.m);
        DrawModel(blade, programSingleColor, "inPosition", "inNormal", "inTexCoord");
    }
}

void displayInvisible(GLfloat t) {
	glUseProgram(programInvisible);
    glEnable(GL_BLEND);
    glUniformMatrix4fv(glGetUniformLocation(programInvisible, "camMatrix"), 1, GL_TRUE, cam.m);

    /* Making a collision cube */
/*    trans = T(xValue, yValue, zValue);
//    rot = Ry(rotation + angle);
//    total = Mult(trans, rot);
    shear = S(1.6, 0.83, 1.5);
    total = Mult(trans, shear);
    glUniformMatrix4fv(glGetUniformLocation(programInvisible, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(cube, programInvisible, "inPosition", "inNormal", "inTexCoord");
*/

    trans = T(60, windY+5, 30);
    shear = S(7, 13, 7);
    total = Mult(trans, shear);
    glUniformMatrix4fv(glGetUniformLocation(programInvisible, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(cube, programInvisible, "inPosition", "inNormal", "inTexCoord");


    trans = T(xValue, yValue-1, zValue);
    shear = S(0.8, 0.8, 0.8);
    total = Mult(trans, shear);
    glUniformMatrix4fv(glGetUniformLocation(programInvisible, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(sphere, programInvisible, "inPosition", "inNormal", "inTexCoord");


    glDisable(GL_BLEND);
}

void displayModels(GLfloat t) {
    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, cam.m);


    /* Making the bunny */
    trans = T(xValue, yValue, zValue);
    rot = Ry(rotation + angle);
    total = Mult(trans, rot);
    glBindTexture(GL_TEXTURE_2D, bunnyTex);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(bunny, program, "inPosition", "inNormal", "inTexCoord");

    /* Making the bunny */
    trans = T(kingX, kingY, kingZ);
    shear = S(0.6, 0.6, 0.6);
    total = Mult(trans, shear);
    rot = Ry(rotation);
    total = Mult(total, rot);
    glBindTexture(GL_TEXTURE_2D, bunnyTex);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(kingKong, program, "inPosition", "inNormal", "inTexCoord");

    /* Making the teapot */
    trans = T(50, teaY + 18, 40);
    rot = Ry(-t/1000);
    total = Mult(trans, rot);
    rot = Rx(t/1000);
    total = Mult(total, rot);
    glBindTexture(GL_TEXTURE_2D, dirtTex);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(teapot, program, "inPosition", "inNormal", "inTexCoord");
}

void displayShadows(GLfloat t) {
    glUseProgram(programShadow);
    glUniformMatrix4fv(glGetUniformLocation(programShadow, "camMatrix"), 1, GL_TRUE, cam.m);

    /* Making shadow under the bunny */
    trans = T(xValue, yFind + 0.2, zValue);
    shear = S(1, 0, 1);
    //shear = S(1/(yValue - yFind + 0.1), 0, 1/(yValue - yFind + 0.1)); //Makes the shaddow smaller when jumping, perspective. Buggy.
    rot = Ry(rotation);
    total = Mult(trans, shear);
    total = Mult(total, rot);
    glUniformMatrix4fv(glGetUniformLocation(programShadow, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(bunny, program, "inPosition", "inNormal", "inTexCoord");


    /* Making shadow under the teapot */
    trans = T(50, teaY + 0.01, 40);
    shear = S(1, 0, 1);
    total = Mult(trans, shear);
    rot = Ry(-t/1000);
    total = Mult(total, rot);
    rot = Rx(t/1000);
    total = Mult(total, rot);
    glUniformMatrix4fv(glGetUniformLocation(programShadow, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(teapot, program, "inPosition", "inNormal", "inTexCoord");
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
