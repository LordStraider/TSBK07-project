#version 150
 
in vec3 exNormal; 
out vec4 outColor;

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;
	
	shade = dot(normalize(exNormal), light);
	shade = clamp(shade, 0, 1);

	outColor = vec4(1.0, 0.0, 0.0, 1.0) * vec4(shade, shade, shade, 1.0);
}
