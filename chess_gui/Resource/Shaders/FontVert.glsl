#version 330
#define BATCH_LIMIT 10

uniform vec4 uvs[BATCH_LIMIT];
uniform vec4 pos[BATCH_LIMIT];
uniform vec4 boxPositions[BATCH_LIMIT];
//vec4 uvs;
//uniform vec4 pos;


out vec2 uv;
out vec2 curPos;
out vec4 boxPos;

void main()
{
	vec2 vert = vec2((gl_VertexID >> 1) & 1, gl_VertexID & 1);
	boxPos = boxPositions[gl_InstanceID];
	curPos = pos[gl_InstanceID].xy + vert * pos[gl_InstanceID].zw;
	//uv = uvs[gl_InstanceID].xy + vert * uvs[gl_InstanceID].zw;
	uv = uvs[gl_InstanceID].xy + vert * uvs[gl_InstanceID].zw;
	//boxPos = uvs[gl_InstanceID];
	gl_Position = vec4(curPos, 0, 1);
}
