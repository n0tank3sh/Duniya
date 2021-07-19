#version 330

in vec4 pos;
in vec3 aNormal;
in vec3 texCoord;

out vec3 oNormal;
out vec3 otexCoord;

uniform mat4 MVP;
uniform vec3 roughness;

void main()
{
  	gl_Position = MVP * pos;
    oNormal = aNormal;
    otexCoord = texCoord;
}

