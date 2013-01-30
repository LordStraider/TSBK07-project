/* Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Includes vary a bit with platforms.
// MS Windows needs GLEW or glee.
// Mac uses slightly different paths.*/
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// linking hint for Lightweight IDE
	//uses framework Cocoa
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

GLfloat translationMatrix[] = { 1.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 1.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 1.0f, -2.0f,
								0.0f, 0.0f, 0.0f, 1.0f };

GLfloat translationMatrix2[] = {1.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 1.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 1.0f, 0.0f,
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

unsigned int bunnyVertexArrayObjID;
unsigned int klingonVertexArrayObjID;
GLuint program;
GLfloat xModify;
GLfloat yModify;
GLfloat zModify;
float gravity;
float rotateFront;
float rotateSide;
Model *m;
Model *m2;





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

	unsigned int klingonVertexBufferObjID;
	unsigned int klingonIndexBufferObjID;
	unsigned int klingonNormalBufferObjID;

	/* Reference to shader program*/

	/* GL inits*/
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_TRUE);
	printError("GL inits");

	xModify = 0.0;
	yModify = 0.0;
	zModify = 0.0;
	gravity = 0.0;
	rotateFront = 0.0;
	rotateSide = 0.0;

	/* Load and compile shader*/
	program = loadShaders("main.vert", "main.frag");
	glUseProgram(program);
	printError("init shader");
	/* Upload geometry to the GPU:*/

	m = LoadModel("bunny.obj");
	m2 = LoadModel("klingon.obj");

    glGenVertexArrays(1, &bunnyVertexArrayObjID);
    glGenBuffers(1, &bunnyVertexBufferObjID);
    glGenBuffers(1, &bunnyIndexBufferObjID);
    glGenBuffers(1, &bunnyNormalBufferObjID);
    
    
    // VBO for vertex data
    glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(glGetAttribLocation(program, "inPosition"));

    // VBO for normal data
    glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "inNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "inNormal"));
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);




    glGenVertexArrays(1, &klingonVertexArrayObjID);
    glGenBuffers(1, &klingonVertexBufferObjID);
    glGenBuffers(1, &klingonIndexBufferObjID);
    glGenBuffers(1, &klingonNormalBufferObjID);


    // VBO for vertex data
    glBindBuffer(GL_ARRAY_BUFFER, klingonVertexBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, m2->numVertices*3*sizeof(GLfloat), m2->vertexArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "inPosition2"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(glGetAttribLocation(program, "inPosition2"));

    // VBO for normal data
    glBindBuffer(GL_ARRAY_BUFFER, klingonNormalBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, m2->numVertices*3*sizeof(GLfloat), m2->normalArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "inNormal2"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "inNormal2"));
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, klingonIndexBufferObjID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m2->numIndices*sizeof(GLuint), m2->indexArray, GL_STATIC_DRAW);




 	// End of upload of geometry
	setSincosX(&rotationMatrixX, 0.0);
	setSincosY(&rotationMatrixY, 0.0);
	setSincosZ(&rotationMatrixZ, 0.0);

	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixX"), 1, GL_TRUE, rotationMatrixX);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixY"), 1, GL_TRUE, rotationMatrixY);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixZ"), 1, GL_TRUE, rotationMatrixZ);
	glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, translationMatrix);
	//glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix2"), 1, GL_TRUE, translationMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projMatrix);


	printError("init arrays");
}

void display(void) {
	printError("pre display");

	/* clear the screen*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	translationMatrix[3] += xModify; //x
	translationMatrix[7] = yModify; //y
	translationMatrix[11] += zModify; //z

	setSincosX(&rotationMatrixX, rotateFront + rotateSide);
	setSincosY(&rotationMatrixY, rotateFront + rotateSide);
	setSincosZ(&rotationMatrixZ, rotateFront + rotateSide);

/*	translationMatrix2[3] = -translationMatrix[3]; //x
	translationMatrix2[7] = -translationMatrix[7]; //y
	translationMatrix2[11] = -translationMatrix[11]; //z
*/
//	glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, translationMatrix);
//	glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix2"), 1, GL_TRUE, translationMatrix);
	//glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixX"), 1, GL_TRUE, rotationMatrixX);
//	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixY"), 1, GL_TRUE, rotationMatrixY);
	//glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixZ"), 1, GL_TRUE, rotationMatrixZ);

    glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO
    glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  	
  	glBindVertexArray(klingonVertexArrayObjID);    // Select VAO
    glDrawElements(GL_TRIANGLES, m2->numIndices, GL_UNSIGNED_INT, 0L);
	
	printError("display");
	
	glutSwapBuffers();
}

void keyUp (unsigned char key, int x, int y) { 
	switch(key){
		case 'w':
			rotateFront += PI;
			zModify = 0;
			break;
		case 's':
			rotateFront -= PI;
			zModify = 0;
			break;
		case 'a':
			rotateSide += PI / 2;
			xModify = 0;
			break;
		case 'd':
			rotateSide -= PI / 2;
			xModify = 0;
		default:
			break;
	}
} 

void processNormalKeys(unsigned char key, int x, int y){
	switch(key){
		case 'w':
			rotateFront -= PI;
			zModify = -0.08;
			break;
		case 's':
			rotateFront += PI;
			zModify = 0.08;
			break;
		case 'a':
			rotateSide -= PI / 2;
			xModify = -0.08;
			break;
		case 'd':
			rotateSide += PI / 2;
			xModify = 0.08;
			break;
		case ' ':
			if (yModify == 0) { 
				gravity = -0.2;
				yModify = 0.01;
			}
			break;
		default:
			break;
	}
}

void OnTimer(int value) {
	if (gravity < 0 && yModify > 0) { 
		yModify -= gravity;
		gravity += 0.035;
	} else if (yModify >= 0.05) {
		gravity += 0.01;
		yModify -= gravity;
	} else {
		yModify = 0;
		gravity = 0;
	}

    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display); 
	//glutKeyboardFunc(processNormalKeys);
	//glutKeyboardUpFunc(keyUp);
	glutTimerFunc(20, &OnTimer, 0);
	init ();
	glutMainLoop();
}
