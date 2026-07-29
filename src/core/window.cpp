#include "core/window.hpp"

#include <GLFW/glfw3.h>

#include <array>
#include <stdexcept>

namespace win {

namespace {

struct WindowSlot {
  GLFWwindow* handle = nullptr;
  uint32_t generation = 1;

  ResizeCallback resizeCallback = nullptr;
  void* resizeUserData = nullptr;
};

std::array<WindowSlot, MAX_WINDOWS> g_windows;

WindowSlot& get_slot(const Window& window) {
  if (window.index >= MAX_WINDOWS)
    throw std::runtime_error("Invalid window handle.");

  WindowSlot& slot = g_windows[window.index];

  if (slot.handle == nullptr || slot.generation != window.generation)
    throw std::runtime_error("Invalid window handle.");

  return slot;
}

}  // namespace

void init() {
  if (glfwInit() != GLFW_TRUE) {
    throw std::runtime_error("Failed to initialize GLFW.");
  }
}

void shutdown() { glfwTerminate(); }

Window create_window(uint32_t width, uint32_t height, const char* title) {
  for (uint32_t i = 0; i < MAX_WINDOWS; ++i) {
    auto& slot = g_windows[i];

    if (slot.handle != nullptr) continue;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* handle =
        glfwCreateWindow(static_cast<int>(width), static_cast<int>(height),
                         title, nullptr, nullptr);

    if (!handle) throw std::runtime_error("Failed to create window.");

    slot.handle = handle;

    glfwSetWindowUserPointer(handle, &slot);

    glfwSetFramebufferSizeCallback(handle, [](GLFWwindow* window, int width,
                                              int height) {
      auto* slot = static_cast<WindowSlot*>(glfwGetWindowUserPointer(window));

      if (slot->resizeCallback) {
        slot->resizeCallback(static_cast<uint32_t>(width),
                             static_cast<uint32_t>(height),
                             slot->resizeUserData);
      }
    });

    return {i, slot.generation};
  }

  throw std::runtime_error("Maximum number of windows reached.");
}

void destroy_window(Window& window) {
  auto& slot = get_slot(window);

  glfwDestroyWindow(slot.handle);

  slot.handle = nullptr;
  slot.resizeCallback = nullptr;
  slot.resizeUserData = nullptr;
  ++slot.generation;

  window = {};
}

bool should_close(const Window& window) {
  return glfwWindowShouldClose(get_slot(window).handle);
}

void set_should_close(const Window& window, bool value) {
  glfwSetWindowShouldClose(get_slot(window).handle,
                           value ? GLFW_TRUE : GLFW_FALSE);
}

void set_title(const Window& window, const char* title) {
  glfwSetWindowTitle(get_slot(window).handle, title);
}

void poll_events() { glfwPollEvents(); }

void set_framebuffer_size_callback(const Window& window,
                                   ResizeCallback callback, void* userData) {
  auto& slot = get_slot(window);

  slot.resizeCallback = callback;
  slot.resizeUserData = userData;
}

void make_context_current(const Window& window) {
  glfwMakeContextCurrent(get_slot(window).handle);
  glfwSwapInterval(0);
}

void swap_buffers(const Window& window) {
  glfwSwapBuffers(get_slot(window).handle);
}

Extent get_window_size(const Window& window) {
  int width;
  int height;

  glfwGetWindowSize(get_slot(window).handle, &width, &height);

  return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
}

Extent get_framebuffer_size(const Window& window) {
  int width;
  int height;

  glfwGetFramebufferSize(get_slot(window).handle, &width, &height);

  return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
}

void* get_native_window(const Window& window) {
  return get_slot(window).handle;
}

void* get_proc_address(const char* name) {
  return reinterpret_cast<void*>(glfwGetProcAddress(name));
}

}  // namespace win