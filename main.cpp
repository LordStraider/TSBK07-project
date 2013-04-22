#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include "MicroGlut.h"
    // Linking hint for Lightweight IDE
    // uses framework Cocoa
#else
//  #include <GL/glee.h> Might be needed for Windows - not tested
    #include <GL/glew.h>
    #include <GL/glut.h>
#endif

#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA2.h"

#include "controller.h"
#include "constants.h"
#include "draw.h"
/* Globals*/

void MouseController(int x, int y){
    if (!menuPressed) {
        //glutWarpPointer(400,320);
        //glutHideCursor(); NO WINDOWS SUPPORT FOR THIS :( : May I suggest implementing the STANDARD setCursor method in MicroGlut?
		
		int width = 800; //glutGet(GLUT_WINDOW_WIDTH);
		
		if (x < width / 4) {
            camMod = -M_PI / 60;
        } else if (x > 3 * (width / 4)) {
            camMod = M_PI / 60;
        } else {
            camMod = 0;
        }
    } else {
        camMod = 0;
       // glutShowCursor(); NO WINDOWS SUPPORT FOR THIS :(
    }
}

void MouseClick(int button, int state, int x, int y) {
    if (button == 0 && state == 0 && bunnyObj->getAmmo() > 0) {
		bunnyObj->subAmmo();
        printf("ammo: %i\n", bunnyObj->getAmmo());
        vec3 direction = vec3(xValue+1, 0, zValue+1);
		float rot1 = bunnyObj->rotation + M_PI / 2;
		Shot* shot = new Shot(xValue, 1, zValue, rot1, 0.4, &dirtTex, sphere, &programSingleColor, vec3(0.4, 0.4, 0.4), SPHERE, direction);
		allObjects.push_back(shot);
    }
}

void OnTimer(int value) {
    keyController();

    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

int main(int argc, char *argv[]) {
	glutInitWindowPosition (100, 100);
    glutInitWindowSize (800, 640);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow ("TSBK07 - project");
    
    //glutHideCursor();

	//windows only...
	#if defined(_WIN32)
	{
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
		  /* Problem: glewInit failed, something is seriously wrong. */
		  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		}
	}
	#endif

    glutDisplayFunc(display);
    initKeymapManager();
    glutMouseFunc(MouseClick);
    //glutPassiveMotionFunc(MouseController);
    glutTimerFunc(20, &OnTimer, 0);
    init();
    glutMainLoop();
    exit(0);
}
