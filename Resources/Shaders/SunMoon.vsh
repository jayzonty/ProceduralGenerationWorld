#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec2 vertexUV;

uniform mat4 mvpMatrix;

out vec2 localPos_fsh_in;
out vec4 color_fsh_in;
out vec2 texCoord_fsh_in;

void main()
{
	vec4 finalPosition = mvpMatrix * vec4(vertexPosition, 1.0);
	gl_Position = finalPosition.xyww;

	localPos_fsh_in = vertexPosition.xy;
	color_fsh_in = vertexColor;
	texCoord_fsh_in = vertexUV;
}
