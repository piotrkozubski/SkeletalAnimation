#version 440 core

uniform sampler2D iTexUnitUnf;

in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vFragPos;
out vec3 color;

void main(void)
{
	vec3 lightColor = vec3(2.0, 2.0, 2.0);
	vec3 lightPos = vec3(5.0, 5.0, 0);

	vec3 norm = normalize(vNormal);
	vec3 lightDir = normalize(lightPos - vFragPos);
	
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;  

	vec3 ambient = vec3(0.3, 0.3, 0.3) * lightColor;
	vec3 tex = texture(iTexUnitUnf, vTexCoord).rgb;
	
    color = tex * (ambient + diffuse);
}
