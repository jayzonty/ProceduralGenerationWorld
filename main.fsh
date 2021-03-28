#version 330

in vec4 outColor;
in vec2 outUV;
in float visibility;

uniform sampler2D tex;

uniform vec4 skyColor;

out vec4 fragColor;

void main()
{
	fragColor = outColor * texture(tex, outUV);
	
	fragColor = mix(skyColor, fragColor, visibility);
}
