#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColor;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;

out vec4 outColor;

void main()
{
	gl_Position = projMatrix * viewMatrix * vec4(vertexPosition * 10.0, 1.0);
	outColor = vertexColor;
}
