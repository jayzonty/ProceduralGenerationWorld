#version 330

in vec2 localPos_fsh_in;
in vec4 color_fsh_in;
in vec2 texCoord_fsh_in;

out vec4 fragColor;

uniform float innerRadius;

uniform sampler2D tex0;
uniform float visibility;

void main()
{
	float dist = length(localPos_fsh_in);
	float outerRadius = 1.0;
	float t = smoothstep(innerRadius, outerRadius, dist);
	t = 1.0 - t;

	vec4 sampledColor = texture2D(tex0, texCoord_fsh_in);
	fragColor = vec4(sampledColor.rgb, t * visibility);
}
