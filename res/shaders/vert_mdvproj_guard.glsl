#version 440

uniform mat4 mMdvProjMatUnf;
uniform mat4 mModelMatUnf;

in vec4 vPositionAttr;
in vec2 vTexCoordAttr;
in vec3 vNormalAttr;

out vec2 vTexCoord;
out vec3 vVertexLight;

void main(void)
{
	gl_Position = mMdvProjMatUnf *  vPositionAttr;
	
	vec3 lightColor = vec3(2.0, 2.0, 2.0);
	vec3 lightPos = vec3(5.0, 5.0, 0);
	vec3 vertexPos = vec3(mModelMatUnf * vPositionAttr);

	vec3 normal = mat3(mModelMatUnf) * vNormalAttr;
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - vertexPos);
	
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;  
	vec3 ambient = vec3(0.3, 0.3, 0.3) * lightColor;
	 
	vVertexLight = ambient + diffuse;
	vTexCoord = vTexCoordAttr;
}
