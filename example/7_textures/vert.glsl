#version 330 core

layout (location = 0) in vec3 position_attr;
layout (location = 1) in vec3 color_attr;
layout (location = 2) in vec2 tex_coord_attr;

out vec3 color;
out vec2 tex_coord;

void main() {
	gl_Position = vec4(position_attr, 1.0);
	color = color_attr;
	tex_coord = tex_coord_attr;
}
