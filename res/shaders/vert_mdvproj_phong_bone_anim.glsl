#version 440

uniform mat4 mMdvProjMatUnf;
uniform mat4 mModelMatUnf;
uniform mat4 mBonesMatUnf[63];

in vec4 vPositionAttr;
in vec2 vTexCoordAttr;
in vec3 vNormalAttr;
in uvec4 vBoneIndexAttr;
in vec4 vBoneWeightAttr;

out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vFragPos; 

void main(void)
{    
	vec4 new_vertex = vec4(0, 0, 0, 0);
	vec4 new_normal = vec4(0, 0, 0, 0);
    if (vBoneIndexAttr.x != 255)
    {
        new_vertex = (mBonesMatUnf[vBoneIndexAttr.x] * vPositionAttr) * vBoneWeightAttr.x;
        new_normal = (mBonesMatUnf[vBoneIndexAttr.x] * vec4(vNormalAttr, 0.0)) * vBoneWeightAttr.x;
    }
	if (vBoneIndexAttr.y != 255)
    {
        new_vertex = new_vertex + (mBonesMatUnf[vBoneIndexAttr.y] * vPositionAttr) * vBoneWeightAttr.y;
        new_normal = new_normal + (mBonesMatUnf[vBoneIndexAttr.y] * vec4(vNormalAttr, 0.0)) * vBoneWeightAttr.y;
    }
    if (vBoneIndexAttr.z != 255)
    {
        new_vertex = new_vertex + (mBonesMatUnf[vBoneIndexAttr.z] * vPositionAttr) * vBoneWeightAttr.z;
        new_normal = new_normal + (mBonesMatUnf[vBoneIndexAttr.z] * vec4(vNormalAttr, 0.0)) * vBoneWeightAttr.z;
    }
    if (vBoneIndexAttr.w != 255)
    {
        new_vertex = new_vertex + (mBonesMatUnf[vBoneIndexAttr.w] * vPositionAttr) * vBoneWeightAttr.w;
        new_normal = new_normal + (mBonesMatUnf[vBoneIndexAttr.w] * vec4(vNormalAttr, 0.0)) * vBoneWeightAttr.w;
    }
    
	if (new_vertex[3] > 1.0) new_vertex[3] = 1.0;
	if (new_vertex[3] < 0.0) new_vertex[3] = 0.0;
	if (new_normal[3] > 1.0) new_normal[3] = 1.0;
	if (new_normal[3] < 0.0) new_normal[3] = 0.0;
	
    gl_Position = mMdvProjMatUnf * new_vertex;
     
    vFragPos = vec3(mModelMatUnf * new_vertex);
    vTexCoord = vTexCoordAttr;
    vNormal = vec3(new_normal);
}
