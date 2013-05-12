#version 150
 
in vec2 exTexCoord;
in vec3 exNormal; 
in vec3 exPos;
out vec4 outColor;
uniform sampler2D texUnit;
uniform mat4 camMatrix;
uniform mat4 mdlMatrix;
uniform vec3 lightSourcesDirArr[8];
uniform vec3 lightSourcesColorArr[8];
uniform vec3 lightSourcesPosArr[8];

vec3 specular(int i){
	mat4 viewMatrix = camMatrix * mdlMatrix;

	vec3 normal = normalize(mat3(mdlMatrix) * exNormal);

	vec3 surf = normalize(vec3(mdlMatrix * vec4(exPos,1.0)));
	
	vec3 lightPosition = normalize(vec3(camMatrix * vec4(lightSourcesPosArr[i],1.0)));
	
	vec3 spotDirection = normalize(vec3(camMatrix * vec4(lightSourcesDirArr[i],1.0)));
	
	vec3 lightDirection = normalize(surf);

	float angle = max(0,dot(spotDirection, lightDirection));

	vec3 reflectedLightDirection = reflect(spotDirection, normal);

	vec3 eyeDirection = normalize(surf);
	
	float specularStrength = 0;

	//if(1 || acos(angle) > 1.5)
	//{
		specularStrength = dot(reflectedLightDirection, eyeDirection);
		specularStrength = max(specularStrength, 0.01);	//prevent negative light
		specularStrength = pow(specularStrength, 10);
	//}
	float diffuseStrength = 0;

	return (specularStrength + diffuseStrength) * lightSourcesColorArr[i];
}

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;
	
	shade = dot(normalize(mat3(camMatrix * mdlMatrix) * exNormal), light);
	shade = clamp(shade, 0, 1);
	vec3 diffuse = vec3(shade); //will be unnecessary once positioned lights work as intended

	int i;
	vec3 lights = vec3(0.0,0.0,0.0);
	//for(i =  0; i < 1; i++)
	//	lights += specular(i);

	outColor = vec4(lights,1.0) + vec4(diffuse,1.0) * texture(texUnit, exTexCoord);
}
