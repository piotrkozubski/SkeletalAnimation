#version 440 core

uniform vec4 vMeshColorUnf;
out vec4 color;

void main(void)
{
    color = vMeshColorUnf;
}
