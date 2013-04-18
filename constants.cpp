#include "constants.h"

mat4 rot, trans, shear, total, cam, proj, tmp;

GLfloat camPos, yCamPos, camMod, xModify, xValue, yFind, yModify, yValue, zModify, zValue, teaY, windY;

float gravity, angle, angleMod, rotate, speed;
bool menuPressed;

Point3D p,l;
GLuint program, programNoLight, programShadow, programSingleColor, programInvisible, programTerrain;

GLuint bunnyTex, dirtTex, cubeTex, skyBoxTex, tex1;

GLuint texWidth, texHeight;
GLfloat *vertexArray;
GLuint *indexArray;

Model *bunny, *bunnyShadow, *teapot, *teapotShadow, *cube, *skyBox, *blade, *windmillWalls, *windmillRoof, *windmillBalcony, *terrain;
//Model *windmill2;
/*
DrawableObjectVector allObjects;

DrawableObjectVector::DrawableObjectVector() : std::vector<DrawableObject*>() {};
void DrawableObjectVector::drawAll(GLfloat t){
	for_each(this->begin(), this->end(), [&t](DrawableObject* obj){
		obj->draw(t);
	});
}
*/
TextureData ttex; // terrain

void init(void) {
    /* GL inits*/
    glClearColor(0.2,0.2,0.5,0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); //causes error 0x500

    glActiveTexture(GL_TEXTURE0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    printError("GL inits");

    proj = frustum(left, right, bottom, top, near, far);

    xModify = 0.0;
    yModify = 0.0;
    zModify = 0.0;
    xValue = 50.0;
    yValue = 0.0;
    yFind = 0.0;
    zValue = 40.0;
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
    programTerrain = loadShaders("terrain.vert", "terrain.frag");

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
    LoadTGATextureSimple("skybox.tga", &skyBoxTex);
    LoadTGATextureSimple("dirt.tga", &dirtTex);
    LoadTGATextureSimple("grass.tga", &cubeTex);


    /* End of upload of geometry*/
    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, proj.m);
    glUseProgram(programShadow);
    glUniformMatrix4fv(glGetUniformLocation(programShadow, "projMatrix"), 1, GL_TRUE, proj.m);
    glUseProgram(programNoLight);
    glUniformMatrix4fv(glGetUniformLocation(programNoLight, "projMatrix"), 1, GL_TRUE, proj.m);
    glUseProgram(programSingleColor);
    glUniformMatrix4fv(glGetUniformLocation(programSingleColor, "projMatrix"), 1, GL_TRUE, proj.m);
    glUseProgram(programInvisible);
    glUniformMatrix4fv(glGetUniformLocation(programInvisible, "projMatrix"), 1, GL_TRUE, proj.m);
    glUseProgram(programTerrain);
    glUniformMatrix4fv(glGetUniformLocation(programTerrain, "projMatrix"), 1, GL_TRUE, proj.m);
    glUniform1i(glGetUniformLocation(programTerrain, "tex"), 0); // Texture unit 0
	printError("load objs");
    
// Load terrain data
    LoadTGATextureSimple("grass.tga", &tex1);
    LoadTGATexture("fft-terrain.tga", &ttex);
    terrain = GenerateTerrain(&ttex);
    teaY = findY(50, 40);
    windY = findY(60, 30);
    printError("init arrays");

	DrawableObject* bunnyObj = new DrawableObject(100,0,100,0,&tex1,bunny,&program);
	//allObjects.push_back(bunnyObj);
}
