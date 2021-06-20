#version 1.4

in vec3 otexCoord;
in vec3 oNormal;
out vec4 FragColor;

uniform sampler2D image;

void main()
{
    FragColor = texture(image, vec2(otexCoord.x, otexCoord.y));
}
