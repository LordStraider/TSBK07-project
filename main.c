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

/*#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
*/

/* Globals*/

GLfloat rot[16], trans[16], shear[16], total[16], cam[16];

#define near 1.0
#define far 30.0
#define right 1.0
#define left -1.0
#define top 1.0
#define bottom -1.0
GLfloat projMatrix[] = {	2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
							0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
							0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
							0.0f, 0.0f, -1.0f, 0.0f };

GLfloat xCamPos;
GLfloat zCamPos;
GLfloat xCamMod;
GLfloat zCamMod;
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
GLuint programShadow;

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
	yValue = 0.0;
	zValue = -2.0;
	gravity = 0.0;
	rotate = 0.0;
	angle = 0.0;
	xCamPos = 0.0;
	zCamPos = 3.0;
	menuPressed = false;

 	/* Load and compile shader*/
	program = loadShaders("main.vert", "main.frag");
	programShadow = loadShaders("mainShadow.vert", "mainShadow.frag");
	glUseProgram(program);
	printError("init shader");


	bunny = LoadModelPlus("bunnyplus.obj");
    klingon = LoadModelPlus("teapot.obj");
    ground = LoadModelPlus("cubeplus.obj");
    skyBox = LoadModelPlus("skybox.obj");

	LoadTGATextureSimple("maskros512.tga", &bunnyTex);
	LoadTGATextureSimple("skyBox512.tga", &skyBoxTex);
	LoadTGATextureSimple("dirt.tga", &klingonTex);
	LoadTGATextureSimple("grass.tga", &groundTex);


 	/* End of upload of geometry*/
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projMatrix);
	glUseProgram(programShadow);
	glUniformMatrix4fv(glGetUniformLocation(programShadow, "projMatrix"), 1, GL_TRUE, projMatrix);

	printError("init arrays");
}

void displayModels(GLfloat t) {
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, cam);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	T(0, 0, 0, trans);
	glBindTexture(GL_TEXTURE_2D, skyBoxTex);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, trans);
    DrawModel(skyBox, program, "inPosition", "inNormal", "inTexCoord");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);


	Ry(angle, rot);
	T(xValue, yValue, zValue, trans);
    Mult(rot, trans, total);
	Ry(rotate, rot);
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


	T(0, -1, 0, trans);
	S(100,0, 100, shear);
    Mult(trans, shear, total);
	glBindTexture(GL_TEXTURE_2D, groundTex);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(ground, program, "inPosition", "inNormal", "inTexCoord");
}

void displayShadows(GLfloat t) {
	glUseProgram(programShadow);
	glUniformMatrix4fv(glGetUniformLocation(programShadow, "camMatrix"), 1, GL_TRUE, cam);

	T(xValue, -0.6, zValue, trans);
	S(1/(yValue+1), 0, 1/(yValue+1), shear);
	Ry(rotate, rot);
    Mult(trans, shear, total);
    Mult(total, rot, total);
	glUniformMatrix4fv(glGetUniformLocation(programShadow, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(bunny, program, "inPosition", "inNormal", "inTexCoord");



	T(5, -0.6, -4, trans);
	S(1, 0, 1, shear);
    Mult(trans, shear, total);
	Ry(-t/1000, rot);
    Mult(total, rot, total);
	Rx(t/1000, rot);
    Mult(total, rot, total);
	glUniformMatrix4fv(glGetUniformLocation(programShadow, "mdlMatrix"), 1, GL_TRUE, total);
    DrawModel(klingon, program, "inPosition", "inNormal", "inTexCoord");
}

void display(void) {
	printError("pre display");

	/* clear the screen*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	angle += angleMod;
//	xCamPos = xValue + zValue * cos(angle);
//	zCamPos = zValue + xValue * sin(angle);

	xValue = (xValue + xModify);
	yValue = yModify;
	zValue = (zValue + zModify);
xCamPos += xCamMod;
zCamPos += zCamMod;
//	SetVector(xCamPos, 1.0, zCamPos, &p);
	SetVector(xValue + 3 * cos(xCamPos), 0.0, zValue + 3 * sin(zCamPos), &p);
	SetVector(xValue, 0.0, zValue, &l);

	lookAt(&p, &l, 0.0, 1.0, 0.0, &cam);


	displayModels(t);
	displayShadows(t);

	printError("display");
	
	glutSwapBuffers();
}

void MouseController(int x, int y){
	//CGDisplayMoveCursorToPoint(CGDirectDisplayID display, CGPoint point);
//	CGDisplayMoveCursorToPoint(200,200);
	if (!menuPressed) {
		/*GLenum WinX = glutGet(GLUT_WINDOW_WIDTH);
		GLenum WinY = glutGet(GLUT_WINDOW_HEIGHT);

		CGWarpMouseCursorPosition(CGPointMake(glutGet(GLUT_INIT_WINDOW_X) + WinX / 2, glutGet(GLUT_INIT_WINDOW_Y) + WinY / 2));
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

/*		CGWarpMouseCursorPosition(CGPointMake(200,600));
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
//	SetCursorPos(200,200);
}

void OnTimer(int value) {
	xModify = 0.0;
	zModify = 0.0;
	angleMod = 0.0;
	xCamMod = 0.0;
	zCamMod = 0.0;
	rotate = 0.0;
	
	if (keyIsDown('w') && keyIsDown('d')){
		xModify = 0.08;
		rotate = - 3 * M_PI / 4;
		zModify = -0.08;
	} else if (keyIsDown('w') && keyIsDown('a')){
		xModify = -0.08;
		rotate = 3 * M_PI / 4;
		zModify = -0.08;
	} else if (keyIsDown('s') && keyIsDown('d')){
		xModify = 0.08;
		rotate = - M_PI / 4;
		zModify = 0.08;
	} else if (keyIsDown('s') && keyIsDown('a')){
		xModify = -0.08;
		rotate = M_PI / 4;
		zModify = 0.08;
	} else if (keyIsDown('w')){
		rotate = M_PI;
		zModify = -0.08;
	} else if (keyIsDown('s')){
		rotate = 0.0;
		zModify = 0.08;
	} else if (keyIsDown('a')){
		xModify = -0.08;
		rotate = M_PI / 2;
	} else if (keyIsDown('d')){
		xModify = 0.08;
		rotate = - M_PI / 2;
	} 

	if (keyIsDown('x')) {
		angleMod = 0.08;
		xCamMod = 0.08;
		zCamMod = 0.08;
	} else if (keyIsDown('z')) {
		angleMod = -0.08;
		xCamMod = -0.08;
		zCamMod = -0.08;
	}

	if (keyIsDown('m')) {
		menuPressed = !menuPressed;
	}

	if (keyIsDown(' ') && yValue == 0) { 
		gravity = -0.3;
		yModify = 0.0;
		yValue = 0.1;
	}

	if (gravity < 0 && yValue > 0) {
		yModify -= gravity;
		gravity += 0.055;
	} else if (yValue >= 0.05) {
		gravity += 0.01;
		yModify -= gravity;
	} else {
		yModify = 0;
		yValue = 0;
		gravity = 0;
	}

    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

int main(int argc, char *argv[]) {
	glutInitWindowPosition (100, 100);
	glutInitWindowSize (800, 640);

	glutInit(&argc, argv);
	glutCreateWindow ("Zelda - Quest of the world");
	glutDisplayFunc(display);
	initKeymapManager();
//	glutPassiveMotionFunc(MouseController);
	glutTimerFunc(20, &OnTimer, 0);
	init ();
	glutMainLoop();
	return 0;
}
