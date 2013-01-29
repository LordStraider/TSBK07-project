#version 150

in  vec3 inPosition;
in  vec3 inNormal;
out vec3 exNormal; // Phong

// NY
uniform mat4 rotationMatrix;
uniform mat4 rotationMatrix2;
uniform mat4 translationMatrix;
uniform mat4 projMatrix;

void main(void)
{
	mat3 normalMatrix = mat3(translationMatrix * rotationMatrix * rotationMatrix2); // Cheap normal matrix
	exNormal = normalMatrix * inNormal; // Phong

	gl_Position = projMatrix * translationMatrix * rotationMatrix * rotationMatrix2 * vec4(inPosition, 1.0);
}
