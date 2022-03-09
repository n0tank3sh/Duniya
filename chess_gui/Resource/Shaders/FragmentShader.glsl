#version 330 core
#define MAX_LIGHTS 150
#define MAX_POINTLIGHT 75
#define MAX_DIRLIGHT 75


in vec3 fragPos;
in vec3 oNormal;
in vec2 uv;
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
uniform sampler2D tex;

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
		float dist = length(pointLights[i].pos - fragPos);
		float attentuation = 1.0/(pointLights[i].constant + pointLights[i].linear * dist + pointLights[i].quadratic * dist * dist);
		totalLightImpact += (ambient + specular + diffuse) * attentuation;

	}
	return totalLightImpact;
}

void main()
{
	//FragColor = texture(image, vec2(otexCoord.x, otexCoord.y));
	//vec3 ambientLight = materialshininess * ambient;
	vec4 col = texture(tex, uv);
	//	col += CalcDirectionalLights(oNormal, vec3(0, 0, 0)) ;
	//	col += CalcPointLights(oNormal, pos.xyz, vec3(0, 0, 0));
	FragColor = col;
}
