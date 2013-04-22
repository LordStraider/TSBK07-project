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
        glutWarpPointer(400,320);
        glutHideCursor();

        if (x < 350) {
            camMod = -M_PI / 60;
        } else if (x > 450) {
            camMod = M_PI / 60;
        } else {
            camMod = 0;
        }
    } else {
        camMod = 0;
        glutShowCursor();
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
//    glutPassiveMotionFunc(MouseController);
    glutTimerFunc(20, &OnTimer, 0);
    init();
    glutMainLoop();
    exit(0);
}
