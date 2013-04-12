#include "draw.h"


void display(void) {
    printError("pre display");

    /* clear the screen*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

    camPos += camMod;

    xValue += xModify * speed;
    yValue += yModify;
    zValue += zModify * speed;
    yValue = findY(xValue, zValue) + 1;

/*    if (yModify == 0) {
        yCamPos = yValue+2;
    }*/
    p = SetVector(xValue + 9 * cos(camPos), yValue+2, zValue + 9 * sin(camPos));
    l = SetVector(xValue, yValue+3 + 2, zValue);

    vec3 v = SetVector(0.0, 1.0, 0.0);
    cam = lookAtv(p, l, v);

    // Build matrix
    
    displayTexture();
    displayNoLight();
    displaySingleColor(t);
    displayModels(t);
    displayShadows(t);
    displayInvisible();

    printError("display");
    
    //glutSwapBuffers();
    glFlush();
}

void displayTexture() {
    glUseProgram(programTerrain);

    p.y += 14;
    int b = 1;
    glUniform3fv(glGetUniformLocation(programTerrain, "camPos"), 1, &p);
    glUniform1fv(glGetUniformLocation(programTerrain, "mode"), 1, &b);

    // Build matrix
    
    

    trans = IdentityMatrix();
    total = Mult(cam, trans);
    tmp = IdentityMatrix();
    glUniformMatrix4fv(glGetUniformLocation(programTerrain, "camMatrix"), 1, GL_TRUE, tmp.m);
    glUniformMatrix4fv(glGetUniformLocation(programTerrain, "mdlMatrix"), 1, GL_TRUE, total.m);

    glBindTexture(GL_TEXTURE_2D, tex1);
    DrawModel(terrain, program, "inPosition", "inNormal", "inTexCoord");
}

void displaySingleColor(GLfloat t) {
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


    int i;
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
    glUniformMatrix4fv(glGetUniformLocation(programInvisible, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(cube, programInvisible, "inPosition", "inNormal", "inTexCoord");



    trans = T(-3.9, 3, 0);
    shear = S(7, 15, 7);
    total = Mult(trans, shear);
    glUniformMatrix4fv(glGetUniformLocation(programInvisible, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(cube, programInvisible, "inPosition", "inNormal", "inTexCoord");

    glDisable(GL_BLEND);
}

void displayModels(GLfloat t) {
    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, cam.m);


    /* Making the bunny */
    trans = T(xValue, yValue, zValue);
    rot = Ry(rotate + angle);
    total = Mult(trans, rot);
    glBindTexture(GL_TEXTURE_2D, bunnyTex);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(bunny, program, "inPosition", "inNormal", "inTexCoord");

    /* Making the teapot */
    trans = T(50, teaY + 8, 40);
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
    trans = T(xValue, yValue + 13.01, zValue);
    shear = S(1/(yValue+0.5), 0, 1/(yValue+0.5));
    rot = Ry(rotate);
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

void displayNoLight() {
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
/*    trans = T(0, 0, 0);
    shear = S(100,0, 100);
    total = Mult(trans, shear);
    glBindTexture(GL_TEXTURE_2D, cubeTex);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "camMatrix"), 1, GL_TRUE, cam.m);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(cube, programNoLight, "inPosition", "inNormal", "inTexCoord");
*/

    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "camMatrix"), 1, GL_TRUE, cam.m);
    /* Making walls around the area */
    trans = T(0, 2.5, 50);
    shear = S(100, 5, 0);
    total = Mult(trans, shear);
    rot = Rz(M_PI/2);
    total = Mult(total, rot);
    glBindTexture(GL_TEXTURE_2D, dirtTex);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(cube, programNoLight, "inPosition", "inNormal", "inTexCoord");
    trans = T(0, 2.5, -50);
    shear = S(100, 5, 0);
    total = Mult(trans, shear);
    rot = Rz(M_PI/2);
    total = Mult(total, rot);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(cube, programNoLight, "inPosition", "inNormal", "inTexCoord");

    trans = T(50, 2.5, 0);
    shear = S(0, 5, 100);
    total = Mult(trans, shear);
    rot = Rz(M_PI/2);
    total = Mult(total, rot);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(cube, programNoLight, "inPosition", "inNormal", "inTexCoord");
    trans = T(-50, 2.5, 0);
    shear = S(0, 5, 100);
    total = Mult(trans, shear);
    rot = Rz(M_PI/2);
    total = Mult(total, rot);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "mdlMatrix"), 1, GL_TRUE, total.m);
    DrawModel(cube, programNoLight, "inPosition", "inNormal", "inTexCoord");

    // Build matrix
}