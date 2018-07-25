#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;

out vec4 vertex_color;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    gl_Position = projection * model * vec4(position, 0.0f, 1.0f);
    vertex_color = vec4(color, 1.0f);
}
