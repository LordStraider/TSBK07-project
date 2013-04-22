#include "controller.h"


Model* GenerateTerrain(TextureData *tex)
{
    int vertexCount = tex->width * tex->height;
    int triangleCount = (tex->width-1) * (tex->height-1) * 2;
    int x, z;
    Point3D p;
    Point3D cross;
    Point3D sum;
    Point3D tmp;
    Model* model;
    GLfloat y;
    GLfloat *normalArray, *texCoordArray;

    vertexArray = (GLfloat*)malloc(sizeof(GLfloat) * 3 * vertexCount);
    normalArray = (GLfloat*)malloc(sizeof(GLfloat) * 3 * vertexCount);
    texCoordArray = (GLfloat*)malloc(sizeof(GLfloat) * 2 * vertexCount);
    indexArray = (GLuint*)malloc(sizeof(GLuint) * triangleCount*3);
    //ballY = malloc(sizeof(GLfloat) * 100);

    texWidth = tex->width;

    texHeight = tex->height;


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


    for (x = 1; x < tex->width - 1; x++)
        for (z = 1; z < tex->height - 1; z++)
        {
            int index;
            sum = SetVector(0,0,0);
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

    model = LoadDataToModel(
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
    GLfloat d;
    Point3D p, v1, v2, v3, norm;

    triangle1[0] = indexArray[(x + z * (texWidth-1))*6 + 0] * 3; //x
    triangle1[1] = indexArray[(x + z * (texWidth-1))*6 + 1] * 3; //y
    triangle1[2] = indexArray[(x + z * (texWidth-1))*6 + 2] * 3; //z
    triangle2[0] = indexArray[(x + z * (texWidth-1))*6 + 3] * 3; //x
    triangle2[1] = indexArray[(x + z * (texWidth-1))*6 + 4] * 3; //y
    triangle2[2] = indexArray[(x + z * (texWidth-1))*6 + 5] * 3; //z

    p = SetVector(x, 0, z);
    v1 = SetVector(vertexArray[triangle1[0]], 0, vertexArray[triangle1[0] + 2]);
    v2 = SetVector(vertexArray[triangle1[1]], 0, vertexArray[triangle1[1] + 2]);
    v3 = SetVector(vertexArray[triangle1[2]], 0, vertexArray[triangle1[2] + 2]);

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


bool checkCollisionBB(DrawableObject* obj1, DrawableObject* obj2) {
    vec3 obj1pos = obj1->getCoords();
    vec3 obj2pos = obj2->getCoords();
    vec3 obj1dim = obj1->getDimensons();
    vec3 obj2dim = obj2->getDimensons();

    obj1pos.y = findY(obj1pos.x, obj1pos.z) + obj1->getYoffset();
    obj2pos.y = findY(obj2pos.x, obj2pos.z) + obj2->getYoffset();

    if (obj1pos.x + obj1dim.x > obj2pos.x - obj2dim.x && obj1pos.x < obj2pos.x + obj2dim.x) {
        if (obj1pos.y + obj1dim.y > obj2pos.y && obj1pos.y < obj2pos.y + obj2dim.y) {
            if (obj1pos.z + obj1dim.z > obj2pos.z - obj2dim.z && obj1pos.z < obj2pos.z + obj2dim.z){
                return true;
            }
        }
    }
    return false;
}

bool checkCollisionBS(DrawableObject* obj1, DrawableObject* obj2) {
    vec3 center, closest, result, obj1pos, obj1dim, obj2dim;
    center = obj2->getCoords();
    obj1pos = obj1->getCoords();
    obj1dim = obj1->getDimensons();
    obj2dim = obj2->getDimensons();

    center.y = findY(center.x, center.z) + obj1->getYoffset() - 1;
    obj1pos.y = findY(obj1pos.x, obj1pos.z) + obj2->getYoffset() - 1;

    if(center.x < obj1pos.x - obj1dim.x)
        closest.x = obj1pos.x - obj1dim.x;
    else if(center.x > obj1pos.x + obj1dim.x)
        closest.x = obj1pos.x + obj1dim.x;
    else
        closest.x = center.x;


    if(center.y < obj1pos.y + obj2->getYoffset() - obj1dim.y)
        closest.y = obj1pos.y + obj2->getYoffset() - obj1dim.y;
    else if(center.y > obj1pos.y + obj2->getYoffset() + obj1dim.y)
        closest.y = obj1pos.y + obj2->getYoffset() + obj1dim.y;
    else
        closest.y = center.y;


    if(center.z < obj1pos.z - obj1dim.z)
        closest.z = obj1pos.z - obj1dim.z;
    else if(center.z > obj1pos.z + obj1dim.z)
        closest.z = obj1pos.z + obj1dim.z;
    else
        closest.z = center.z;


    result = VectorSub(closest, center);

    if (sqrt(result.x * result.x + result.y * result.y + result.z * result.z) < obj2dim.x)
        return true;
    return false;
}

bool checkCollisionSS(DrawableObject* obj1, DrawableObject* obj2) {
    Point3D center1, center2, result, obj1dim, obj2dim;
    center1 = obj1->getCoords();
    center2 = obj2->getCoords();
    obj1dim = obj1->getDimensons();
    obj2dim = obj2->getDimensons();

    result = VectorSub(center2, center1);

    if (sqrt(result.x * result.x + result.y * result.y + result.z * result.z) < obj1dim.x + obj2dim.x)
        return true;
    return false;
}

bool checkBoundaries() {
    return checkBoundaries(xValue, zValue);
}

//returns true if OUT of bounds
bool checkBoundaries(int x, int z) {
    return x < 0 || x >= texWidth || z < 0 || z >= texHeight;
}

void keyController(){
    if (gameOver)
        return;

    float rotateFront = 0.0;
    float rotateSide = 0.0;

    xModify = 0.0;
    zModify = 0.0;
    angleMod = 0.0;
    camMod = 0.0;
    bunnyRotation = camPos + M_PI / 2;

    speed = 1.0;


    if (keyIsDown('<')){
        speed = 2.0;
    }

    if (keyIsDown('w')){
        xModify += direction * (-0.2 * cos(camPos));
        zModify += direction * (-0.2 * sin(camPos));
    }
    if (keyIsDown('s')){
        xModify += direction * (0.2 * cos(camPos));
        zModify += direction * (0.2 * sin(camPos));
        rotateFront = direction * (- M_PI);
    }
    if (keyIsDown('a')){
        xModify += direction * (-0.2 * sin(camPos));
        zModify += direction * (0.2 * cos(camPos));
        rotateSide = direction * (- M_PI / 2);
    }
    if (keyIsDown('d')){
        xModify += direction * (0.2 * sin(camPos));
        zModify += direction * (-0.2 * cos(camPos));
        rotateSide = direction * (M_PI / 2);
    }

    if (keyIsDown('w') && keyIsDown('a')){
        bunnyRotation += direction * (- M_PI / 4);
    } else if (keyIsDown('w') && keyIsDown('d')){
        bunnyRotation += direction * (M_PI / 4);
    } else if (keyIsDown('s') && keyIsDown('a')){
        bunnyRotation += direction * (- 3 * M_PI / 4);
    } else if (keyIsDown('s') && keyIsDown('d')){
        bunnyRotation += direction * (3 * M_PI / 4);
    } else {
        bunnyRotation += direction * (rotateFront + rotateSide);
    }

	if(keyIsDown('f')){
		bunnyObj->fireBulletIfAmmo();
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

    if (keyIsDown(' ') && yValue < yFind + 0.705) {
        gravity = -0.18;
        yValue += 0.35;
    }

    if (gravity < 0 && yValue > 0.5) {
        gravity += 0.1;
        yModify -= gravity;
    } else if (yValue > 1.1) {
        gravity += 0.1;
        yModify -= gravity;
    } else {
        yModify = 0;
        gravity = 0;

        if (yFind == 1.5) {
            yValue = 0;
        } else {
            yValue = 0.7;
        }
    }
}
