#version 330

layout(location = 0) in vec3 vertexPosition;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;

uniform float fogDensity;
uniform float fogGradient;

out float visibility;

void main()
{
	vec4 viewSpacePosition = viewMatrix * vec4(vertexPosition, 1.0);
	gl_Position = projMatrix * viewSpacePosition;
	
	float distance = length(viewSpacePosition.xyz);
	visibility = exp(-pow(distance * fogDensity, fogGradient));
	visibility = clamp(visibility, 0.0, 1.0);
}
