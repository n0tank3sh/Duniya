#version 330

in vec3 otexCoord;
in vec3 oNormal;
out vec4 FragColor;

uniform sampler2D image;

uniform float shininess;
uniform vec3 diffuse;
uniform vec3 specular;
uniform vec3 ambient;

void main()
{
	FragColor = texture(image, vec2(otexCoord.x, otexCoord.y));
}
