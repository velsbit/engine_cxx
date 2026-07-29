#include "core/renderer.hpp"

#include <glad/gl.h>

#include <stdexcept>

namespace gfx {

void init(const win::Window& window) {
  win::make_context_current(window);

  if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(win::get_proc_address))) {
    throw std::runtime_error("Failed to initialize GLAD.");
  }

  glEnable(GL_BLEND);
}

void shutdown() {}

void set_clear_color(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
}

void set_viewport(int x, int y, int width, int height) {
  glViewport(x, y, width, height);
}

void begin_frame() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void end_frame(const win::Window& window) { win::swap_buffers(window); }

}  // namespace gfx