#pragma once

#include <cstdint>

#include "gfx/mesh.hpp"

namespace msh {

namespace cfg {

struct Quad {
  float width = 1.0f;
  float height = 1.0f;
};

struct Circle {
  float radius = 0.5f;
  uint32_t segments = 32;
};

struct Star {
  uint32_t points = 5;
  float outer_radius = 0.5f;
  float inner_radius = 0.3f;
};

}  // namespace cfg

gfx::Mesh generate_primitive(const cfg::Quad& config = cfg::Quad{});
gfx::Mesh generate_primitive(const cfg::Circle& config);
gfx::Mesh generate_primitive(const cfg::Star& config);

}  // namespace msh