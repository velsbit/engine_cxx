#pragma once

#include <cstdint>
#include <functional>

namespace win {

inline constexpr uint32_t MAX_WINDOWS = 16;

struct Window {
  uint32_t index = 0;
  uint32_t generation = 0;
};

struct Extent {
  uint32_t width = 0;
  uint32_t height = 0;
};

using ResizeCallback = void (*)(uint32_t width, uint32_t height,
                                void* userData);

void init();
void shutdown();

[[nodiscard]] Window create_window(uint32_t width, uint32_t height,
                                   const char* title);

void destroy_window(Window& window);

[[nodiscard]] bool should_close(const Window& window);

void set_should_close(const Window& window, bool value);

void set_title(const Window& window, const char* title);

void poll_events();

void set_framebuffer_size_callback(const Window& window,
                                   ResizeCallback callback,
                                   void* userData = nullptr);

void make_context_current(const Window& window);

void swap_buffers(const Window& window);

[[nodiscard]] Extent get_window_size(const Window& window);

[[nodiscard]] Extent get_framebuffer_size(const Window& window);

[[nodiscard]] void* get_native_window(const Window& window);

[[nodiscard]] void* get_proc_address(const char* name);
}  // namespace win