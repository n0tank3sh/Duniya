
#version 1.4

in vec4 pos;
in vec3 aNormal;
in vec3 texCoord;

out vec3 oNormal;
out vec3 otexCoord;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * pos;
    oNormal = aNormal;
    otexCoord = otexCoord;
}
