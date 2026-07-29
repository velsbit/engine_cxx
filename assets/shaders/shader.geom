#version 460 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

// Добавляем location = 0
layout(location = 0) out vec3 vBarycentric;

void main()
{
    vBarycentric = vec3(1.0, 0.0, 0.0);
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    vBarycentric = vec3(0.0, 1.0, 0.0);
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    vBarycentric = vec3(0.0, 0.0, 1.0);
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}