#version 330	
in vec4 inColor;
in vec2 uv;
in float panelCorner;
out vec4 outColor;

void main() {

	vec2 dirs[4] = vec2[4](vec2(-1, -1), vec2(1, -1), vec2(-1, 1), vec2(1, 1));
	vec4 col = inColor;
	vec2 tmp = (uv - 0.5) * 2;
	for(int i = 0; i < 4; i++)
	{
		vec2 dir = dirs[i];
		vec2 dirCorner = dir * panelCorner;
		//float sub = length((tmp - dirCorner)/(dir - dirCorner));
		float sub = length(smoothstep(dirCorner, dir, tmp));
		if(tmp.x >= min(dir.x, dirCorner.x) && tmp.x <= max(dir.x, dirCorner.x)
				&& tmp.y >= min(dir.y, dirCorner.y) && tmp.y <= max(dir.y, dirCorner.y)
				&&   sub > 1 
		  )
		{
			col *= smoothstep(.2, 0, sub);
			break;
		}
	}
	outColor = col;
}
