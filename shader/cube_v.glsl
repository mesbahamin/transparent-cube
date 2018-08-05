#version 300 es

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 color;

void main()
{
    gl_Position = projection * view * model * vec4(a_position, 1.0f);
    color = a_color;
}
