#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec3 vertexNormal;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;

uniform float fogDensity;
uniform float fogGradient;

out vec4 outColor;
out vec2 outUV;
out vec3 outNormal;
out float visibility;

void main()
{
	vec4 viewSpacePosition = viewMatrix * vec4(vertexPosition, 1.0);
	gl_Position = projMatrix * viewSpacePosition;
	outColor = vertexColor;
	outUV = vertexUV;
	
	float distance = length(viewSpacePosition.xyz);
	visibility = exp(-pow(distance * fogDensity, fogGradient));
	visibility = clamp(visibility, 0.0, 1.0);
	
	outNormal = vertexNormal;
}
