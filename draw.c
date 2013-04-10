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

    if (yModify == 0) {
        yCamPos = yValue+2;
    }
    SetVector(xValue + 5 * cos(camPos), yCamPos, zValue + 5 * sin(camPos), &p);
    SetVector(xValue, yCamPos + 0.5, zValue, &l);

    lookAt(&p, &l, 0.0, 1.0, 0.0, cam);


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
    glUseProgram(programSingleColor);
    glUniformMatrix4fv(glGetUniformLocation(programSingleColor, "camMatrix"), 1, GL_TRUE, cam);


    T(-3.9, 0, 0, trans);
    S(0.8, 0.8, 0.8, shear);
    Mult(trans, shear, total);
    glUniformMatrix4fv(glGetUniformLocation(programSingleColor, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(windmillRoof, programSingleColor, "inPosition", "inNormal", "inTexCoord");

    T(-3.9, 0, 0, trans);
    S(0.8, 0.8, 0.8, shear);
    Mult(trans, shear, total);
    glUniformMatrix4fv(glGetUniformLocation(programSingleColor, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(windmillBalcony, programSingleColor, "inPosition", "inNormal", "inTexCoord");

    T(-3.9, 0, 0, trans);
    S(0.8, 0.8, 0.8, shear);
    Mult(trans, shear, total);
    glUniformMatrix4fv(glGetUniformLocation(programSingleColor, "mdlMatrix"), 1, GL_TRUE, total);    
    DrawModel(windmillWalls, programSingleColor, "inPosition", "inNormal", "inTexCoord");
    
/*

    T(-13.9, 0, 0, trans);
    S(0.8, 0.8, 0.8, shear);
    Mult(trans, shear, total);
    glUniformMatrix4fv(glGetUniformLocation(programSingleColor, "mdlMatrix"), 1, GL_TRUE, total);    
    DrawModel(windmill2, programSingleColor, "inPosition", "inNormal", "inTexCoord");

*/


    int i;
    for (i = 0; i < 4; i++) {
        T(0, 7.4, 0, trans);
        S(0.5, 0.5, 0.5, shear);
        Mult(trans, shear, total);
        Rx(i * M_PI / 2 + t/1000, rot);
        Mult(total, rot, total);
        glUniformMatrix4fv(glGetUniformLocation(programSingleColor, "mdlMatrix"), 1, GL_TRUE, total);
        DrawModel(blade, programSingleColor, "inPosition", "inNormal", "inTexCoord");
    }
}

void displayInvisible() {
    glUseProgram(programInvisible);
    glEnable(GL_BLEND);
    glUniformMatrix4fv(glGetUniformLocation(programInvisible, "camMatrix"), 1, GL_TRUE, cam);

    /* Making a collision cube */
    T(xValue, yValue, zValue, trans);
    Ry(rotate + angle, rot);
    Mult(trans, rot, total);
    S(0.8, 0.83, 1.5, shear);
    Mult(total, shear, total);
    glBindTexture(GL_TEXTURE_2D, bunnyTex);
    glUniformMatrix4fv(glGetUniformLocation(programInvisible, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(cube, programInvisible, "inPosition", "inNormal", "inTexCoord");



    T(-3.9, 3, 0, trans);
    S(7, 15, 7, shear);
    Mult(trans, shear, total);
    glBindTexture(GL_TEXTURE_2D, bunnyTex);
    glUniformMatrix4fv(glGetUniformLocation(programInvisible, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(cube, programInvisible, "inPosition", "inNormal", "inTexCoord");

    glDisable(GL_BLEND);
}

void displayModels(GLfloat t) {
    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, cam);


    /* Making the bunny */
    T(xValue, yValue, zValue, trans);
    Ry(rotate+angle, rot);
    Mult(trans, rot, total);
    glBindTexture(GL_TEXTURE_2D, bunnyTex);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(bunny, program, "inPosition", "inNormal", "inTexCoord");

    /* Making the teapot */
    T(5, 8, -4, trans);
    Ry(-t/1000, rot);
    Mult(trans, rot, total);
    Rx(t/1000, rot);
    Mult(total, rot, total);
    glBindTexture(GL_TEXTURE_2D, dirtTex);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(teapot, program, "inPosition", "inNormal", "inTexCoord");
}

void displayShadows(GLfloat t) {
    glUseProgram(programShadow);
    glUniformMatrix4fv(glGetUniformLocation(programShadow, "camMatrix"), 1, GL_TRUE, cam);

    /* Making shadow under the bunny */
    T(xValue, 0.01, zValue, trans);
    S(1/(yValue+0.5), 0, 1/(yValue+0.5), shear);
    Ry(rotate, rot);
    Mult(trans, shear, total);
    Mult(total, rot, total);
    glUniformMatrix4fv(glGetUniformLocation(programShadow, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(bunny, program, "inPosition", "inNormal", "inTexCoord");


    /* Making shadow under the teapot */
    T(5, 0.01, -4, trans);
    S(1, 0, 1, shear);
    Mult(trans, shear, total);
    Ry(-t/1000, rot);
    Mult(total, rot, total);
    Rx(t/1000, rot);
    Mult(total, rot, total);
    glUniformMatrix4fv(glGetUniformLocation(programShadow, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(teapot, program, "inPosition", "inNormal", "inTexCoord");
}

void displayNoLight() {
    glUseProgram(programNoLight);

    /* Making skybox */
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    T(0, 0, 0, trans);
    glBindTexture(GL_TEXTURE_2D, skyBoxTex);
    GLfloat tmp[16];
    CopyMatrix(cam, tmp);
    tmp[3] = 0;
    tmp[7] = 0;
    tmp[11] = 0;
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "camMatrix"), 1, GL_TRUE, tmp);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "mdlMatrix"), 1, GL_TRUE, trans);
    DrawModel(skyBox, programNoLight, "inPosition", "inNormal", "inTexCoord");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


    /* Making cube */
    T(0, 0, 0, trans);
    S(100,0, 100, shear);
    Mult(trans, shear, total);
    glBindTexture(GL_TEXTURE_2D, cubeTex);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "camMatrix"), 1, GL_TRUE, cam);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(cube, programNoLight, "inPosition", "inNormal", "inTexCoord");


    /* Making walls around the area */
    T(0, 2.5, 50, trans);
    S(100, 5, 0, shear);
    Mult(trans, shear, total);
    Rz(M_PI/2, rot);
    Mult(total, rot, total);
    glBindTexture(GL_TEXTURE_2D, dirtTex);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "camMatrix"), 1, GL_TRUE, cam);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(cube, programNoLight, "inPosition", "inNormal", "inTexCoord");
    T(0, 2.5, -50, trans);
    S(100, 5, 0, shear);
    Mult(trans, shear, total);
    Rz(M_PI/2, rot);
    Mult(total, rot, total);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "camMatrix"), 1, GL_TRUE, cam);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(cube, programNoLight, "inPosition", "inNormal", "inTexCoord");

    T(50, 2.5, 0, trans);
    S(0, 5, 100, shear);
    Mult(trans, shear, total);
    Rz(M_PI/2, rot);
    Mult(total, rot, total);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "camMatrix"), 1, GL_TRUE, cam);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(cube, programNoLight, "inPosition", "inNormal", "inTexCoord");
    T(-50, 2.5, 0, trans);
    S(0, 5, 100, shear);
    Mult(trans, shear, total);
    Rz(M_PI/2, rot);
    Mult(total, rot, total);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "camMatrix"), 1, GL_TRUE, cam);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(cube, programNoLight, "inPosition", "inNormal", "inTexCoord");
}