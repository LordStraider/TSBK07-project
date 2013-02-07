#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include "MicroGlut.h"
#endif
#import <ApplicationServices/ApplicationServices.h>
#include "GL_utilities.h"
#include "LoadTGA.h"
#include <math.h>
#include "loadobj.h"
#include "VectorUtils2.h"
#include <stdio.h>

/*
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
*/

/* Globals*/

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

#define FPS             50

GLfloat rot[16], trans[16], shear[16], total[16], cam[16];

#define near 1.0
#define far 300.0
#define right 1.0
#define left -1.0
#define top 1.0
#define bottom -1.0
GLfloat projMatrix[] = {    2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
                            0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
                            0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
                            0.0f, 0.0f, -1.0f, 0.0f };

GLfloat camPos;
GLfloat camMod;
GLfloat xModify;
GLfloat xValue;
GLfloat yModify;
GLfloat yValue;
GLfloat zModify;
GLfloat zValue;
float gravity;
float angle;
float angleMod;
float rotate;
bool menuPressed;

Point3D p,l;
GLuint program;
GLuint programNoLight;
GLuint programShadow;
GLuint programSingleColor;

GLuint bunnyTex;
GLuint klingonTex;
GLuint groundTex;
GLuint skyBoxTex;

Model *bunny;
Model *bunnyShadow;
Model *klingon;
Model *klingonShadow;
Model *ground;
Model *skyBox;
Model *blade;
Model *windmillWalls;
Model *windmillRoof;
Model *windmillBalcony;








void init(void) {
    /* GL inits*/
    glClearColor(0.2,0.2,0.5,0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_TRUE);
    glActiveTexture(GL_TEXTURE0);
    printError("GL inits");

    xModify = 0.0;
    yModify = 0.0;
    zModify = 0.0;
    xValue = 0.0;
    yValue = 0.5;
    zValue = -2.0;
    gravity = 0.0;
    rotate = 0.0;
    angle = 0.0;
    camPos = 1.0;
    menuPressed = false;

    /* Load and compile shader*/
    program = loadShaders("main.vert", "main.frag");
    programNoLight = loadShaders("mainNoLight.vert", "mainNoLight.frag");
    programShadow = loadShaders("mainShadow.vert", "mainShadow.frag");
    programSingleColor = loadShaders("mainSingleColor.vert", "mainSingleColor.frag");

    glUseProgram(program);

    printError("init shader");


    bunny = LoadModelPlus("bunnyplus.obj");
    klingon = LoadModelPlus("teapot.obj");
    ground = LoadModelPlus("cubeplus.obj");
    skyBox = LoadModelPlus("skybox.obj");
    blade = LoadModelPlus("blade.obj");
    windmillBalcony = LoadModelPlus("windmill-balcony.obj");
    windmillRoof = LoadModelPlus("windmill-roof.obj");
    windmillWalls = LoadModelPlus("windmill-walls.obj");


    LoadTGATextureSimple("maskros512.tga", &bunnyTex);
    LoadTGATextureSimple("SkyBox512.tga", &skyBoxTex);
    LoadTGATextureSimple("dirt.tga", &klingonTex);
    LoadTGATextureSimple("grass.tga", &groundTex);


    /* End of upload of geometry*/
    glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projMatrix);
    glUseProgram(programShadow);
    glUniformMatrix4fv(glGetUniformLocation(programShadow, "projMatrix"), 1, GL_TRUE, projMatrix);
    glUseProgram(programNoLight);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "projMatrix"), 1, GL_TRUE, projMatrix);
    glUseProgram(programSingleColor);
    glUniformMatrix4fv(glGetUniformLocation(programSingleColor, "projMatrix"), 1, GL_TRUE, projMatrix);

    printError("init arrays");
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

void displayModels(GLfloat t) {
    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, cam);


    T(xValue, yValue, zValue, trans);
    Ry(rotate+angle, rot);
    Mult(trans, rot, total);
    glBindTexture(GL_TEXTURE_2D, bunnyTex);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(bunny, program, "inPosition", "inNormal", "inTexCoord");


    T(5, 8, -4, trans);
    Ry(-t/1000, rot);
    Mult(trans, rot, total);
    Rx(t/1000, rot);
    Mult(total, rot, total);
    glBindTexture(GL_TEXTURE_2D, klingonTex);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(klingon, program, "inPosition", "inNormal", "inTexCoord");
}

