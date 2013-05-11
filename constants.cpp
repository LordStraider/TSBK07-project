#include "constants.h"

using namespace std;

mat4 rot, trans, shear, total, cam, proj, tmp;

GLfloat camPos, yCamPos, camMod, camModY, xModify, xValue, yFind, yModify, yValue, zModify, zValue, teaY, windY;
GLfloat kingX, kingY, kingZ;

float gravity, angle, angleMod, bunnyRotation, speed, kingRotation;
bool menuPressed, gameOver;

Point3D p,l;
GLuint program, programNoLight, programShadow, programSingleColor, programInvisible, programTerrain;

GLuint dirtTex, bunnyTex, skyBoxTex, grassTex;

GLuint texWidth, texHeight;
GLfloat *vertexArray;
GLuint *indexArray;

int direction;

Model *kingKong, *bunny, *bunnyShadow, *teapot, *teapotShadow, *cube, *skyBox, *blade, *windmillWalls, *windmillRoof, *windmillBalcony, *terrain, *sphere, *lowResTree, *highResTree, *billBoard;
//Model *windmill2;

FrustumG* frustumG;

vector<GLuint*> programs;
vector<DrawableObject*> allObjects;

vector<vec3> lightDirections;
vector<vec3> lightColors; 

Player* bunnyObj;

/*DrawableObjectVector::DrawableObjectVector() : std::vector<DrawableObject*>() {};
void DrawableObjectVector::drawAll(GLfloat t){
	for_each(this->begin(), this->end(), [&t](DrawableObject* obj){
		obj->draw(t);
	});
}*/

TextureData ttex; // terrain

void init_constants(void) {
    /* GL inits*/
    glClearColor(0.2,0.2,0.5,0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); //causes error 0x500

    glActiveTexture(GL_TEXTURE0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    printError("GL inits");

    proj = frustum(left, right, bottom, top, near, far);

    gameOver = false;
    direction = 1;
    xModify = 0.0;
    yModify = 0.0;
    zModify = 0.0;
    xValue = 50.0;
    yValue = 0.0;
    yFind = 0.0;
    zValue = 40.0;
    gravity = 0.0;
    bunnyRotation = M_PI / 2;
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
    glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
    glUniform1i(glGetUniformLocation(programTerrain, "tex"), 1); // Texture unit 1
    glUniform1i(glGetUniformLocation(programTerrain, "tex2"), 2); // Texture unit 0
	printError("load objs");

	programs.push_back(&program);
	programs.push_back(&programShadow);
	programs.push_back(&programSingleColor);
	programs.push_back(&programInvisible);
	programs.push_back(&programTerrain);

// Load terrain data

    LoadTGATexture("fft-terrain.tga", &ttex);
    terrain = GenerateTerrain(&ttex);
    printError("init arrays");


    /* WindMill */
	allObjects.push_back(new SingleColor(60, 0, 30, 0, 0.8, vec3(0,1,0), windmillWalls, vec3(3.5, 13, 3.5), BOX));
    allObjects.push_back(new SingleColor(60, 0, 30, 0, 0.8, vec3(1,1,0), windmillRoof, vec3(0,0,0), NONE));
    allObjects.push_back(new SingleColor(60, 0, 30, 0, 0.8, vec3(0,1,1), windmillBalcony, vec3(0,0,0), NONE, true));
    int i;
    for (i = 0; i < 4; i++) {
        float r = i * M_PI / 2;
        allObjects.push_back(new Blade(64, 7.4, 30, r, 0.5, vec3(1,0,1), blade, vec3(0,0,0), NONE));
    }

    /* Teapot */
    allObjects.push_back(new DrawableObject(-1, 0, 258, 0, &dirtTex, teapot, &program, vec3(10, 20, 10), BOX, true));

    /* KingKong */
    allObjects.push_back(new Enemy(rand() % texWidth, 0, rand() % texHeight, 0, 1, &bunnyTex, kingKong, &program, vec3(5.1, 40, 5.1), BOX, true));
    allObjects.push_back(new Enemy(rand() % texWidth, 0, rand() % texHeight, 0, 1, &bunnyTex, kingKong, &program, vec3(5.1, 40, 5.1), BOX, true));

    /* Bunny */
    bunnyObj = new Player(xValue, 0.7, zValue, 0, 1, &bunnyTex, bunny, &program, vec3(0.8, 0.8, 0.8), SPHERE, true);
    allObjects.push_back(bunnyObj);

	for (int i = 0; i < 100; i++) {
        /* Spheres */
		//allObjects.push_back(new DrawableObject(rand() % texWidth, 0, rand() % texHeight, 0, 1, &dirtTex, sphere, &programSingleColor, vec3(1, 1, 1), SPHERE));
        /* Trees */
    	allObjects.push_back(new Tree(rand() % texWidth, 0, rand() % texHeight, 0, 1, &grassTex, highResTree, &program, vec3(0.5, 10, 0.5), BOX));
	    /* Billboards */
        allObjects.push_back(new Billboard(rand() % (texWidth-1), 10, rand() % (texHeight-1), 10, &skyBoxTex, &program, vec3(0,0,0), NONE));
    }

	for (int i = 0; i < 8; i++){
		//when lightSources works: only push to allObjects, let constructor take care of its LightSource.
		//these are only displayed for debug purposes
		//the objects are also there to make eventual subclasses simpler. Moving lights, fires, etc. 
		vec3 pos = vec3(rand() % (texWidth -1), 0, rand() % (texHeight - 1));
		Light* light = new Light(pos.x,pos.y,pos.z, pos, 1, &grassTex, bunny, &program);
		allObjects.push_back(light);
	}


//    frustumG = new FrustumG();
//    frustumG->setCamInternals(1,texWidth/texHeight,near,far);
}
