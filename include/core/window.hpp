#pragma once

#include <cstdint>
#include <string>

namespace win {

inline constexpr uint32_t MAX_WINDOWS = 16;
inline constexpr uint32_t INVALID_INDEX = UINT32_MAX;

struct Window {
  uint32_t index = INVALID_INDEX;
  uint32_t generation = 0;
};

enum class VSyncMode {
  Off = 0,
  On = 1,
  Adaptive = -1
};

struct Config {
  std::string title = "MyGame";
  int window_width = 640;
  int window_height = 480;
  int target_fps = 60;
  bool fullscreen = false;
  VSyncMode vsync = VSyncMode::Off;
};

struct Extent {
  uint32_t width = 0;
  uint32_t height = 0;
};

using ResizeCallback = void (*)(uint32_t width, uint32_t height, void *userData);

void init();
void shutdown();

[[nodiscard]] Window create_window(const Config &config);
void destroy_window(Window &window);

[[nodiscard]] bool should_close(const Window &window);
void set_should_close(const Window &window, bool value);

void set_title(const Window &window, const char *title);
void apply_config(const Window &window, const Config &settings);

void poll_events();

void set_framebuffer_size_callback(const Window &window,
                                   ResizeCallback callback,
                                   void *userData = nullptr);

void make_context_current(const Window &window);
void set_swap_interval(int interval);
void swap_buffers(const Window &window);

[[nodiscard]] Extent get_window_size(const Window &window);
[[nodiscard]] Extent get_framebuffer_size(const Window &window);

[[nodiscard]] void *get_native_window(const Window &window);
[[nodiscard]] void *get_proc_address(const char *name);

} // namespace win
