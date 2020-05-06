#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexCoords;

out vec3 normal; 
out vec4 fragmentPositionEyeSpace;
out vec4 fragmentPositionLightSpace;
out vec2 fragmentTextureCoordinates;

out vec4 fragPos;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projection;
uniform mat4 lightSpaceTrMatrix;

void main() 
{
	//compute eye space coordinates
	fragmentPositionEyeSpace = viewMatrix * modelMatrix * vec4(vPosition, 1.0f);
	normal = vNormal;
	fragmentTextureCoordinates = vTexCoords;
	fragmentPositionLightSpace = lightSpaceTrMatrix * modelMatrix * vec4(vPosition, 1.0f);
	gl_Position = projection * viewMatrix * modelMatrix * vec4(vPosition, 1.0f);

	fragPos = modelMatrix * vec4(vPosition, 1.0f);
}
