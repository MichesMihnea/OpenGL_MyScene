#version 410 core

layout(location=0) in vec3 vPosition;

uniform mat4 lightSpaceTrMatrix;
uniform mat4 modelMatrix;

void main()
{
    gl_Position = lightSpaceTrMatrix * modelMatrix * vec4(vPosition, 1.0f);
}
