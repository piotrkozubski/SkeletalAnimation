#version 440

uniform mat4 mMdvProjMatUnf;
uniform mat4 mModelMatUnf;

in vec4 vPositionAttr;
in vec2 vTexCoordAttr;
in vec3 vNormalAttr;

out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vFragPos; 

void main(void)
{
	gl_Position = mMdvProjMatUnf *  vPositionAttr;
	
	vFragPos = vec3(mModelMatUnf * vPositionAttr);
	vTexCoord = vTexCoordAttr;				
	vNormal = vNormalAttr; 				
}
