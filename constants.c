#include "constants.h"

mat4 rot, trans, shear, total, cam;


GLfloat projMatrix[16] = { 2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
	                            0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
	                            0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
	                            0.0f, 0.0f, -1.0f, 0.0f };
GLfloat camPos;
GLfloat yCamPos;
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
float speed;
bool menuPressed;

Point3D p,l, v;
GLuint program;
GLuint programNoLight;
GLuint programShadow;
GLuint programSingleColor;
GLuint programInvisible;

GLuint bunnyTex;
GLuint dirtTex;
GLuint cubeTex;
GLuint skyBoxTex;

Model *bunny;
Model *bunnyShadow;
Model *teapot;
Model *teapotShadow;
Model *cube;
Model *skyBox;
Model *blade;
//Model *windmill2;
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
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    printError("GL inits");

    xModify = 0.0;
    yModify = 0.0;
    zModify = 0.0;
    xValue = 0.0;
    yValue = 0.5;
    zValue = -2.0;
    gravity = 0.0;
    rotate = M_PI / 2;
    angle = 0.0;
    camPos = M_PI / 2;
    menuPressed = false;
    yCamPos = 2.0;

    /* Load and compile shader*/
    program = loadShaders("main.vert", "main.frag");
    programNoLight = loadShaders("mainNoLight.vert", "mainNoLight.frag");
    programShadow = loadShaders("mainShadow.vert", "mainShadow.frag");
    programSingleColor = loadShaders("mainSingleColor.vert", "mainSingleColor.frag");
    programInvisible = loadShaders("mainInvisible.vert", "mainInvisible.frag");

    glUseProgram(program);

    printError("init shader");


    bunny = LoadModelPlus("bunnyplus.obj");
    teapot = LoadModelPlus("teapot.obj");
    cube = LoadModelPlus("cubeplus.obj");
    skyBox = LoadModelPlus("skybox.obj");
    blade = LoadModelPlus("blade.obj");
    windmillBalcony = LoadModelPlus("windmill-balcony.obj");
    windmillRoof = LoadModelPlus("windmill-roof.obj");
    windmillWalls = LoadModelPlus("windmill-walls.obj");
//    windmill2 = LoadModelPlus("windmill02.obj");


    LoadTGATextureSimple("maskros512.tga", &bunnyTex);
    LoadTGATextureSimple("SkyBox512.tga", &skyBoxTex);
    LoadTGATextureSimple("dirt.tga", &dirtTex);
    LoadTGATextureSimple("grass.tga", &cubeTex);


    /* End of upload of geometry*/
    glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projMatrix);
    glUseProgram(programShadow);
    glUniformMatrix4fv(glGetUniformLocation(programShadow, "projMatrix"), 1, GL_TRUE, projMatrix);
    glUseProgram(programNoLight);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "projMatrix"), 1, GL_TRUE, projMatrix);
    glUseProgram(programSingleColor);
    glUniformMatrix4fv(glGetUniformLocation(programSingleColor, "projMatrix"), 1, GL_TRUE, projMatrix);
    glUseProgram(programInvisible);
    glUniformMatrix4fv(glGetUniformLocation(programInvisible, "projMatrix"), 1, GL_TRUE, projMatrix);

    printError("init arrays");
}
