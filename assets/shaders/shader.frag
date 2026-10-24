#version 460 core

// Принимаем текстурные координаты с тем же layout(location = 0),
// который вы укажете для выхода в вершинном шейдере
layout(location = 0) in vec2 v_TexCoords;

// Выходной цвет пикселя
layout(location = 0) out vec4 FragColor;

// Текстура, привязанная к слоту 0 (через gfx::bind_texture(..., 0))
layout(binding = 0) uniform sampler2D u_Texture;

void main() {
  FragColor = texture(u_Texture, v_TexCoords);
}
