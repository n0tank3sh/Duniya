#version 330

in vec4 pos;
in vec3 aNormal;
in vec3 texCoord;

out vec3 oNormal;
out vec3 otexCoord;
out vec3 fragPos;

uniform mat4 MVP;
uniform vec3 roughness;

void main()
{
  	gl_Position = MVP * pos;
	fragPos = vec3(gl_Position.x, gl_Position.y, gl_Position.z);
    oNormal = aNormal;
    otexCoord = texCoord;
}

