#version 460 core

layout(location = 0) in vec2 aPos;

layout(std140, binding = 0) uniform CameraData
{
    mat4 view_projection;
};

layout(std140, binding = 1) uniform ObjectData
{
    mat4 model;
};

void main()
{
    gl_Position = view_projection * model * vec4(aPos, 0.0, 1.0);
}