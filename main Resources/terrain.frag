#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 exPos;
in vec3 exNormal;
uniform sampler2D tex;
uniform sampler2D tex2;
uniform vec3 camPos;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;
uniform vec3 lightSourcesDirPosArr[8];
uniform vec3 lightSourcesColorArr[8];

vec3 specular(int i){
	vec3 normal = normalize(mat3(camMatrix * mdlMatrix) * exNormal);
	vec3 surf = vec3(camMatrix * mdlMatrix * vec4(exPos,1.0));
	//vec3 normal = normalize(mat3(mdlMatrix) * exNormal);
	//vec3 surf = vec3(mdlMatrix * vec4(exPos,1.0)); 
	
	vec3 lightDirection = normalize(mat3(camMatrix) * lightSourcesDirPosArr[i]);
	//lightDirection = normalize(mat3(mdlMatrix) * camPos);
		
	vec3 reflectedLightDirection = reflect(lightDirection, normal);
	vec3 eyeDirection = normalize(surf);
	
	float specularStrength = dot(reflectedLightDirection, eyeDirection);
	specularStrength = max(specularStrength, 0.01);	//prevent negative light
	specularStrength = pow(specularStrength, 10);
	return specularStrength * lightSourcesColorArr[i];
}

void main(void)
{
	const vec3 light = vec3(0.58, 1, 0.58);
	float diffuse;
	
//	diffuse = dot(norm, normalize(lightSourcesDirPosArr[i]));
//	shade = max(dot(normalize(exNormal), normalize(camPos)), 0.0);
	diffuse = max(dot(normalize(exNormal), normalize(light)), 0.0);
	
	vec3 specularLight = vec3(0,0,0);
	int i;
	//for(i = 0; i < 8; i++){
	//	specularLight += specular(i);
	//}
	
	vec3 lights = (specularLight / exPos.y);

	if (exPos.y <= 1.9){ //water
		outColor = vec4(lights,1.0) + max(exPos.y / 2, 0.6) * (diffuse * 1.2 * texture(tex2, texCoord));
	} else {
		outColor = vec4(lights,1.0) + diffuse * texture(tex, texCoord);
	}
}
