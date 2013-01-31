#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
#endif
#include "GL_utilities.h"
#include <math.h>
#include "loadobj.h"



/* Globals*/
#define PI 3.14159

GLfloat rotationMatrixX[] = { 	1.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 1.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 1.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 1.0f };

GLfloat rotationMatrixY[] = { 	1.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 1.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 1.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 1.0f };

GLfloat rotationMatrixZ[] = { 	1.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 1.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 1.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 1.0f };

GLfloat scalingMatrix[] = {		1.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 1.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 1.0f };

GLfloat translationMatrix[] = { 1.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 1.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 1.0f, -2.0f,
								0.0f, 0.0f, 0.0f, 1.0f };

GLfloat translationMatrix2[] = { 1.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 1.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 1.0f, -2.0f,
								0.0f, 0.0f, 0.0f, 1.0f };

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

unsigned int bunnyShadowVertexArrayObjID;
unsigned int bunnyVertexArrayObjID;
unsigned int klingonVertexArrayObjID;
GLuint program;
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





void setSincosX(GLfloat* m, float alpha) { 
	m[5] = cos(alpha);
	m[6] = -sin(alpha);
	m[9] = sin(alpha);
	m[10] = cos(alpha);
}

void setSincosY(GLfloat* m, float alpha) { 
	m[0] = cos(alpha);
	m[2] = sin(alpha);
	m[8] = -sin(alpha);
	m[10] = cos(alpha);
}

void setSincosZ(GLfloat* m, float alpha) { 
	m[0] = cos(alpha);
	m[1] = -sin(alpha);
	m[4] = sin(alpha);
	m[5] = cos(alpha);
}

void init(void) {
	/* two vertex buffer objects, used for uploading the*/

	unsigned int bunnyVertexBufferObjID;
	unsigned int bunnyIndexBufferObjID;
	unsigned int bunnyNormalBufferObjID;

	unsigned int bunnyShadowVertexBufferObjID;
	unsigned int bunnyShadowIndexBufferObjID;
	unsigned int bunnyShadowNormalBufferObjID;

	unsigned int klingonVertexBufferObjID;
	unsigned int klingonIndexBufferObjID;
	unsigned int klingonNormalBufferObjID;

	/* Reference to shader program*/

	/* GL inits*/
	glClearColor(0.5f,0.5f,0.5f,0.5f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_TRUE);
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
	glUseProgram(program);
	printError("init shader");
	/* Upload geometry to the GPU:*/

	bunny = LoadModel("bunny.obj");

    glGenVertexArrays(1, &bunnyVertexArrayObjID);
    glGenBuffers(1, &bunnyVertexBufferObjID);
    glGenBuffers(1, &bunnyIndexBufferObjID);
    glGenBuffers(1, &bunnyNormalBufferObjID);
    
    
    glBindVertexArray(bunnyVertexArrayObjID);
    
    // VBO for vertex data
    glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, bunny->numVertices*3*sizeof(GLfloat), bunny->vertexArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "inPosition0"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(glGetAttribLocation(program, "inPosition0"));

    // VBO for normal data
    glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, bunny->numVertices*3*sizeof(GLfloat), bunny->normalArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "inNormal0"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "inNormal0"));
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bunny->numIndices*sizeof(GLuint), bunny->indexArray, GL_STATIC_DRAW);




	bunnyShadow = LoadModel("bunny.obj");

    glGenVertexArrays(1, &bunnyShadowVertexArrayObjID);
    glGenBuffers(1, &bunnyShadowVertexBufferObjID);
    glGenBuffers(1, &bunnyShadowIndexBufferObjID);
    glGenBuffers(1, &bunnyShadowNormalBufferObjID);
    
    
    glBindVertexArray(bunnyShadowVertexArrayObjID);
    
    // VBO for vertex data
    glBindBuffer(GL_ARRAY_BUFFER, bunnyShadowVertexBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, bunnyShadow->numVertices*3*sizeof(GLfloat), bunnyShadow->vertexArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "inPosition1"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(glGetAttribLocation(program, "inPosition1"));

    // VBO for normal data
    glBindBuffer(GL_ARRAY_BUFFER, bunnyShadowNormalBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, bunnyShadow->numVertices*3*sizeof(GLfloat), bunnyShadow->normalArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "inNormal1"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "inNormal1"));
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyShadowIndexBufferObjID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bunnyShadow->numIndices*sizeof(GLuint), bunnyShadow->indexArray, GL_STATIC_DRAW);





	klingon = LoadModel("klingon.obj");

    glGenVertexArrays(1, &klingonVertexArrayObjID);
    glGenBuffers(1, &klingonVertexBufferObjID);
    glGenBuffers(1, &klingonIndexBufferObjID);
    glGenBuffers(1, &klingonNormalBufferObjID);

    
    glBindVertexArray(klingonVertexArrayObjID);

    // VBO for vertex data
    glBindBuffer(GL_ARRAY_BUFFER, klingonVertexBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, klingon->numVertices*3*sizeof(GLfloat), klingon->vertexArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "inPosition2"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(glGetAttribLocation(program, "inPosition2"));

    // VBO for normal data
    glBindBuffer(GL_ARRAY_BUFFER, klingonNormalBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, klingon->numVertices*3*sizeof(GLfloat), klingon->normalArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "inNormal2"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "inNormal2"));
   
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, klingonIndexBufferObjID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, klingon->numIndices*sizeof(GLuint), klingon->indexArray, GL_STATIC_DRAW);




	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixX"), 1, GL_TRUE, rotationMatrixX);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixY"), 1, GL_TRUE, rotationMatrixY);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixZ"), 1, GL_TRUE, rotationMatrixZ);
	glUniformMatrix4fv(glGetUniformLocation(program, "scalingMatrix"), 1, GL_TRUE, scalingMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, translationMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix2"), 1, GL_TRUE, translationMatrix2);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projMatrix);


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
			rotate = - PI / 2;
			break;
		case 20:		// d
			rotate = PI / 2;
			break;

		case 11:		// w + a
			rotate = - 3 * PI / 4;
			break;
		case 21:		// w + d
			rotate = 3 * PI / 4;
			break;

		case 12:		// s + a
			rotate = - PI / 4;
			break;
		case 22:		// s + d
			rotate = PI / 4;
			break;
		default:
			rotate = 0.0;
			break;
	}

	return rotate;
}

