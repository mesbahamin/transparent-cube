#version 300 es
precision highp float;

in vec4 vertex_color;

out vec4 color;

void main()
{
    color = vertex_color;
}
//#version 300 es
//
//precision highp float;
//
//uniform float alpha;
//
//in vec3 color;
//
//out vec4 frag_color;
//
//void main()
//{
//    frag_color = vec4(color, alpha);
//}
