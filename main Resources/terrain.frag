#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 exPos;
in vec3 exNormal;
uniform sampler2D tex;
uniform vec3 camPos;
uniform bool mode;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;

float specular(void){
	vec3 normal = normalize(mat3(mdlMatrix) * exNormal);
	vec3 surf = vec3(mdlMatrix * vec4(exPos,1.0)); 
	
	vec3 lightDirection;
	//fix
	lightDirection = normalize(mat3(mdlMatrix) * camPos);
		
	vec3 reflectedLightDirection = reflect(lightDirection, normal);
	vec3 eyeDirection = normalize(surf);
	
	float specularStrength = dot(reflectedLightDirection, eyeDirection);
	specularStrength = max(specularStrength, 0.01);	//prevent negative light
	specularStrength = pow(specularStrength, 10);
	return specularStrength;
}

void main(void)
{
	const vec3 light = vec3(0.58, 1, 0.58);
	float diffuse;
	
//	diffuse = dot(norm, normalize(lightSourcesDirPosArr[i]));
//	shade = max(dot(normalize(exNormal), normalize(camPos)), 0.0);
	diffuse = max(dot(normalize(exNormal), normalize(light)), 0.0);
	diffuse = clamp(diffuse, 0, 1);
	diffuse = diffuse + specular() / exPos.y;
	
	if (exPos.y <= 1.5){ //water
		outColor = exPos.y / 2.5 * (diffuse * 1.2 * texture(tex, texCoord) + diffuse * vec4(0,0,1,1));
	} else {
		outColor = diffuse * texture(tex, texCoord);
	}
}
