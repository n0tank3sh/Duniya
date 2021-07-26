#version 330
#define MAX_LIGHTS 150


in vec3 otexCoord;
in vec3 oNormal;
out vec4 FragColor;

struct Material
{
	uniform vec3 ambient;
	uniform vec3 diffuse;
	uniform vec3 specular;
	uniform float shininess;
};
uniform sampler2D image;

uniform uint numLights;
uniform Light

uniform float shininess;
uniform vec3 diffuse;
uniform vec3 specular;
uniform vec3 ambient;

void main()
{
//	FragColor = texture(image, vec2(otexCoord.x, otexCoord.y));
	vec3 ambientLight = shininess * ambient;
	FragColor = vec4(1, .5, 1, 1);
}
