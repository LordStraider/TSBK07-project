#include "frustum.h"

#define ANG2RAD 180.0/3.14159265358979323846

bool objectVisible3(const vec3 pos, const GLfloat radius) {

	// calculate the center of the sphere
	frustumCoords = VectorAdd(p, ScalarMult(l, frustumLength * 0.5 - 40));
	frustumCoords.y = 0;

    Point3D result = VectorSub(frustumCoords, pos);

    if (Norm(result) < frustumRadius + radius)
        return true;
    return false;
}
 

bool objectVisible2(const vec3 pos, const GLfloat &radius) {
	vec3 direction = VectorSub(pos, p);
	direction =  Normalize(direction);

	vec3 b = VectorSub(p, l);
	b = Normalize(b);

	GLfloat costheta = DotProduct(direction, b);
	
	float Degrees = acosf(costheta);
	Degrees = Degrees * ANG2RAD;

	if(Degrees > 50)
	    return false;
	return true;

/*	1) Get a vector from the object's position to the camera's position
	2) Normalize it
	3) Get just the view component of your View vector (View - Position)
	4) Because your View and Normalized (View - Player.CamPos) vectors already have a length of 1, you can boil down the equation to just:
	costheta = DotProduct(A, B)
	5) Use arcsine function to get the number of radians between the two vectors
	6) Optional step: convert to degrees for readability (which is what I did)
	7) If the degrees between these two vectors is greater than the radius of your cone, then it isn't visible. 

	Here is my IsVisible() function, if it returns false, the object doesn't get drawn


	bool GameEntity::IsVisible() {
	Vector  ObjectToCamera;
	ObjectToCamera = Position - Player1->CamPos;
	ObjectToCamera.Normalize();
	 
	float   CosTheta = DotProduct(ObjectToCamera, (Player1->View - Player1->Position));
	// / (ObjectToCamera.GetLength() * (Player1->View - Player1->Position).GetLength());
	float   Degrees = acosf(CosTheta);
	Degrees = RadiansToDegrees(Degrees);
	if(Degrees > 50) {
	    return false;
	}*/
}


bool objectVisible(const vec3 pos, const GLfloat &radius) {
	// The equation for a plane is: Ax + By + Cz + D = 0, where A, B and C define the plane's normal vector, D is the distance from the origin to the plane,
	// and x, y and z are any points on the plane.. You can plug any point into the equation and if the result is 0 then the point lies on the plane. If the
	// result is greater than 0 then the point is in front of the plane, and if it's negative the point is behind the plane.
	enum term { A = 0, B, C, D };
 
 	mat4 matrix = Mult(proj, Mult(cam, total));

	GLfloat leftPlane[4];
	leftPlane[A] = matrix[3]  + matrix[0];
	leftPlane[B] = matrix[7]  + matrix[4];
	leftPlane[C] = matrix[11] + matrix[8];
	leftPlane[D] = matrix[15] + matrix[12];

	// Normalise the plane
	GLfloat length = sqrtf(leftPlane[A] * leftPlane[A] + leftPlane[B] * leftPlane[B] + leftPlane[C] * leftPlane[C]);
	leftPlane[A] /= length;
	leftPlane[B] /= length;
	leftPlane[C] /= length;
	leftPlane[D] /= length;
 
	// Check the point's location with respect to the left plane of our viewing frustrum
	GLfloat distance = leftPlane[A] * pos.x + leftPlane[B] * pos.y + leftPlane[C] * pos.z + leftPlane[D];
	if (distance <= -radius) {
		return false; // Bounding sphere is completely outside the left plane
	}
 
	// Check the point's location with respect to the right plane of our viewing frustum
	GLfloat rightPlane[4];
	rightPlane[A] = matrix[3]  - matrix[0];
	rightPlane[B] = matrix[7]  - matrix[4];
	rightPlane[C] = matrix[11] - matrix[8];
	rightPlane[D] = matrix[15] - matrix[12];
 
	// Normalise the plane
	length = sqrtf(rightPlane[A] * rightPlane[A] + rightPlane[B] * rightPlane[B] + rightPlane[C] * rightPlane[C]);
	rightPlane[A] /= length;
	rightPlane[B] /= length;
	rightPlane[C] /= length;
	rightPlane[D] /= length;
 
	distance = rightPlane[A] * pos.x + rightPlane[B] * pos.y + rightPlane[C] * pos.z + rightPlane[D];
	if (distance <= -radius) {
		return false; // Bounding sphere is completely outside the right plane
	}
 
	// Check the point's location with respect to the bottom plane of our viewing frustum
	GLfloat bottomPlane[4];
	bottomPlane[A] = matrix[3]  + matrix[1];
	bottomPlane[B] = matrix[7]  + matrix[5];
	bottomPlane[C] = matrix[11] + matrix[9];
	bottomPlane[D] = matrix[15] + matrix[13];
 
	// Normalise the plane
	length = sqrtf(bottomPlane[A] * bottomPlane[A] + bottomPlane[B] * bottomPlane[B] + bottomPlane[C] * bottomPlane[C]);
	bottomPlane[A] /= length;
	bottomPlane[B] /= length;
	bottomPlane[C] /= length;
	bottomPlane[D] /= length;
 
	distance = bottomPlane[A] * pos.x + bottomPlane[B] * pos.y + bottomPlane[C] * pos.z + bottomPlane[D];
	if (distance <= -radius) {
		return false; // Bounding sphere is completely outside the bottom plane
	}
 
	// Check the point's location with respect to the top plane of our viewing frustrum
	GLfloat topPlane[4];
	topPlane[A] = matrix[3]  - matrix[1];
	topPlane[B] = matrix[7]  - matrix[5];
	topPlane[C] = matrix[11] - matrix[9];
	topPlane[D] = matrix[15] - matrix[13];
 
	// Normalise the plane
	length = sqrtf(topPlane[A] * topPlane[A] + topPlane[B] * topPlane[B] + topPlane[C] * topPlane[C]);
	topPlane[A] /= length;
	topPlane[B] /= length;
	topPlane[C] /= length;
	topPlane[D] /= length;
 
	distance = topPlane[A] * pos.x + topPlane[B] * pos.y + topPlane[C] * pos.z + topPlane[D];
	if (distance <= -radius) {
		return false; // Bounding sphere is completely outside the top plane
	}
 
	// Check the point's location with respect to the near plane of our viewing frustum
	GLfloat nearPlane[4];
	nearPlane[A] = matrix[3]  + matrix[2];
	nearPlane[B] = matrix[7]  + matrix[6];
	nearPlane[C] = matrix[11] + matrix[10];
	nearPlane[D] = matrix[15] + matrix[14];
 
	// Normalise the plane
	length = sqrtf(nearPlane[A] * nearPlane[A] + nearPlane[B] * nearPlane[B] + nearPlane[C] * nearPlane[C]);
	nearPlane[A] /= length;
	nearPlane[B] /= length;
	nearPlane[C] /= length;
	nearPlane[D] /= length;
 
	distance = nearPlane[A] * pos.x + nearPlane[B] * pos.y + nearPlane[C] * pos.z + nearPlane[D];
	if (distance <= -radius) {
		return false; // Bounding sphere is completely outside the near plane
	}
 
	// Check the point's location with respect to the far plane of our viewing frustum
	GLfloat farPlane[4];
	farPlane[A] = matrix[3]  - matrix[2];
	farPlane[B] = matrix[7]  - matrix[6];
	farPlane[C] = matrix[11] - matrix[10];
	farPlane[D] = matrix[15] - matrix[14];
 
	// Normalise the plane
	length = sqrtf(farPlane[A] * farPlane[A] + farPlane[B] * farPlane[B] + farPlane[C] * farPlane[C]);
	farPlane[A] /= length;
	farPlane[B] /= length;
	farPlane[C] /= length;
	farPlane[D] /= length;
 
	distance = farPlane[A] * pos.x + farPlane[B] * pos.y + farPlane[C] * pos.z + farPlane[D];
	if (distance <= -radius) {
		return false; // Bounding sphere is completely outside the far plane
	}
 
	// If we got here, then the bounding sphere is within at least all six sides of the view frustum, so it's visible and we should draw it!
	return true;
}


