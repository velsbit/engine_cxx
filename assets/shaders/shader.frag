#version 460 core

layout(location = 0) in vec3 vBarycentric;

layout(location = 0) out vec4 FragColor;

void main()
{

    FragColor = vec4(1.0, 0.8118, 0.2, 1.0);
}