#define STB_IMAGE_IMPLEMENTATION
#include "gfx/texture.hpp"
#include <glad/gl.h>
#include <iostream>
#include <stb_image.h>

namespace gfx {

Texture load_texture(const std::string &filepath) {
  Texture texture{};

  stbi_set_flip_vertically_on_load(true);

  unsigned char *data = stbi_load(filepath.c_str(), &texture.width, &texture.height, &texture.channels, 0);

  if (!data) {
    std::cerr << "Failed to load texture: " << filepath << " (" << stbi_failure_reason() << ")" << std::endl;
    return texture;
  }

  glGenTextures(1, &texture.id);
  glBindTexture(GL_TEXTURE_2D, texture.id);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  GLenum format = (texture.channels == 4) ? GL_RGBA : GL_RGB;

  glTexImage2D(GL_TEXTURE_2D, 0, format, texture.width, texture.height, 0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_image_free(data);

  return texture;
}

void destroy_texture(Texture &texture) {
  if (texture.id) {
    glDeleteTextures(1, &texture.id);
    texture.id = 0;
  }
}

void bind_texture(const Texture &texture, uint32_t slot) {
  glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(slot));
  glBindTexture(GL_TEXTURE_2D, texture.id);
}

} // namespace gfx
