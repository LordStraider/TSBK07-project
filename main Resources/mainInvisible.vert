#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in  vec2 inTexCoord;
out vec2 texCoord;
out vec3 exPos;
out vec3 exNormal;

uniform mat4 mdlMatrix;
uniform mat4 camMatrix;
uniform mat4 projMatrix;

void main(void)
{
	exPos = inPosition;
	exNormal = inNormal;

	gl_Position = projMatrix * camMatrix * mdlMatrix * vec4(inPosition, 1.0);
}