void displayShadows(GLfloat t) {
    glUseProgram(programShadow);
    glUniformMatrix4fv(glGetUniformLocation(programShadow, "camMatrix"), 1, GL_TRUE, cam);

    T(xValue, 0.01, zValue, trans);
    S(1/(yValue+0.5), 0, 1/(yValue+0.5), shear);
    Ry(rotate, rot);
    Mult(trans, shear, total);
    Mult(total, rot, total);
    glUniformMatrix4fv(glGetUniformLocation(programShadow, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(bunny, program, "inPosition", "inNormal", "inTexCoord");



    T(5, 0.01, -4, trans);
    S(1, 0, 1, shear);
    Mult(trans, shear, total);
    Ry(-t/1000, rot);
    Mult(total, rot, total);
    Rx(t/1000, rot);
    Mult(total, rot, total);
    glUniformMatrix4fv(glGetUniformLocation(programShadow, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(klingon, program, "inPosition", "inNormal", "inTexCoord");
}

void displayNoLight() {
    glUseProgram(programNoLight);


    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    T(0, 0, 0, trans);
    glBindTexture(GL_TEXTURE_2D, skyBoxTex);
    GLfloat tmp[16];
    lookAt(&p, &l, 0.0, 1.0, 0.0, &tmp);
    tmp[3] = 0;
    tmp[7] = 0;
    tmp[11] = 0;
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "camMatrix"), 1, GL_TRUE, tmp);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "mdlMatrix"), 1, GL_TRUE, trans);
    DrawModel(skyBox, programNoLight, "inPosition", "inNormal", "inTexCoord");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


    T(0, 0, 0, trans);
    S(100,0, 100, shear);
    Mult(trans, shear, total);
    glBindTexture(GL_TEXTURE_2D, groundTex);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "camMatrix"), 1, GL_TRUE, cam);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(ground, programNoLight, "inPosition", "inNormal", "inTexCoord");
}

void display(void) {
    printError("pre display");

    /* clear the screen*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

    camPos += camMod;

/*
    Point3D a;
    Point3D b;
    SetVector(xValue + xModify, 0, zValue + zModify, &a);
    SetVector(camPos, 0, camPos, &b);
    Normalize(&a);
    Normalize(&b);
    GLfloat ang = DotProduct(&a, &b);
//    printf("ang: %f, xVal: %f, zVal: %f, sin: %f, cos: %f, newX: %f, newZ: %f\n", 
//            ang, xValue, zValue, sin(ang), cos(ang), (xValue + xModify) * cos(ang) + (zValue + zModify) * sin(ang), 
//            (zValue + zModify) * sin(ang) + (xValue + xModify) * cos(ang));

    angle = DotProduct(&a, &b);// += angleMod;
//  camPos = xValue + zValue * cos(angle);
//  camPos = zValue + xValue * sin(angle);
//*/
    Point3D newPos;
    SetVector(xValue + xModify, yValue + yModify, zValue + zModify, &newPos);
    Ry(angle, rot);
    MatrixMultPoint3D(rot, &newPos, &newPos);
//*/ 
   /* xValue += xModify * sin(camPos) + xModify * sin(camPos);
    yValue += yModify;
    zValue += zModify * cos(camPos) - zModify * sin(camPos);
xValue = newPos.x;
yValue = newPos.y;
zValue = newPos.z;
//*/
//    xValue += xModify * cos(ang) + zModify * sin(ang);
//    yValue = yModify;
//    zValue += zModify * -sin(ang) + xModify * cos(ang);
//  SetVector(camPos, 1.0, camPos, &p);

xValue += xModify;
yValue += yModify;
zValue += zModify;

    SetVector(xValue + 5 * cos(camPos), 2.0, zValue + 5 * sin(camPos), &p);
    SetVector(xValue, 2.5, zValue, &l);

    lookAt(&p, &l, 0.0, 1.0, 0.0, &cam);


    displayNoLight();
    displaySingleColor(t);
    displayModels(t);
    displayShadows(t);

    printError("display");
    
    glutSwapBuffers();
}

