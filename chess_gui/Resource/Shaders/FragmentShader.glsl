#version 330
#define MAX_LIGHTS 150
#define MAX_POINTLIGHT 75
#define MAX_DIRLIGHT 75


in vec3 otexCoord;
in vec3 oNormal;
out vec4 FragColor;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};


struct LightColor
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


struct DirectionalLight
{
	LightColor lightColor;
	vec3 directionalLight;
};

struct PointLight
{
	vec3 position;
	LightColor lightColor;
};

uniform bool materialProvided;
uniform Material material;
uniform DirectionalLight dirLights[MAX_DIRLIGHT];
uniform PointLight pointLights[MAX_POINTLIGHT];
uniform sampler2D image;

uniform uint numLights;

void main()
{
	if(materialProvided)
	{
	}
	else
	{
	}
	//FragColor = texture(image, vec2(otexCoord.x, otexCoord.y));
	//vec3 ambientLight = materialshininess * ambient;
	FragColor = vec4(1, .5, 1, 1);
}
