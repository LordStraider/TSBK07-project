#version 150

out vec4 outColor;
in vec3 exNormal0;
in vec3 exNormal1; 

void main(void)
{
	const vec3 light = vec3(0.7, 0.58, 0.88);

	float shade0 = dot(normalize(exNormal0), light);
	float shade1 = dot(normalize(exNormal1), light);	

	outColor = vec4(shade0, shade0, shade0, 1.0);
}
