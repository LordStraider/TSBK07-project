#version 150
 
in vec2 exTexCoord;
in vec3 exNormal; 
in vec3 exPos;
out vec4 outColor;
uniform sampler2D texUnit;
uniform mat4 camMatrix;
uniform mat4 mdlMatrix;
uniform vec3 lightSourcesDirPosArr[8];
uniform vec3 lightSourcesColorArr[8];

vec3 specular(int i){
	vec3 normal = normalize(mat3(camMatrix * mdlMatrix) * exNormal);
	vec3 surf = vec3(camMatrix * mdlMatrix * vec4(exPos,1.0));
	//vec3 normal = normalize(mat3(mdlMatrix) * exNormal);
	//vec3 surf = vec3(mdlMatrix * vec4(exPos,1.0)); 
	
	vec3 lightDirection = normalize(vec3(camMatrix * mdlMatrix * vec4(lightSourcesDirPosArr[i],1.0)));
	//lightDirection = normalize(mat3(mdlMatrix) * camPos);
		
	vec3 reflectedLightDirection = reflect(lightDirection, normal);
	vec3 eyeDirection = normalize(surf);
	
	float specularStrength = dot(reflectedLightDirection, eyeDirection);
	specularStrength = max(specularStrength, 0.01);	//prevent negative light
	specularStrength = pow(specularStrength, 10);

	float diffuseStrength = 0;

	return (specularStrength + diffuseStrength) * lightSourcesColorArr[i];
}

vec3 newSpec(int i){
	vec3 N = normalize(exNormal);
 	vec3 L = normalize(lightSourcesDirPosArr[i]);
    float lambertTerm = dot(N,L);
    float res = 0;
    if (lambertTerm > 0.0)
    {
    	res += 0.5 * lambertTerm;	
    	vec3 eyeVec = vec3(camMatrix * mdlMatrix * vec4(exPos,1.0));
    	vec3 E = normalize(eyeVec);
    	vec3 R = reflect(-L, N);
    	float shininess = 10;
    	float specular = pow(max(dot(R, E), 0.0), shininess);
    	res += specular;	
    }
    return res * lightSourcesColorArr[i];
}

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;
	
	shade = dot(normalize(exNormal), light);
	shade = clamp(shade, 0, 1);
	vec3 diffuse = vec3(shade); //will be unnecessary once positioned lights work as intended

	int i;
	vec3 lights = vec3(0.0,0.0,0.0);
	for(i =  0; i < 8; i++)
		lights += specular(i);

	outColor = vec4(lights,1.0) + vec4(diffuse,1.0) * texture(texUnit, exTexCoord);
}
