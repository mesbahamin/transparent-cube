#version 300 es

precision highp float;

uniform float alpha;

in vec3 color;

out vec4 frag_color;

void main()
{
    frag_color = vec4(color, alpha);
}
