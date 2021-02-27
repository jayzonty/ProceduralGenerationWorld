#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec2 vertexUV;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;

out vec4 outColor;
out vec2 outUV;

void main()
{
	gl_Position = projMatrix * viewMatrix * vec4(vertexPosition, 1.0);
	outColor = vertexColor;
	outUV = vertexUV;
}
