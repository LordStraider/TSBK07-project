#include "plane.h"

Plane::Plane(vec3 v1, vec3 v2, vec3 v3) {
	set3Points(v1, v2, v3);
}

void Plane::set3Points(vec3 v1, vec3 v2, vec3 v3) {
	vec3 aux1, aux2;

	aux1 = VectorSub(v1, v2);
	aux2 = VectorSub(v3, v2);

	normal = CrossProduct(aux2, aux1);

	normal = Normalize(normal);
	point = vec3(v2);
	d = DotProduct(normal, point);
	d = -1;
}

void Plane::setNormalAndPoint(vec3 normal, vec3 point) {
	this->normal = Normalize(normal);
	d = DotProduct(this->normal, point);
	d = -1;
}

void Plane::setCoefficients(float a, float b, float c, float d) {
	// set the normal vector
	normal = vec3(a,b,c);
	//compute the lenght of the vector
	float l = Norm(normal);
	// normalize the vector
	normal = vec3(a/l,b/l,c/l);
	// and divide d by th length as well
	this->d = d/l;
}

float Plane::distance(vec3 p) {
//	printf("d: %f, dot: %f, x: %f, y: %f, z: %f\n", d, DotProduct(normal, p), normal.x, normal.y, normal.z);
	return d + DotProduct(normal, p);
}

