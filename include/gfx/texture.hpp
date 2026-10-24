#pragma once
#include <cstdint>
#include <string>

namespace gfx {

struct Texture {
  uint32_t id;
  int width;
  int height;
  int channels;
};

Texture load_texture(const std::string &filepath);

void destroy_texture(Texture &texture);

void bind_texture(const Texture &texture, uint32_t slot = 0);

} // namespace gfx
