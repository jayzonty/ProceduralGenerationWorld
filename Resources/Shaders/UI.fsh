#version 330 core

in vec4 fragColor;
in vec2 fragUV;

uniform sampler2D tex0;

out vec4 finalFragColor;

void main()
{
    vec4 texColor = texture(tex0, fragUV);
    finalFragColor = fragColor * texColor;
}
