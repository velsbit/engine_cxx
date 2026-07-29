#include "engine.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <vector>

#include "core/input.hpp"
#include "core/renderer.hpp"
#include "core/window.hpp"
#include "engine.hpp"
#include "game.hpp"

void Engine::run() {
  win::init();

  const int initial_width = 640;
  const int initial_height = 480;

  win::Window window =
      win::create_window(initial_width, initial_height, "MyGame");

  gfx::init(window);
  inp::init(window);

  Game game;
  game.init();

  gfx::set_clear_color(0.5f, 0.5f, 0.5f, 1.f);

  constexpr double dt = 1.0 / 60.0;
  double accumulator = 0.0;

  int frame_count = 0;
  double fps_timer = 0.0;

  double t_events_ms = 0.0;
  double t_update_ms = 0.0;
  double t_render_ms = 0.0;
  double t_present_ms = 0.0;
  int update_ticks_count = 0;

  auto previous_time = std::chrono::high_resolution_clock::now();

  while (!win::should_close(window)) {
    auto current_time = std::chrono::high_resolution_clock::now();
    double frame_time =
        std::chrono::duration<double>(current_time - previous_time).count();
    previous_time = current_time;

    if (frame_time > 0.05) {
      frame_time = 0.05;
    }

    accumulator += frame_time;

    // 1. TEST: Event polling delay (Poll Events)
    auto t0 = std::chrono::high_resolution_clock::now();
    win::poll_events();
    auto t1 = std::chrono::high_resolution_clock::now();
    t_events_ms += std::chrono::duration<double, std::milli>(t1 - t0).count();

    // 2. TEST: Update latency (Fixed Updates)
    t0 = std::chrono::high_resolution_clock::now();
    int current_frame_ticks = 0;
    while (accumulator >= dt) {
      inp::update();
      game.update(static_cast<float>(dt));
      inp::post_fixed_update();

      accumulator -= dt;
      current_frame_ticks++;
    }
    t1 = std::chrono::high_resolution_clock::now();
    t_update_ms += std::chrono::duration<double, std::milli>(t1 - t0).count();
    update_ticks_count += current_frame_ticks;

    // 3. TEST: Rendering delay (CPU-side draw calls)
    t0 = std::chrono::high_resolution_clock::now();
    const float alpha =
        std::clamp(static_cast<float>(accumulator / dt), 0.0f, 1.0f);
    auto fb = win::get_framebuffer_size(window);
    gfx::set_viewport(0, 0, fb.width, fb.height);
    float aspect = static_cast<float>(fb.width) / static_cast<float>(fb.height);

    gfx::begin_frame();
    game.render(alpha, aspect);
    t1 = std::chrono::high_resolution_clock::now();
    t_render_ms += std::chrono::duration<double, std::milli>(t1 - t0).count();

    // 4. TEST: Frame presentation / VSync latency (Swap Buffers / Present)
    t0 = std::chrono::high_resolution_clock::now();
    gfx::end_frame(window);
    t1 = std::chrono::high_resolution_clock::now();
    t_present_ms += std::chrono::duration<double, std::milli>(t1 - t0).count();

    frame_count++;
    fps_timer += frame_time;

    if (fps_timer >= 1.0) {
      double avg_fps = frame_count / fps_timer;
      double avg_frame_time = (fps_timer / frame_count) * 1000.0;

      std::cout << "\n=== PROFILING REPORT ===" << std::endl;
      std::cout << "FPS: " << static_cast<int>(avg_fps) << " ("
                << avg_frame_time << " ms/frame)" << std::endl;
      std::cout << "Events:  " << (t_events_ms / frame_count) << " ms"
                << std::endl;
      std::cout << "Update:  " << (t_update_ms / frame_count) << " ms (avg "
                << static_cast<double>(update_ticks_count) / frame_count
                << " ticks/frame)" << std::endl;
      std::cout << "Render:  " << (t_render_ms / frame_count) << " ms"
                << std::endl;
      std::cout << "Present: " << (t_present_ms / frame_count)
                << " ms (Wait GPU/VSync)" << std::endl;

      // Reset counters
      frame_count = 0;
      fps_timer = 0.0;
      t_events_ms = 0.0;
      t_update_ms = 0.0;
      t_render_ms = 0.0;
      t_present_ms = 0.0;
      update_ticks_count = 0;
    }
  }

  inp::shutdown();
  gfx::shutdown();
  win::shutdown();
}