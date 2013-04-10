#include "controller.h"


void keyController(){
    xModify = 0.0;
    zModify = 0.0;
    angleMod = 0.0;
    camMod = 0.0;
    rotate = camPos + M_PI / 2;
    float rotateFront = 0.0;
    float rotateSide = 0.0;
    speed = 1.0;


    if (keyIsDown('<')){
        speed = 2.0;
    }
    
    if (keyIsDown('w')){
        xModify += -0.2 * cos(camPos);
        zModify += -0.2 * sin(camPos);
    }
    if (keyIsDown('s')){
        xModify += 0.2 * cos(camPos);
        zModify += 0.2 * sin(camPos);
        rotateFront = - M_PI;
    }
    if (keyIsDown('a')){
        xModify += -0.2 * sin(camPos);
        zModify += 0.2 * cos(camPos);
        rotateSide = - M_PI / 2;
    }
    if (keyIsDown('d')){
        xModify += 0.2 * sin(camPos);
        zModify += -0.2 * cos(camPos);
        rotateSide = M_PI / 2;
    } 

    if (keyIsDown('w') && keyIsDown('a')){
        rotate += - M_PI / 4;
    } else if (keyIsDown('w') && keyIsDown('d')){
        rotate += M_PI / 4;
    } else if (keyIsDown('s') && keyIsDown('a')){
        rotate += - 3 * M_PI / 4;
    } else if (keyIsDown('s') && keyIsDown('d')){
        rotate += 3 * M_PI / 4;
    } else {
        rotate += rotateFront + rotateSide;
    }

    if (keyIsDown('e')) {
        angleMod = M_PI / 60;
        camMod = M_PI / 60;
    } else if (keyIsDown('q')) {
        angleMod = -M_PI / 60;
        camMod = -M_PI / 60;
    }

    if (keyIsDown('m')) {
        menuPressed = !menuPressed;
    }

    if (keyIsDown(' ') && yValue == 0.5) { 
        gravity = -0.18;
        yValue = 0.55;
    }

    if (gravity < 0 && yValue > 0.5) {
        gravity += 0.035;
        yModify -= gravity;
    } else if (yValue > 1.5) {
        gravity += 0.01;
        yModify -= gravity;
    } else {
        yModify = 0;
        yValue = 0.5;
        gravity = 0;
    }
}