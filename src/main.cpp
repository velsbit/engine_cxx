#include <algorithm>
#include <chrono>
#include <thread>

#include "core/renderer.hpp"
#include "core/window.hpp"
#include "game.hpp"
#include "input/input.hpp"

using Clock = std::chrono::steady_clock;

namespace {
constexpr double kFixedDt = 1.0 / 60.0;
}

int main() {
  win::init();

  win::Config cfg = {
      .title = "Simple Game",
      .target_fps = 120,
      .vsync = win::VSyncMode::Off,
  };

  const win::Window window = win::create_window(cfg);

  inp::init(window);
  gfx::init(window);
  game::init();
  gfx::set_clear_color(0.5f, 0.5f, 0.5f, 1.0f);

  const auto target_frame_duration =
      cfg.target_fps > 0
          ? std::chrono::duration_cast<Clock::duration>(
                std::chrono::duration<double>(1.0 / cfg.target_fps))
          : Clock::duration::zero();

  double accumulator = 0.0;
  auto previous_frame = Clock::now();

  while (!win::should_close(window)) {
    const auto frame_start = Clock::now();

    const double frame_time =
        std::chrono::duration<double>(frame_start - previous_frame).count();

    previous_frame = frame_start;
    accumulator += frame_time;

    // Input
    inp::begin_frame();

    // Fixed update
    while (accumulator >= kFixedDt) {
      game::update(static_cast<float>(kFixedDt));
      accumulator -= kFixedDt;
    }

    inp::end_frame();

    // Render & Present
    const auto framebuffer = win::get_framebuffer_size(window);
    gfx::set_viewport(0, 0, framebuffer.width, framebuffer.height);

    if (framebuffer.width > 0 && framebuffer.height > 0) {
      const float aspect =
          static_cast<float>(framebuffer.width) /
          static_cast<float>(framebuffer.height);

      const float alpha = static_cast<float>(accumulator / kFixedDt);

      gfx::begin_frame();
      game::render(alpha, aspect);
    }

    gfx::end_frame(window);

    // Frame limiter
    if (target_frame_duration > Clock::duration::zero()) {
      std::this_thread::sleep_until(frame_start + target_frame_duration);
    }
  }

  game::shutdown();
  gfx::shutdown();
  inp::shutdown();
  win::shutdown();

  return 0;
}
