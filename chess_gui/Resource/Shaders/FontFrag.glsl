#version 330

in vec2 uv;
in vec2 curPos;
in vec4 boxPos;
uniform sampler2D tex;

out vec4 oColor;

void main()
{
	vec2 ch = uv;
	//ch.y = 1 - ch.y;
	//if(ch.x >= boxPos.x && ch.x <= (boxPos.z + boxPos.x)
	//		&& ch.y >= boxPos.y && ch.y <= (boxPos.w + boxPos.y)
	//  )
	//	oColor = vec4(0, 1, 0, texture(tex, ch).r);
	//else
		oColor = vec4(1, 0, 0, texture(tex, ch).r);
}
