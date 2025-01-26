#version 330 core

out vec4 frag_color;

in vec3 color;
in vec2 tex_coord;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	frag_color = mix(texture(texture0, tex_coord), texture(texture1, tex_coord), 0.2) * vec4(color, 1.0);
}
