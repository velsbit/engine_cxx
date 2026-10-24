#include "core/window.hpp"

#include <GLFW/glfw3.h>

#include <array>
#include <stdexcept>

namespace win {

namespace {

struct WindowSlot {
  GLFWwindow *handle = nullptr;
  uint32_t generation = 1;

  ResizeCallback resizeCallback = nullptr;
  void *resizeUserData = nullptr;
};

std::array<WindowSlot, MAX_WINDOWS> g_windows{};

WindowSlot &get_slot(const Window &window) {
  if (window.index >= MAX_WINDOWS) {
    throw std::runtime_error("Window index out of range.");
  }

  WindowSlot &slot = g_windows[window.index];

  if (slot.handle == nullptr || slot.generation != window.generation) {
    throw std::runtime_error("Attempted to access an invalid or destroyed window handle.");
  }

  return slot;
}

} // namespace

void init() {
  if (glfwInit() != GLFW_TRUE) {
    throw std::runtime_error("Failed to initialize GLFW.");
  }
}

void shutdown() {
  glfwTerminate();
}

Window create_window(const Config &config) {
  uint32_t slot_index = MAX_WINDOWS;
  for (uint32_t i = 0; i < MAX_WINDOWS; ++i) {
    if (g_windows[i].handle == nullptr) {
      slot_index = i;
      break;
    }
  }

  if (slot_index == MAX_WINDOWS) {
    throw std::runtime_error("Maximum number of windows reached.");
  }

  auto &slot = g_windows[slot_index];

  GLFWwindow *handle = glfwCreateWindow(
      static_cast<int>(config.window_width),
      static_cast<int>(config.window_height),
      config.title.c_str(),
      nullptr,
      nullptr);

  if (!handle) {
    throw std::runtime_error("Failed to create GLFW window.");
  }

  slot.handle = handle;
  slot.resizeCallback = nullptr;
  slot.resizeUserData = nullptr;

  glfwSetWindowUserPointer(handle, &slot);

  glfwSetFramebufferSizeCallback(handle, [](GLFWwindow *window, int w, int h) {
    auto *s = static_cast<WindowSlot *>(glfwGetWindowUserPointer(window));
    if (s && s->resizeCallback) {
      s->resizeCallback(
          static_cast<uint32_t>(std::max(0, w)),
          static_cast<uint32_t>(std::max(0, h)),
          s->resizeUserData);
    }
  });

  set_swap_interval(config.vsync == VSyncMode::On ? 1 : config.vsync == VSyncMode::Adaptive ? -1
                                                                                            : 0);

  return {slot_index, slot.generation};
}

void destroy_window(Window &window) {
  auto &slot = get_slot(window);

  glfwDestroyWindow(slot.handle);

  slot.handle = nullptr;
  slot.resizeCallback = nullptr;
  slot.resizeUserData = nullptr;

  ++slot.generation;

  window = {};
}

bool should_close(const Window &window) {
  return glfwWindowShouldClose(get_slot(window).handle) != 0;
}

void set_should_close(const Window &window, bool value) {
  glfwSetWindowShouldClose(get_slot(window).handle, value ? GLFW_TRUE : GLFW_FALSE);
}

void set_title(const Window &window, const char *title) {
  glfwSetWindowTitle(get_slot(window).handle, title);
}

void apply_config(const Window &window, const Config &settings) {
  auto &slot = get_slot(window);

  if (settings.fullscreen) {
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    glfwSetWindowMonitor(slot.handle, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
  } else {
    glfwSetWindowMonitor(slot.handle, nullptr, 100, 100, settings.window_width, settings.window_height, 0);
  }

  set_swap_interval(settings.vsync == VSyncMode::On ? 1 : (settings.vsync == VSyncMode::Adaptive ? -1 : 0));
}

void poll_events() {
  glfwPollEvents();
}

void set_framebuffer_size_callback(const Window &window, ResizeCallback callback, void *userData) {
  auto &slot = get_slot(window);
  slot.resizeCallback = callback;
  slot.resizeUserData = userData;
}

void make_context_current(const Window &window) {
  glfwMakeContextCurrent(get_slot(window).handle);
}

void set_swap_interval(int interval) {
  glfwSwapInterval(interval);
}

void swap_buffers(const Window &window) {
  glfwSwapBuffers(get_slot(window).handle);
}

Extent get_window_size(const Window &window) {
  int width = 0;
  int height = 0;
  glfwGetWindowSize(get_slot(window).handle, &width, &height);
  return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
}

Extent get_framebuffer_size(const Window &window) {
  int width = 0;
  int height = 0;
  glfwGetFramebufferSize(get_slot(window).handle, &width, &height);
  return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
}

void *get_native_window(const Window &window) {
  return get_slot(window).handle;
}

void *get_proc_address(const char *name) {
  return reinterpret_cast<void *>(glfwGetProcAddress(name));
}

} // namespace win
