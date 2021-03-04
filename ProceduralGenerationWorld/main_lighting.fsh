#version 330

struct Light
{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	//vec3 specular;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	//vec3 specular;
	//float shininess;
};

in vec4 outColor;
in vec2 outUV;
in vec3 outNormal;
in float visibility;

uniform sampler2D tex;

uniform Light light;
uniform Material material;

uniform vec4 skyColor;

out vec4 fragColor;

void main()
{
	vec3 ambient = light.ambient * material.ambient;
	
	vec3 directionToLight = -light.direction;
	float diffuseFactor = max(dot(outNormal, directionToLight), 0);
	vec3 diffuse = light.diffuse * (material.diffuse * diffuseFactor);
	
	vec4 finalColor = outColor * texture(tex, outUV);
	finalColor = vec4(ambient + diffuse * finalColor.rgb, finalColor.a);
	
	fragColor = mix(skyColor, finalColor, visibility);
}
