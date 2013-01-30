#version 150

out vec4 outColor;
in vec3 exNormal; 
in vec3 exNormal2; 

void main(void)
{
	const vec3 light = vec3(0.7, 0.58, 0.88);

	float shade = dot(normalize(exNormal), light);
	float shade2 = dot(normalize(exNormal2), light);	

	outColor = vec4(shade, shade, shade, 1.0) + vec4(shade2, shade2, shade2, 1.0);
}
