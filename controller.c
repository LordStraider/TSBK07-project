#include "controller.h"


Model* GenerateTerrain(TextureData *tex)
{
    int vertexCount = tex->width * tex->height;
    int triangleCount = (tex->width-1) * (tex->height-1) * 2;
    int x, z;
    
    vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
    GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
    GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
    indexArray = malloc(sizeof(GLuint) * triangleCount*3);
    //ballY = malloc(sizeof(GLfloat) * 100);
    
    texWidth = tex->width;
    GLfloat y;

    printf("bpp %d\n", tex->bpp);
    for (x = 0; x < tex->width; x++)
        for (z = 0; z < tex->height; z++)
        {
// Vertex array. You need to scale this properly
            vertexArray[(x + z * tex->width)*3 + 0] = x;
            y = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 25.0;
            if (y < 1.5) {
                y = 1.5;
            }
            vertexArray[(x + z * tex->width)*3 + 1] = y;
            vertexArray[(x + z * tex->width)*3 + 2] = z;
// Normal vectors. You need to calculate these.
            normalArray[(x + z * tex->width)*3 + 0] = 0.0;
            normalArray[(x + z * tex->width)*3 + 1] = 1.0;
            normalArray[(x + z * tex->width)*3 + 2] = 0.0;
// Texture coordinates. You may want to scale them.
            texCoordArray[(x + z * tex->width)*2 + 0] = x; // (float)x / tex->width;
            texCoordArray[(x + z * tex->width)*2 + 1] = z; // (float)z / tex->height;
        }

    for (x = 0; x < tex->width-1; x++)
        for (z = 0; z < tex->height-1; z++)
        {
        // Triangle 1
            indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
            indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
            indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
        // Triangle 2
            indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
            indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
            indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
        }
    
/*
    Point3D *TmpCrossArray[3 * 9];
    for (x = 0; x < tex->width; x++)
        for (z = 0; z < tex->height; z++) {
            //vår punkt!
            Point3D p = vertexArray[(x + z * tex->width)*3 + 0], vertexArray[(x + z * tex->width)*3 + 1], vertexArray[(x + z * tex->width)*3 + 2]);
            //Hämta alla vertices som är grannar med oss och ta kryssprodukten av dem!
            for (k = -1; k < 1; k++){
                for (j = -1; j < 1; j++){
                    // Funkar (?) men problem vid hörn och kanter!! 
                    TmpCrossArray[k + 1 + (j + 1) * 3] = CrossProduct(
Point3D(vertexArray[((x+k) + (z+j) * tex->width)*3 + 0], vertexArray[((x+k) + (z+j) * tex->width)*3 + 1], vertexArray[((x+k) + (z+j) * tex->width)*3 + 2]), p);

                }
            }
            //Ta medelvärde av TmpCrossArray för alla x, y, z och lägg in i normalArray.

            normalArray[(x + z * tex->width)*3 + 0] = 0.0;
            normalArray[(x + z * tex->width)*3 + 1] = 1.0;
            normalArray[(x + z * tex->width)*3 + 2] = 0.0;
        }*/

    Point3D p;
    Point3D cross;
    Point3D sum;
    Point3D tmp;
    int index;
    for (x = 1; x < tex->width - 1; x++)
        for (z = 1; z < tex->height - 1; z++)
        {
            //vår punkt!
            index = (x + z * tex->width)*3;
            p = SetVector(vertexArray[index + 0], vertexArray[index + 1], vertexArray[index + 2]);

            index = (x + 1 + z * tex->width) * 3;
            tmp = SetVector(vertexArray[index + 0], vertexArray[index + 1], vertexArray[index + 2]);
            cross = CrossProduct(tmp, p);
            sum = VectorAdd(cross, sum);

            index = (x + 1 + (z - 1) * tex->width) * 3;
            tmp = SetVector(vertexArray[index + 0], vertexArray[index + 1], vertexArray[index + 2]);
            cross = CrossProduct(tmp, p);
            sum = VectorAdd(cross, sum);

            index = (x - 1 + (z - 1) * tex->width) * 3;
            tmp = SetVector(vertexArray[index + 0], vertexArray[index + 1], vertexArray[index + 2]);
            cross = CrossProduct(tmp, p);
            sum = VectorAdd(cross, sum);

            sum = Normalize(sum);
            normalArray[(x + z * tex->width)*3 + 0] = sum.x;
            normalArray[(x + z * tex->width)*3 + 1] = -sum.y;
            normalArray[(x + z * tex->width)*3 + 2] = sum.z;
        }

    // End of terrain generation
    
    // Create Model and upload to GPU:

    Model* model = LoadDataToModel(
            vertexArray,
            normalArray,
            texCoordArray,
            NULL,
            indexArray,
            vertexCount,
            triangleCount*3);

    return model;
}


