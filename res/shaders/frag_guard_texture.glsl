#version 440 core

uniform sampler2D iTexUnitUnf;

in vec2 vTexCoord;
in vec3 vVertexLight;
out vec3 color;

void main(void)
{
	vec3 tex = texture(iTexUnitUnf, vTexCoord).rgb;	
    color = tex * vVertexLight;
}
