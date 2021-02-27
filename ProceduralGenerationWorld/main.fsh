#version 330

in vec4 outColor;
in vec2 outUV;

uniform sampler2D tex;

out vec4 fragColor;

void main()
{
	fragColor = outColor * texture(tex, outUV);
}
