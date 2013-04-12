#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 exPos;
in vec3 exNormal; 
uniform sampler2D tex;
uniform vec3 camPos;
uniform bool mode;

void main(void)
{
	const vec3 light = vec3(0.58, 1, 0.58);
	float diffuse;
	
//		diffuse = dot(norm, normalize(lightSourcesDirPosArr[i]));
	//shade = max(dot(normalize(exNormal), normalize(camPos)), 0.0);
	diffuse = max(dot(normalize(exNormal), normalize(light)), 0.0);
	clamp(diffuse, 0, 1);

	if (exPos.y <= 1.5){
		outColor = exPos.y / 2.5 * (diffuse * 1.2 * texture(tex, texCoord) + diffuse * vec4(0,0,1,1));
	} else {
		outColor = diffuse * texture(tex, texCoord);
	}
}
