#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_norm;
layout(location = 2) in vec3 v_texcoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 offset;
uniform vec3 newColor;
uniform int hp;

out vec3 color;
out float hp_fs;

void main()
{
	color = newColor;
	//float dist = 2000 * distance(v_position, vec3(0,0,0));
	//int d = int(dist);
	vec3 newPos = v_position;
	if (gl_VertexID % hp == 0 || gl_VertexID % hp == hp - 1) {
		newPos = v_position + (0.06 * hp) * v_norm * v_texcoord * vec3(0,1,0) ;
	}
	hp_fs = hp;
	gl_Position = Projection * View * Model * vec4(newPos, 1);
}