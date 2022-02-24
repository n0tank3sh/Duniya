#version 330

in vec4 aPos;
in vec3 aNormal;
in vec2 texCoord;

out vec3 fragPos;
out vec3 oNormal;
out vec2 uv;

uniform mat4 MVP;
uniform vec3 roughness;

void main()
{
  	//gl_Position = MVP * aPos;
	gl_Position = aPos;
	fragPos = gl_Position.xyz;
    oNormal = aNormal;
    uv = texCoord;
}

