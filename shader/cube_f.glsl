#version 330 core

uniform vec3 cube_color;

out vec4 frag_color;

void main()
{
    frag_color = vec4(cube_color, 1.0f);
}
