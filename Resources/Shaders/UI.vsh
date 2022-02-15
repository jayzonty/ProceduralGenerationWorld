#version 330

layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec2 vertexUV;

uniform mat4 viewProjectionMatrix;

out vec4 fragColor;
out vec2 fragUV;

void main()
{
    gl_Position = viewProjectionMatrix * vec4(vertexPosition, 0.0, 1.0);
    
	fragColor = vertexColor;
    fragUV = vertexUV;
}
