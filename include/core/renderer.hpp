#pragma once

#include "core/window.hpp"

namespace gfx {

void init(const win::Window& window);
void shutdown();

void set_clear_color(float r, float g, float b, float a);
void set_viewport(int x, int y, int width, int height);

void begin_frame();
void end_frame(const win::Window& window);

}  // namespace gfx