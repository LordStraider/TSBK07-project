#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include "MicroGlut.h"
    // Linking hint for Lightweight IDE
    // uses framework Cocoa
#import <ApplicationServices/ApplicationServices.h>
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
#include "controller.h"

/* Globals*/


void MouseController(int x, int y){
    //CGDisplayMoveCursorToPoint(CGDirectDisplayID display, CGPoint point);
//  CGDisplayMoveCursorToPoint(200,200);
    if (!menuPressed) {
        /*GLenum WinX = glutGet(GLUT_WINDOW_WIDTH);
        GLenum WinY = glutGet(GLUT_WINDOW_HEIGHT);

        CGWarpMouseCursorPosition(CGPointMake(glutGet(GLUT_INIT_WINDOW_X) + 
        WinX / 2, glutGet(GLUT_INIT_WINDOW_Y) + WinY / 2));
        if (WinX / 2 < x) {
            xLookAt -= x / WinX;
        } else {
            xLookAt += x / WinX;
        } 

        if (WinY / 2 < y) {
            yLookAt -= x / WinY;
        } else {
            yLookAt += x / WinY;
        } */

/*      CGWarpMouseCursorPosition(CGPointMake(200,600));
        if (200 < x) {
            xLookAt -= x*100;
        } else {
            xLookAt += x*100;
        } 

        if (200 < y) {
            yLookAt -= y*100;
        } else {
            yLookAt += y*100;
        } */
    }
//  SetCursorPos(200,200);
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

	//windows only...
	#if defined(_WIN32)
		err = glewInit();
		if (GLEW_OK != err)
		{
		  /* Problem: glewInit failed, something is seriously wrong. */
		  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		}
	#endif

    glutDisplayFunc(display);
    initKeymapManager();
//  glutPassiveMotionFunc(MouseController);
    glutTimerFunc(20, &OnTimer, 0);
    init ();
    glutMainLoop();
    exit(0);
}
