#version 330 core

out vec4 FragColor;

in vec3 Color;
in vec2 TexCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.2) * vec4(Color, 1.0);
}
