#version 150
 
in vec2 exTexCoord;
in vec3 exNormal; 
out vec4 outColor;
uniform sampler2D texUnit;


void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;
	
	shade = dot(normalize(exNormal), light);
	shade = clamp(shade, 0, 1);

	outColor = shade * texture(texUnit, exTexCoord);
}
