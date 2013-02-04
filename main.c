#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
#endif
#include "GL_utilities.h"
#include "LoadTGA.h"
#include <math.h>
#include "loadobj.h"
#include "VectorUtils2.h"



/* Globals*/
#define PI 3.14159

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

Point3D p,l;
GLuint program;
GLuint programShadow;
GLuint bunnyTex;
GLuint klingonTex;
GLuint groundTex;
GLfloat xModify;
GLfloat xValue;
GLfloat yModify;
GLfloat yValue;
GLfloat zModify;
GLfloat zValue;
float gravity;
int rotateFront;
int rotateSide;
Model *bunny;
Model *bunnyShadow;
Model *klingon;
Model *klingonShadow;
Model *ground;







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
	rotateFront = 0.0;
	rotateSide = 0.0;

 	/* Load and compile shader*/
	program = loadShaders("main.vert", "main.frag");
	programShadow = loadShaders("mainShadow.vert", "mainShadow.frag");
	glUseProgram(program);
	printError("init shader");


	bunny = LoadModelPlus("bunnyplus.obj");
    klingon = LoadModelPlus("teapot.obj");
    ground = LoadModelPlus("cubeplus.obj");

	LoadTGATextureSimple("maskros512.tga", &bunnyTex);
	LoadTGATextureSimple("dirt.tga", &klingonTex);
	LoadTGATextureSimple("grass.tga", &groundTex);


 	/* End of upload of geometry*/
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projMatrix);
	glUseProgram(programShadow);
	glUniformMatrix4fv(glGetUniformLocation(programShadow, "projMatrix"), 1, GL_TRUE, projMatrix);

	printError("init arrays");
}

float getRotation() {
	float rotate = 0.0;

	switch (rotateFront + rotateSide) {
		case 1:			// w
			rotate = PI;
			break;
		case 2:			// s
			rotate = 0.0;
			break;
		case 10:		// a
			rotate = PI / 2;
			break;
		case 20:		// d
			rotate = - PI / 2;
			break;

		case 11:		// w + a
			rotate = 3 * PI / 4;
			break;
		case 21:		// w + d
			rotate = - 3 * PI / 4;
			break;

		case 12:		// s + a
			rotate = PI / 4;
			break;
		case 22:		// s + d
			rotate = - PI / 4;
			break;
		default:
			rotate = PI;
			break;
	}

	return rotate;
}

void displayModels(GLfloat t) {
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, cam);


	T(xValue, yValue, zValue, trans);
	Ry(getRotation(), rot);
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
	Ry(getRotation(), rot);
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
	
/*	if (rotateFront == 2) {
	 	SetVector(xValue, 0.0, zValue-3, &p);
	 	SetVector(xValue, 0.0, zValue, &l);
	} else {
	 	SetVector(xValue, 0.0, (zValue+3), &p);
	 	SetVector(xValue, 0.0, zValue, &l);
	}*/	
	SetVector(xValue, 0.0, (zValue+3), &p);
 	SetVector(xValue, 0.0, zValue, &l);
	lookAt(&p, &l, 0.0, 1.0, 0.0, &cam);

	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	xValue += xModify;
	yValue += yModify;
	zValue += zModify;

	displayModels(t);
	displayShadows(t);

	printError("display");
	
	glutSwapBuffers();
}

void keyUpFunc (unsigned char key, int x, int y) { 
	switch(key){
		case 'w':
			rotateFront = 0;
			zModify = 0;
			break;
		case 's':
			rotateFront = 0;
			zModify = 0;
			break;
		case 'a':
			rotateSide = 0;
			xModify = 0;
			break;
		case 'd':
			rotateSide = 0;
			xModify = 0;
		default:
			break;
	}
} 

void processNormalKeys(unsigned char key, int x, int y){
	switch(key){
		case 'w':
			rotateFront = 1;
			zModify = -0.08;
			break;
		case 's':
			rotateFront = 2;
			zModify = 0.08;
			break;
		case 'a':
			rotateSide = 10;
			xModify = -0.08;
			break;
		case 'd':
			rotateSide = 20;
			xModify = 0.08;
			break;
		case ' ':
			if (yValue == 0) { 
				gravity = -0.1;
				yModify = 0.0;
				yValue = 0.1;
			}
			break;
		default:
			break;
	}
}

void OnTimer(int value) {
	if (gravity < 0 && yValue > 0) {
		yModify -= gravity;
		gravity += 0.035;
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
	glutInit(&argc, argv);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display); 
	glutKeyboardFunc(processNormalKeys);
	glutKeyboardUpFunc(keyUpFunc);
	glutTimerFunc(20, &OnTimer, 0);
	init ();
	glutMainLoop();
	return 0;
}
