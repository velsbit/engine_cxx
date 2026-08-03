// src/input/input.cpp

#include "input/input.hpp"

#include "backend/backend.hpp"
#include "device_internal.hpp"

namespace inp {

void init(const win::Window& window) noexcept {
  backend::init();
  backend::set_window(window);
}

void shutdown() noexcept { backend::shutdown(); }

void begin_frame() noexcept { backend::poll_events(); }
void end_frame() noexcept { update_devices(); }

void on_key_event(KeyCode key, bool down) noexcept { set_key(key, down); }

}  // namespace inp