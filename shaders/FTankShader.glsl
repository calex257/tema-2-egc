#version 330

in vec3 color;
in float hp_fs;
layout(location = 0) out vec4 out_color;

void main()
{
	out_color = vec4(color.x * hp_fs/5, color.y, color.z, 1);
}