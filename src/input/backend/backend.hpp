// input/backend/backend.hpp

#pragma once

#include "input/device.hpp"

namespace win {
struct Window;
}

namespace inp::backend {

bool init() noexcept;
void shutdown() noexcept;

void poll_events() noexcept;

void set_window(const win::Window& window) noexcept;

}  // namespace inp::backend