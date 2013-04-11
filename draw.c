#include "draw.h"

void display(void) {
	GLfloat t;
    printError("pre display");

    /* clear the screen*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

    camPos += camMod;

    xValue += xModify * speed;
    yValue += yModify;
    zValue += zModify * speed;

    if (yModify == 0) {
        yCamPos = yValue+2;
    }
    p = SetVector(xValue + 5 * cos(camPos), yCamPos, zValue + 5 * sin(camPos));
    l = SetVector(xValue, yCamPos + 0.5, zValue);

    v = SetVector(0.0, 1.0, 0.0);
    cam = lookAtv(p, l, v);


    displayNoLight();
    displaySingleColor(t);
    displayModels(t);
    displayShadows(t);
    displayInvisible();

    printError("display");
    
    //glutSwapBuffers();
    glFlush();
}




void displaySingleColor(GLfloat t) {
	int i;
    glUseProgram(programSingleColor);
    glUniformMatrix4fv(glGetUniformLocation(programSingleColor, "camMatrix"), 1, GL_TRUE, cam.m);


    trans = T(-3.9, 0, 0);
    shear = S(0.8, 0.8, 0.8);
    total = Mult(trans, shear);
    glUniformMatrix4fv(glGetUniformLocation(programSingleColor, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(windmillRoof, programSingleColor, "inPosition", "inNormal", "inTexCoord");

    trans = T(-3.9, 0, 0);
    shear = S(0.8, 0.8, 0.8);
    total = Mult(trans, shear);
    glUniformMatrix4fv(glGetUniformLocation(programSingleColor, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(windmillBalcony, programSingleColor, "inPosition", "inNormal", "inTexCoord");

    trans = T(-3.9, 0, 0);
    shear = S(0.8, 0.8, 0.8);
    total = Mult(trans, shear);
    glUniformMatrix4fv(glGetUniformLocation(programSingleColor, "mdlMatrix"), 1, GL_TRUE, total.m);    
    DrawModel(windmillWalls, programSingleColor, "inPosition", "inNormal", "inTexCoord");
    
/*

    trans = T(-13.9, 0, 0);
    shear = S(0.8, 0.8, 0.8);
    total = Mult(trans, shear);
    glUniformMatrix4fv(glGetUniformLocation(programSingleColor, "mdlMatrix"), 1, GL_TRUE, total.m);    
    DrawModel(windmill2, programSingleColor, "inPosition", "inNormal", "inTexCoord");

*/


  
    for (i = 0; i < 4; i++) {
        trans = T(0, 7.4, 0);
        shear = S(0.5, 0.5, 0.5);
        total = Mult(trans, shear);
        rot = Rx(i * M_PI / 2 + t/1000);
        total = Mult(total, rot);
        glUniformMatrix4fv(glGetUniformLocation(programSingleColor, "mdlMatrix"), 1, GL_TRUE, total.m);
        DrawModel(blade, programSingleColor, "inPosition", "inNormal", "inTexCoord");
    }
}

void displayInvisible() {
    glUseProgram(programInvisible);
    glEnable(GL_BLEND);
    glUniformMatrix4fv(glGetUniformLocation(programInvisible, "camMatrix"), 1, GL_TRUE, cam.m);

    /* Making a collision cube */
    trans = T(xValue, yValue, zValue);
    rot = Ry(rotate + angle);
    total = Mult(trans, rot);
    shear = S(0.8, 0.83, 1.5);
    total = Mult(total, shear);
    glBindTexture(GL_TEXTURE_2D, bunnyTex);
    glUniformMatrix4fv(glGetUniformLocation(programInvisible, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(cube, programInvisible, "inPosition", "inNormal", "inTexCoord");



    trans = T(-3.9, 3, 0);
    shear = S(7, 15, 7);
    total = Mult(trans, shear);
    glBindTexture(GL_TEXTURE_2D, bunnyTex);
    glUniformMatrix4fv(glGetUniformLocation(programInvisible, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(cube, programInvisible, "inPosition", "inNormal", "inTexCoord");

    glDisable(GL_BLEND);
}

void displayModels(GLfloat t) {
    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, cam.m);


    /* Making the bunny */
    trans = T(xValue, yValue, zValue);
    rot = Ry(rotate+angle);
    total = Mult(trans, rot);
    glBindTexture(GL_TEXTURE_2D, bunnyTex);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(bunny, program, "inPosition", "inNormal", "inTexCoord");

    /* Making the teapot */
    trans = T(5, 8, -4);
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
    trans = T(xValue, 0.01, zValue);
    shear = S(1/(yValue+0.5), 0, 1/(yValue+0.5));
    rot = Ry(rotate);
    total = Mult(trans, shear);
    total = Mult(total, rot);
    glUniformMatrix4fv(glGetUniformLocation(programShadow, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(bunny, program, "inPosition", "inNormal", "inTexCoord");


    /* Making shadow under the teapot */
    trans = T(5, 0.01, -4);
    shear = S(1, 0, 1);
    total = Mult(trans, shear);
    rot = Ry(-t/1000);
    total = Mult(total, rot);
    rot = Rx(t/1000);
    total = Mult(total, rot);
    glUniformMatrix4fv(glGetUniformLocation(programShadow, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(teapot, program, "inPosition", "inNormal", "inTexCoord");
}

void displayNoLight() {
	mat4 tmp;
    glUseProgram(programNoLight);

    /* Making skybox */
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    trans = T(0, 0, 0);
    glBindTexture(GL_TEXTURE_2D, skyBoxTex);

    tmp = cam;
    tmp.m[3] = 0;
    tmp.m[7] = 0;
    tmp.m[11] = 0;
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "camMatrix"), 1, GL_TRUE, tmp.m);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "mdlMatrix"), 1, GL_TRUE, trans.m);
    DrawModel(skyBox, programNoLight, "inPosition", "inNormal", "inTexCoord");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


    /* Making cube */
    trans = T(0, 0, 0);
    shear = S(100,0, 100);
    total = Mult(trans, shear);
    glBindTexture(GL_TEXTURE_2D, cubeTex);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "camMatrix"), 1, GL_TRUE, cam.m);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(cube, programNoLight, "inPosition", "inNormal", "inTexCoord");


    /* Making walls around the area */
    trans = T(0, 2.5, 50);
    shear = S(100, 5, 0);
    total = Mult(trans, shear);
    rot = Rz(M_PI/2);
    total = Mult(total, rot);
    glBindTexture(GL_TEXTURE_2D, dirtTex);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "camMatrix"), 1, GL_TRUE, cam.m);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(cube, programNoLight, "inPosition", "inNormal", "inTexCoord");
    trans = T(0, 2.5, -50);
    shear = S(100, 5, 0);
    total = Mult(trans, shear);
    rot = Rz(M_PI/2);
    total = Mult(total, rot);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "camMatrix"), 1, GL_TRUE, cam.m);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(cube, programNoLight, "inPosition", "inNormal", "inTexCoord");

    trans = T(50, 2.5, 0);
    shear = S(0, 5, 100);
    total = Mult(trans, shear);
    rot = Rz(M_PI/2);
    total = Mult(total, rot);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "camMatrix"), 1, GL_TRUE, cam.m);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(cube, programNoLight, "inPosition", "inNormal", "inTexCoord");
    trans = T(-50, 2.5, 0);
    shear = S(0, 5, 100);
    total = Mult(trans, shear);
    rot = Rz(M_PI/2);
    total = Mult(total, rot);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "camMatrix"), 1, GL_TRUE, cam.m);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(cube, programNoLight, "inPosition", "inNormal", "inTexCoord");
}