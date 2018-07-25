#version 330 core

uniform vec3 pyramid_color;

out vec4 frag_color;

void main()
{
    frag_color = vec4(pyramid_color, 1.0f);
}
