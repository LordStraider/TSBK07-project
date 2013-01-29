#version 150

in  vec3 inPosition;
in  vec3 inNormal;

out vec3 exNormal; 

// NY
uniform mat4 rotationMatrixX;
uniform mat4 rotationMatrixY;
uniform mat4 rotationMatrixZ;
uniform mat4 translationMatrix;
//uniform mat4 translationMatrix2;
uniform mat4 projMatrix;

void main(void)
{
	mat3 normalMatrix = mat3(translationMatrix * rotationMatrixZ * rotationMatrixY * rotationMatrixX); // Cheap normal matrix
	exNormal = normalMatrix * inNormal; // Phong

	gl_Position = projMatrix * translationMatrix * rotationMatrixZ * rotationMatrixY * rotationMatrixX * vec4(inPosition, 1.0);
}
