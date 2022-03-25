#version 330

in vec2 uv;
in vec2 curPos;
in vec4 boxPos;
uniform sampler2D tex;

out vec4 oColor;

void main()
{
	vec2 ch = uv;
	if(curPos.x >= boxPos.x && curPos.x <= (boxPos.z + boxPos.x)
			&& curPos.y >= boxPos.y && curPos.y <= (boxPos.w + boxPos.y)
	  )
		oColor = vec4(0, 1, 0, texture(tex, ch).r);
	else
		oColor = vec4(0);
}