void display(void) {
	printError("pre display");

	/* clear the screen*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	xValue += xModify;
	yValue += yModify;
	zValue += zModify;
	translationMatrix[3] = xValue; //x
	translationMatrix[7] = yValue; //y
	translationMatrix[11] = zValue; //z

//	setSincosX(&rotationMatrixX, getRotation());
	setSincosY(&rotationMatrixY, getRotation());
//	setSincosZ(&rotationMatrixZ, getRotation());

	glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, translationMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixY"), 1, GL_TRUE, rotationMatrixY);
    glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO
    glDrawElements(GL_TRIANGLES, bunny->numIndices, GL_UNSIGNED_INT, 0L);


	translationMatrix[3] = xValue; //x
	translationMatrix[7] = -0.2; //y
	translationMatrix[11] = zValue; //z
	glUniformMatrix4fv(glGetUniformLocation(program, "scalingMatrix"), 1, GL_TRUE, scalingMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, translationMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixY"), 1, GL_TRUE, rotationMatrixY);
  	glBindVertexArray(bunnyShadowVertexArrayObjID);    // Select VAO
    glDrawElements(GL_TRIANGLES, bunnyShadow->numIndices, GL_UNSIGNED_INT, 0L);


	translationMatrix[3] = 0; //x
	translationMatrix[7] = 0; //y
	translationMatrix[11] = -2; //z
	glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, translationMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixY"), 1, GL_TRUE, rotationMatrixY);
  	glBindVertexArray(klingonVertexArrayObjID);    // Select VAO
    glDrawElements(GL_TRIANGLES, klingon->numIndices, GL_UNSIGNED_INT, 0L);

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
