#version 150
 
in vec2 exTexCoord;
out vec4 outColor;
uniform sampler2D texUnit;


void main(void)
{
	outColor = texture(texUnit, exTexCoord);
}
