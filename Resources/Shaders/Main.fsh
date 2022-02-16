#version 330

struct DirectionalLight
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

// Sunlight (0) and moonlight (1)
uniform DirectionalLight lights[2];
uniform Material material;

uniform vec4 skyColor;

out vec4 fragColor;

void main()
{
	vec3 ambient = vec3(0.0);
	vec3 diffuse = vec3(0.0);

	for (int i = 0; i < 2; ++i)
	{
		ambient += lights[i].ambient * material.ambient;

		vec3 directionToLight = -lights[i].direction;
		float diffuseFactor = max(dot(outNormal, directionToLight), 0);
		diffuse += lights[i].diffuse * (material.diffuse * diffuseFactor);
	}

	ambient /= 2;
	
	vec4 finalColor = outColor * texture(tex, outUV);
	finalColor = vec4(ambient + diffuse * finalColor.rgb, finalColor.a);
	
	fragColor = mix(skyColor, finalColor, visibility);
}
