#include "engine.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>

#include "core/renderer.hpp"
#include "core/window.hpp"
#include "game.hpp"
#include "input/input.hpp"

void Engine::run() {
  win::init();

  constexpr int width = 640;
  constexpr int height = 480;
  constexpr double fixed_dt = 1.0 / 60.0;

  const win::Window window = win::create_window(width, height, "MyGame");

  inp::init(window);
  gfx::init(window);

  Game game;
  game.init();

  gfx::set_clear_color(0.5f, 0.5f, 0.5f, 1.0f);

  double accumulator = 0.0;
  auto previous_time = std::chrono::high_resolution_clock::now();

  int frames = 0;
  int update_ticks = 0;
  double timer = 0.0;

  double events_time = 0.0;
  double update_time = 0.0;
  double render_time = 0.0;
  double present_time = 0.0;

  while (!win::should_close(window)) {
    const auto current_time = std::chrono::high_resolution_clock::now();

    double frame_time =
        std::chrono::duration<double>(current_time - previous_time).count();

    previous_time = current_time;
    frame_time = std::min(frame_time, 0.05);

    accumulator += frame_time;

    // Input
    auto t0 = std::chrono::high_resolution_clock::now();

    inp::begin_frame();

    auto t1 = std::chrono::high_resolution_clock::now();
    events_time += std::chrono::duration<double, std::milli>(t1 - t0).count();

    // Fixed update
    t0 = std::chrono::high_resolution_clock::now();

    int ticks = 0;

    while (accumulator >= fixed_dt) {
      game.update(static_cast<float>(fixed_dt));
      accumulator -= fixed_dt;

      ++ticks;
      inp::end_frame();
    }

    t1 = std::chrono::high_resolution_clock::now();

    update_time += std::chrono::duration<double, std::milli>(t1 - t0).count();

    update_ticks += ticks;

    // Render
    t0 = std::chrono::high_resolution_clock::now();

    const float alpha =
        std::clamp(static_cast<float>(accumulator / fixed_dt), 0.0f, 1.0f);

    const auto framebuffer = win::get_framebuffer_size(window);
    gfx::set_viewport(0, 0, framebuffer.width, framebuffer.height);

    const float aspect = static_cast<float>(framebuffer.width) /
                         static_cast<float>(framebuffer.height);

    gfx::begin_frame();
    game.render(alpha, aspect);

    t1 = std::chrono::high_resolution_clock::now();

    render_time += std::chrono::duration<double, std::milli>(t1 - t0).count();

    // Present
    t0 = std::chrono::high_resolution_clock::now();

    gfx::end_frame(window);

    t1 = std::chrono::high_resolution_clock::now();

    present_time += std::chrono::duration<double, std::milli>(t1 - t0).count();

    ++frames;
    timer += frame_time;

    if (timer < 1.0) continue;

    const double fps = frames / timer;
    const double frame_ms = timer / frames * 1000.0;

    std::cout << "\n=== PROFILING ===\n"
              << "FPS:     " << static_cast<int>(fps) << " (" << frame_ms
              << " ms)\n"
              << "Events:  " << events_time / frames << " ms\n"
              << "Update:  " << update_time / frames << " ms ("
              << static_cast<double>(update_ticks) / frames << " ticks/frame)\n"
              << "Render:  " << render_time / frames << " ms\n"
              << "Present: " << present_time / frames << " ms\n";

    frames = 0;
    update_ticks = 0;
    timer = 0.0;

    events_time = 0.0;
    update_time = 0.0;
    render_time = 0.0;
    present_time = 0.0;
  }

  gfx::shutdown();
  inp::shutdown();
  win::shutdown();
}