#define MAX_SPRITE 50
vec4 spriteCord[MAX_SPRITE];
vec2 uvCord[MAX_SPRITE * 4];


out vec2 uv;
out vec2 panelCord;


void main()
{
	panelCord = (vec2(gl_VertexID >> 1 & 1, gl_VertexID & 1) + spriteCord.xy) * spriteCord.wz;
	uv = uvCord[gl_InstanceID * 4 + gl_VertexID];
}
