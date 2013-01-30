#version 150

in  vec3 inPosition;
//in  vec3 inNormal;

in  vec3 inPosition2;
//in  vec3 inNormal2;

//out vec3 exNormal; 
//out vec3 exNormal2;

uniform mat4 rotationMatrixX;
uniform mat4 rotationMatrixY;
uniform mat4 rotationMatrixZ;
uniform mat4 translationMatrix;
//uniform mat4 translationMatrix2;
uniform mat4 projMatrix;

void main(void)
{
	//mat3 normalMatrix = mat3(translationMatrix * rotationMatrixZ * rotationMatrixY * rotationMatrixX); // Cheap normal matrix
	//exNormal = normalMatrix * inNormal;


//	mat3 normalMatrix2 = mat3(translationMatrix * rotationMatrixZ); // Cheap normal matrix
//	exNormal2 = normalMatrix2 * inNormal2;

	gl_Position = projMatrix * translationMatrix * rotationMatrixZ * rotationMatrixY * rotationMatrixX * vec4(inPosition, 1.0)    +    projMatrix * translationMatrix * rotationMatrixZ * vec4(inPosition2, 1.0);
}