void MouseController(int x, int y){
    //CGDisplayMoveCursorToPoint(CGDirectDisplayID display, CGPoint point);
//  CGDisplayMoveCursorToPoint(200,200);
    if (!menuPressed) {
        /*GLenum WinX = glutGet(GLUT_WINDOW_WIDTH);
        GLenum WinY = glutGet(GLUT_WINDOW_HEIGHT);

        CGWarpMouseCursorPosition(CGPointMake(glutGet(GLUT_INIT_WINDOW_X) + 
        WinX / 2, glutGet(GLUT_INIT_WINDOW_Y) + WinY / 2));
        if (WinX / 2 < x) {
            xLookAt -= x / WinX;
        } else {
            xLookAt += x / WinX;
        } 

        if (WinY / 2 < y) {
            yLookAt -= x / WinY;
        } else {
            yLookAt += x / WinY;
        } */

/*      CGWarpMouseCursorPosition(CGPointMake(200,600));
        if (200 < x) {
            xLookAt -= x*100;
        } else {
            xLookAt += x*100;
        } 

        if (200 < y) {
            yLookAt -= y*100;
        } else {
            yLookAt += y*100;
        } */
    }
//  SetCursorPos(200,200);
}

void OnTimer(int value) {
    xModify = 0.0;
    zModify = 0.0;
    angleMod = 0.0;
    camMod = 0.0;
    rotate = 0.0;
    
    if (keyIsDown('w') && keyIsDown('d')){
        xModify = 0.2 * cos(camPos);
        zModify = -0.2 * sin(camPos);
        rotate = - 3 * M_PI / 4;
    } else if (keyIsDown('w') && keyIsDown('a')){
        xModify = -0.2 * cos(camPos);
        zModify = -0.2 * sin(camPos);
        rotate = 3 * M_PI / 4;
    } else if (keyIsDown('s') && keyIsDown('d')){
        xModify = 0.2 * cos(camPos);
        zModify = 0.2 * sin(camPos);
        rotate = - M_PI / 4;
    } else if (keyIsDown('s') && keyIsDown('a')){
        xModify = -0.2 * cos(camPos);
        zModify = 0.2 * sin(camPos);
        rotate = M_PI / 4;
    } else if (keyIsDown('w')){
        xModify = -0.2 * cos(camPos);
        zModify = -0.2 * sin(camPos);
        rotate = 3 * M_PI / 4;
//        zModify = -0.2 * sin(camPos);
//        rotate = M_PI;
    } else if (keyIsDown('s')){
        xModify = 0.2 * cos(camPos);
        zModify = 0.2 * sin(camPos);
        rotate = - M_PI / 4;
//        zModify = 0.2 * sin(camPos);
//        rotate = 0.0;
    } else if (keyIsDown('a')){
        xModify = -0.2 * sin(camPos);
        zModify = 0.2 * cos(camPos);
        rotate = - M_PI / 4;
//        xModify = -0.2 * cos(camPos);
//        rotate = M_PI / 2;
    } else if (keyIsDown('d')){
        xModify = 0.2 * sin(camPos);
        zModify = -0.2 * cos(camPos);

//        xModify = 0.2 * cos(camPos);
        rotate = - M_PI / 2;
    } 

    if (keyIsDown('x')) {
        angleMod = M_PI / FPS;
        camMod = M_PI / FPS;
    } else if (keyIsDown('z')) {
        angleMod = -M_PI / FPS;
        camMod = -M_PI / FPS;
    }

    if (keyIsDown('m')) {
        menuPressed = !menuPressed;
    }

    if (keyIsDown(' ') && yValue == 0.5) { 
        gravity = -0.1;
        yValue = 0.55;
    }

    if (gravity < 0 && yValue > 0.5) {
        yModify -= gravity;
        gravity += 0.025;
    } else if (yValue >= 1.00) {
        gravity += 0.012;
        yModify -= gravity;
    } else {
        yModify = 0;
        yValue = 0.5;
        gravity = 0;
    }

    glutPostRedisplay();
    glutTimerFunc(1000/FPS, &OnTimer, value);
}

int main(int argc, char *argv[]) {
    glutInitWindowPosition (100, 100);
    glutInitWindowSize (800, 640);

    glutInit(&argc, argv);
    glutCreateWindow ("Zelda - Quest of the world");
    glutDisplayFunc(display);
    initKeymapManager();
//  glutPassiveMotionFunc(MouseController);
    glutTimerFunc(1000/FPS, &OnTimer, 0);
    init ();
    glutMainLoop();
    return 0;
}
