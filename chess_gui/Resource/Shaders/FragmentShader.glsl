#version 330 core
#define MAX_LIGHTS 150
#define MAX_POINTLIGHT 75
#define MAX_DIRLIGHT 75


in vec3 fragPos;
in vec3 oNormal;
in vec2 uv;
out vec4 FragColor;


struct LightColor
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
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

uniform vec3 viewPos;
uniform Material material;
uniform DirectionalLight dirLights[MAX_DIRLIGHT];
uniform PointLight pointLights[MAX_POINTLIGHT];
uniform sampler2D tex;

uniform uint numDirLights;
uniform uint numPointLights;

vec3 CalcDirectionalLights(vec3 normal, vec3 viewDir)
{
	vec3 totalLightImpact = vec3(1);
	for(uint i = 0u; i < numDirLights; i++)
	{
		vec3 lightDir = normalize(-dirLights[i].direction);
		vec3 reflectDir = reflect(-lightDir, normal);

		float diff = max(dot(normal, lightDir), 0.0);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

		vec3 ambient = dirLights[i].lightColor.ambient * material.ambient;
		vec3 specular = dirLights[i].lightColor.specular * spec * material.specular;
		vec3 diffuse = dirLights[i].lightColor.diffuse * diff * material.diffuse;

		totalLightImpact *= (ambient + specular + diffuse);

	}
	return totalLightImpact;
}

vec3 CalcPointLights(vec3 normal, vec3 fragPos, vec3 viewDir)
{	
	vec3 totalLightImpact = vec3(1);
	for(uint i = 0u; i < numPointLights; i++)
	{
		vec3 lightDir = normalize(pointLights[i].pos - fragPos);
		vec3 reflectDir = reflect(-lightDir, normal);

		float diff = max(dot(normal, lightDir), .0);
		float spec = pow(max(dot(viewDir, reflectDir), .0), material.shininess);

		vec3 ambient = pointLights[i].lightColor.ambient * material.ambient;
		vec3 specular = pointLights[i].lightColor.specular * spec * material.specular;
		vec3 diffuse = pointLights[i].lightColor.diffuse * diff * material.diffuse;

		float dist = length(pointLights[i].pos - fragPos);
		float attentuation = 1.0/(pointLights[i].constant + pointLights[i].linear * dist + pointLights[i].quadratic * dist * dist);

		totalLightImpact *= (ambient + specular);// + diffuse) * attentuation;

	}
	return totalLightImpact;
}

void main()
{
	vec3 col =  texture(tex, uv).xyz;
	vec3 normal = normalize(oNormal);
	vec3 viewDir = normalize(viewPos - fragPos);
	col *= CalcDirectionalLights(normal, viewDir);
	col *= CalcPointLights(normal, fragPos, viewDir);
	FragColor = vec4(col, texture(tex, uv).w);
}
