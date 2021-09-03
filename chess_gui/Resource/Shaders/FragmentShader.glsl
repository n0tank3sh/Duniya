#version 330
#define MAX_LIGHTS 150
#define MAX_POINTLIGHT 75
#define MAX_DIRLIGHT 75


in vec3 otexCoord;
in vec3 oNormal;
in vec3 fragPos;
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
	vec3 direction;
};

struct PointLight
{
	vec3 pos;
	float constant, linear, quadratic;
	LightColor lightColor;
};

uniform bool materialProvided;
uniform Material material;
uniform DirectionalLight dirLights[MAX_DIRLIGHT];
uniform PointLight pointLights[MAX_POINTLIGHT];
uniform sampler2D image;

uniform uint numDirLights;
uniform uint numPointLights;

vec3 CalcDirectionalLights(vec3 normal, vec3 viewDir)
{
	vec3 totalLightImpact = vec3(.0, .0, .0);
	for(uint i = 0u; i < numDirLights; i++)
	{
		vec3 lightDir = normalize(-dirLights[i].direction);
		float diff = max(dot(normal, lightDir), .0);
		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), .0), material.shininess);
		vec3 ambient = dirLights[i].lightColor.ambient * material.ambient;
		vec3 specular = dirLights[i].lightColor.specular * spec * material.specular;
		vec3 diffuse = dirLights[i].lightColor.diffuse * diff * material.diffuse;
		totalLightImpact += (ambient + specular + diffuse);

	}
	return totalLightImpact;
}

vec3 CalcPointLights(vec3 normal, vec3 fragPos, vec3 viewDir)
{	
	vec3 totalLightImpact = vec3(.0, .0, .0);
	for(uint i = 0u; i < numPointLights; i++)
	{
		vec3 lightDir = normalize(pointLights[i].pos - fragPos);
		float diff = max(dot(normal, lightDir), .0);
		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), .0), material.shininess);
		vec3 ambient = dirLights[i].lightColor.ambient * material.ambient;
		vec3 specular = dirLights[i].lightColor.specular * spec * material.specular;
		vec3 diffuse = dirLights[i].lightColor.diffuse * diff * material.diffuse;
		float distance = length(pointLights[i].pos - fragPos);
		float attentuation = 1.0/(pointLights[i].constant + pointLights[i].linear * distance + pointLights[i].quadratic * distance * distance);
		totalLightImpact += (ambient + specular + diffuse) * attentuation;

	}
	return totalLightImpact;
}

void main()
{
	//FragColor = texture(image, vec2(otexCoord.x, otexCoord.y));
	//vec3 ambientLight = materialshininess * ambient;
	vec3 col;
	if(materialProvided)
	{
		col += CalcDirectionalLights(oNormal, vec3(0, 0, 0)) ;
		col += CalcPointLights(oNormal, fragPos, vec3(0, 0, 0));
	}
	FragColor = vec4(col, 1);
}
