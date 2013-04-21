#include "constants.h"

using namespace std;

mat4 rot, trans, shear, total, cam, proj, tmp;

GLfloat camPos, yCamPos, camMod, xModify, xValue, yFind, yModify, yValue, zModify, zValue, teaY, windY;
GLfloat kingX, kingY, kingZ;

float gravity, angle, angleMod, rotation, speed, kingRotation;
bool menuPressed;

Point3D p,l;
GLuint program, programNoLight, programShadow, programSingleColor, programInvisible, programTerrain;

GLuint dirtTex, bunnyTex, skyBoxTex, grassTex;

GLuint texWidth, texHeight;
GLfloat *vertexArray;
GLuint *indexArray;

Model *batmobil, *kingKong, *bunny, *bunnyShadow, *teapot, *teapotShadow, *cube, *skyBox, *blade, *windmillWalls, *windmillRoof, *windmillBalcony, *terrain, *sphere, *lowResTree, *highResTree, *billBoard;
//Model *windmill2;

vector<GLuint*> programs;
vector<DrawableObject*> allObjects;
vector<DrawableObject*> lightSources;
DrawableObject* bunnyObj;

/*DrawableObjectVector::DrawableObjectVector() : std::vector<DrawableObject*>() {};
void DrawableObjectVector::drawAll(GLfloat t){
	for_each(this->begin(), this->end(), [&t](DrawableObject* obj){
		obj->draw(t);
	});
}*/

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
    rotation = M_PI / 2;
    angle = 0.0;
    camPos = M_PI / 2;
    menuPressed = false;
    yCamPos = 2.0;
    kingRotation = 0;

    kingX = 20;
    kingY = 2;
    kingZ = 20;

    /* Load and compile shader*/
    program = loadShaders("main.vert", "main.frag");
    programNoLight = loadShaders("mainNoLight.vert", "mainNoLight.frag");
    programShadow = loadShaders("mainShadow.vert", "mainShadow.frag");
    programSingleColor = loadShaders("mainSingleColor.vert", "mainSingleColor.frag");
    programInvisible = loadShaders("mainInvisible.vert", "mainInvisible.frag");
    programTerrain = loadShaders("terrain.vert", "terrain.frag");

    printError("init shader");

    bunny = LoadModelPlus("bunnyplus.obj");
    kingKong = LoadModelPlus("King_Kong.obj");
	batmobil = LoadModelPlus("batmobile.obj");
    teapot = LoadModelPlus("teapot.obj");
    cube = LoadModelPlus("cubeplus.obj");
    skyBox = LoadModelPlus("skybox.obj");
    blade = LoadModelPlus("blade.obj");
	lowResTree = LoadModelPlus("tree_oak.obj");
	highResTree = LoadModelPlus("Tree.obj");
    sphere = LoadModelPlus("groundsphere.obj");
    windmillBalcony = LoadModelPlus("windmill-balcony.obj");
    windmillRoof = LoadModelPlus("windmill-roof.obj");
    windmillWalls = LoadModelPlus("windmill-walls.obj");
	billBoard = LoadModelPlus("quadplus.obj");
//    windmill2 = LoadModelPlus("windmill02.obj");

    LoadTGATextureSimple("skybox.tga", &skyBoxTex);
    LoadTGATextureSimple("dirt.tga", &dirtTex);
    LoadTGATextureSimple("fur.tga", &bunnyTex);
    LoadTGATextureSimple("grass.tga", &grassTex);

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

	programs.push_back(&program);
	programs.push_back(&programShadow);
	programs.push_back(&programSingleColor);
	programs.push_back(&programInvisible);
	programs.push_back(&programTerrain);

// Load terrain data

    LoadTGATexture("fft-terrain.tga", &ttex);
    terrain = GenerateTerrain(&ttex);
    teaY = findY(50, 40);
    windY = findY(60, 30);
    kingY = findY(kingX, kingZ);
    printError("init arrays");

    bunnyObj = new DrawableObject(rand() % texWidth, 0, rand() % texHeight, 0, &dirtTex, teapot, &program, true);
    allObjects.push_back(bunnyObj);
    bunnyObj = new Enemy(rand() % texWidth, 0, rand() % texHeight, 0, 1, &bunnyTex, kingKong, &program, true);
    allObjects.push_back(bunnyObj);
    bunnyObj = new DrawableObject(xValue, yValue, zValue, 0, &bunnyTex, bunny, &program, true);
    allObjects.push_back(bunnyObj);
	for (int i = 0; i < 20; i++) {
        bunnyObj = new DrawableObject(rand() % (texWidth-1), 0, rand() % (texHeight-1), 0, 1, &dirtTex, sphere, &program);
    	allObjects.push_back(bunnyObj);
		bunnyObj = new Tree(rand() % (texWidth-1), 0, rand() % (texHeight-1), 0, 1, &grassTex, highResTree, &program);
    	allObjects.push_back(bunnyObj);
		allObjects.push_back(new Billboard(rand() % (texWidth-1), 10, rand() % (texHeight-1), 10, &skyBoxTex, &program));
    }
	for (int i = 0; i < 8; i++){
		bunnyObj = new Light(20, 5, 20, vec3(rand() % 6, rand() % 6, rand() % 6), 3, &skyBoxTex, sphere, &program);
		lightSources.push_back(bunnyObj);
		//when lightSources works: only push to allObjects, let constructor take care of its LightSource.
		allObjects.push_back(bunnyObj);
	}

}
