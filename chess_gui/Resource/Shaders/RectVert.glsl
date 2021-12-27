#version 330																
#define MAX_PANEL 200
uniform vec4 panels[MAX_PANEL];												
uniform vec4 panelColors[MAX_PANEL];
uniform float panelCorners[MAX_PANEL];

out vec4 inColor;
out vec2 uv;
out float panelCorner;
void main() 
{  													
    vec2 tmp;														
    tmp.x = (gl_VertexID & 1);										
    tmp.y = (gl_VertexID >> 1) & 1;								
	uv = tmp;
    tmp = (tmp - 0.5) * 2;	 										
    tmp = panels[gl_InstanceID].xy + tmp * panels[gl_InstanceID].wz; 								
	inColor = panelColors[gl_InstanceID];
	panelCorner = panelCorners[gl_InstanceID];
    gl_Position = vec4(tmp ,0, 1.0);								
}
