#include "core/renderer.hpp"

#include <glad/gl.h>

#include <algorithm>
#include <stdexcept>

namespace gfx {

void init(const win::Window &window) {
  win::make_context_current(window);

  int version = gladLoadGL(reinterpret_cast<GLADloadfunc>(win::get_proc_address));
  if (version == 0) {
    throw std::runtime_error("Failed to initialize GLAD / OpenGL context.");
  }

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void shutdown() {
}

void set_clear_color(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
}

void set_viewport(int x, int y, int width, int height) {
  glViewport(x, y, std::max(0, width), std::max(0, height));
}

void begin_frame() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void end_frame(const win::Window &window) {
  win::swap_buffers(window);
}

} // namespace gfx
