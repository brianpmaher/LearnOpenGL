#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 uTransform;

out vec3 Color;
out vec2 TexCoord;

void main()
{
	gl_Position = uTransform * vec4(aPos, 1.0f);
	TexCoord = aTexCoord;
}
