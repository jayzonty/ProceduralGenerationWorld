#version 330

in float visibility;

uniform vec4 skyColor;
uniform vec4 waterColor;

out vec4 fragColor;

void main()
{
	fragColor = mix(skyColor, waterColor, visibility);
}
