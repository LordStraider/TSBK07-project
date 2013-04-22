#ifdef __APPLE__
    // Linking hint for Lightweight IDE
    // uses framework Cocoa

    #include <OpenGL/gl3.h>
    #include "lib/MicroGlut.h"
#else
    #include <GL/glew.h>
    #include <GL/glut.h>
#endif

// Third-party module imports
#include "lib/GL_utilities.h"
#include "lib/loadobj.h"
#include "lib/LoadTGA2.h"
#include "lib/VectorUtils3.h"

// Project modules
#include "controller.h"
#include "constants.h"
#include "draw.h"


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
    if (button == 0 && state == 0) {
		bunnyObj->fireBulletIfAmmo();
	}
}

void OnTimer(int value) {
    keyController();

    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

void reshape(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);
    sfSetRasterSize(w, h);
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
    glutReshapeFunc(reshape);

    initKeymapManager();
    glutMouseFunc(MouseClick);
    //glutPassiveMotionFunc(MouseController);
    glutTimerFunc(20, &OnTimer, 0);

    sfMakeRasterFont();

    init_constants();

    glutMainLoop();
    exit(0);
}
