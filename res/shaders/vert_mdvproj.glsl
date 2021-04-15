#version 440

uniform mat4 mMdvProjMatUnf;
in vec4 vPositionAttr;

void main(void)
{
	gl_Position = mMdvProjMatUnf *  vPositionAttr; 				
}