void FrustumG::setCamInternals(float angle, float ratio, float nearD, float farD) {

	// store the information
	this->ratio = ratio;
	this->angle = angle;
	this->nearD = nearD;
	this->farD = farD;

	// compute width and height of the near and far plane sections
	tang = (float)tan(ANG2RAD * angle * 0.5) ;
	nh = nearD * tang;
	nw = nh * ratio;
	fh = farD  * tang;
	fw = fh * ratio;
}


void FrustumG::setCamDef(vec3 *p, vec3 *l, vec3 *u) {

	vec3 dir,nc,fc,X,Y,Z;

	// compute the Z axis of camera
	// this axis points in the opposite direction from
	// the looking direction
	Z = VectorSub(*p, *l);
	Z = Normalize(Z);

	// X axis of camera with given "up" vector and Z axis
	X = CrossProduct(*u, Z);
	X = Normalize(X);

	// the real "up" vector is the cross product of Z and X
	Y = CrossProduct(Z, X);

	// compute the centers of the near and far planes
	nc = VectorSub(*p, Z * nearD);
	fc = VectorSub(*p, Z * farD);

	// compute the 4 corners of the frustum on the near plane
	ntl = nc + Y * nh - X * nw;
	ntr = nc + Y * nh + X * nw;
	nbl = nc - Y * nh - X * nw;
	nbr = nc - Y * nh + X * nw;

	// compute the 4 corners of the frustum on the far plane
	ftl = fc + Y * fh - X * fw;
	ftr = fc + Y * fh + X * fw;
	fbl = fc - Y * fh - X * fw;
	fbr = fc - Y * fh + X * fw;

	// compute the six planes
	// the function set3Points assumes that the points
	// are given in counter clockwise order
	pl[TOP].set3Points(ntr, ntl, ftl);
	pl[BOTTOM].set3Points(nbl, nbr, fbr);
	pl[LEFT].set3Points(ntl, nbl, fbl);
	pl[RIGHT].set3Points(nbr, ntr, fbr);
	pl[NEARP].set3Points(ntl, ntr, nbr);
	pl[FARP].set3Points(ftr, ftl, fbl);
}


int FrustumG::pointInFrustum(vec3 p) {

	int result = INSIDE;

	for(int i=0; i < 6; i++) {
		if (pl[i].distance(p) < 0)
			return OUTSIDE;
	}
	return result;
}


int FrustumG::sphereInFrustum(vec3 p, float radius) {

	float distance;
	int result = INSIDE;

	for(int i = 0; i < 6; i++) {
		distance = pl[i].distance(p);
		if (distance < -radius)
			return OUTSIDE;
		else if (distance < radius)
			result =  INTERSECT;
	}
	return result;
}


int FrustumG::boxInFrustum(DrawableObject *b) {

	int result = INSIDE;

    vec3 pos = b->getCoords();
    vec3 dim = b->getDimensions();

    pos.y = findY(pos.x, pos.z) + b->getYoffset();

    for (int i = 0; i < 6; i++) {
	    if (pl[i].distance(pos) < 0)
			return OUTSIDE;
		else if (pl[i].distance(pos + dim) < 0)
			result =  INTERSECT;
	}

	return result;
}
