#include <GLFW/glfw3.h>

#include "backend.hpp"
#include "core/window.hpp"
#include "input/input.hpp"

namespace inp::backend {

namespace {

GLFWwindow* g_window = nullptr;

KeyCode translate_key(int key) { return KeyCode{static_cast<uint16_t>(key)}; }

void key_callback(GLFWwindow*, int key, int, int action, int) {
  if (action == GLFW_PRESS) {
    inp::on_key_event(translate_key(key), true);
  } else if (action == GLFW_RELEASE) {
    inp::on_key_event(translate_key(key), false);
  }
}

}  // namespace

bool init() noexcept { return true; }

void shutdown() noexcept { g_window = nullptr; }

void set_window(const win::Window& window) noexcept {
  g_window = static_cast<GLFWwindow*>(win::get_native_window(window));

  glfwSetKeyCallback(g_window, key_callback);
}

void poll_events() noexcept { glfwPollEvents(); }

}  // namespace inp::backend