bool SameSide(Point3D p1, Point3D p2, Point3D a, Point3D b) {
    Point3D sub1, sub2, sub3, result, result2;
    sub1 = VectorSub(b, a);
    sub2 = VectorSub(p1, a);
    sub3 = VectorSub(p2, a);
    result = CrossProduct(sub1, sub2);
    result2 = CrossProduct(sub1, sub3);
    
    if (DotProduct(result, result2) >= 0) {
        return true;
    }

    return false;
}

bool PointInTriangle(Point3D p, Point3D a, Point3D b, Point3D c) {
    if (SameSide(p,a, b,c) && SameSide(p,b, a,c) && SameSide(p,c, a,b)) {
        return true;
    }

    return false;
}

GLfloat findY(int x, int z) {
    GLuint triangle1[3]; 
    GLuint triangle2[3]; 
    GLfloat y = 0.0;

    triangle1[0] = indexArray[(x + z * (texWidth-1))*6 + 0] * 3; //x
    triangle1[1] = indexArray[(x + z * (texWidth-1))*6 + 1] * 3; //y
    triangle1[2] = indexArray[(x + z * (texWidth-1))*6 + 2] * 3; //z
    triangle2[0] = indexArray[(x + z * (texWidth-1))*6 + 3] * 3; //x
    triangle2[1] = indexArray[(x + z * (texWidth-1))*6 + 4] * 3; //y
    triangle2[2] = indexArray[(x + z * (texWidth-1))*6 + 5] * 3; //z

    Point3D p, v1, v2, v3, norm;

    p = SetVector(x, 0, z);
    v1 = SetVector(vertexArray[triangle1[0]], 0, vertexArray[triangle1[0] + 2]);
    v2 = SetVector(vertexArray[triangle1[1]], 0, vertexArray[triangle1[1] + 2]);
    v3 = SetVector(vertexArray[triangle1[2]], 0, vertexArray[triangle1[2] + 2]);

    GLfloat d; 
    if (PointInTriangle(p, v1, v2, v3)) {
        v1 = SetVector(vertexArray[triangle1[0]], vertexArray[triangle1[0] + 1], vertexArray[triangle1[0] + 2]);
        v2 = SetVector(vertexArray[triangle1[1]], vertexArray[triangle1[1] + 1], vertexArray[triangle1[1] + 2]);
        v3 = SetVector(vertexArray[triangle1[2]], vertexArray[triangle1[2] + 1], vertexArray[triangle1[2] + 2]);
    } else {
        v1 = SetVector(vertexArray[triangle2[0]], vertexArray[triangle2[0] + 1], vertexArray[triangle2[0] + 2]);
        v2 = SetVector(vertexArray[triangle2[1]], vertexArray[triangle2[1] + 1], vertexArray[triangle2[1] + 2]);
        v3 = SetVector(vertexArray[triangle2[2]], vertexArray[triangle2[2] + 1], vertexArray[triangle2[2] + 2]);
    }

    norm = CalcNormalVector(v1, v2, v3);
    norm = Normalize(norm);
    // A * x + B * y + C * z - D = 0 => y = (D - A*x - C*z) / B
    d = norm.x * v1.x + norm.y * v1.y + norm.z * v1.z;
    y = (d - norm.x * v1.x - norm.z * v1.z) / norm.y;
    
    return y;
}

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