#version 150

in  vec3 inPosition0; //Bunny
in  vec3 inNormal0;

in  vec3 inPosition1; //BunnyShadow
in  vec3 inNormal1;

in  vec3 inPosition2; //Klingon
in  vec3 inNormal2;

out vec3 exNormal0; 
out vec3 exNormal1;


uniform mat4 rotationMatrixX;
uniform mat4 rotationMatrixY;
uniform mat4 rotationMatrixZ;
uniform mat4 scalingMatrix;
uniform mat4 translationMatrix;
uniform mat4 projMatrix;

void main(void)
{
	mat3 normalMatrix0 = mat3(translationMatrix * rotationMatrixZ * rotationMatrixY * rotationMatrixX); 
	exNormal0 = normalMatrix0 * inNormal0;


	mat3 normalMatrix1 = mat3(translationMatrix * rotationMatrixZ); 
	exNormal1 = normalMatrix1 * inNormal1;


	gl_Position = projMatrix * translationMatrix * rotationMatrixZ * rotationMatrixY * rotationMatrixX * vec4(inPosition0, 1.0) + projMatrix * translationMatrix * scalingMatrix * rotationMatrixZ * rotationMatrixY * rotationMatrixX * vec4(inPosition1, 1.0) + projMatrix * translationMatrix * rotationMatrixZ * rotationMatrixY * rotationMatrixX * vec4(inPosition2, 1.0);
}
