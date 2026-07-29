// core/input.cpp

#include "core/input.hpp"

#include <GLFW/glfw3.h>

#include <array>
#include <bitset>
#include <cassert>
#include <cstddef>

#include "core/window.hpp"

namespace {

constexpr uint32_t MAX_KEYS_PER_ACTION = 4;
constexpr size_t ACTION_COUNT = static_cast<size_t>(inp::Action::Count);

struct ActionKeyBinding {
  inp::Key keys[MAX_KEYS_PER_ACTION]{};
  uint32_t count = 0;
};

std::array<ActionKeyBinding, ACTION_COUNT> g_key_bindings{};

std::bitset<ACTION_COUNT> g_current_state;
std::bitset<ACTION_COUNT> g_previous_state;
std::bitset<ACTION_COUNT> g_pressed_latch;
std::bitset<ACTION_COUNT> g_released_latch;

GLFWwindow* g_window = nullptr;

int to_glfw_key(inp::Key key) {
  static constexpr std::array<int, static_cast<size_t>(inp::Key::Count)>
      k_key_map = [] {
        std::array<int, static_cast<size_t>(inp::Key::Count)> map{};
        map.fill(GLFW_KEY_UNKNOWN);

        // Words (A-Z)
        map[static_cast<size_t>(inp::Key::A)] = GLFW_KEY_A;
        map[static_cast<size_t>(inp::Key::B)] = GLFW_KEY_B;
        map[static_cast<size_t>(inp::Key::C)] = GLFW_KEY_C;
        map[static_cast<size_t>(inp::Key::D)] = GLFW_KEY_D;
        map[static_cast<size_t>(inp::Key::E)] = GLFW_KEY_E;
        map[static_cast<size_t>(inp::Key::F)] = GLFW_KEY_F;
        map[static_cast<size_t>(inp::Key::G)] = GLFW_KEY_G;
        map[static_cast<size_t>(inp::Key::H)] = GLFW_KEY_H;
        map[static_cast<size_t>(inp::Key::I)] = GLFW_KEY_I;
        map[static_cast<size_t>(inp::Key::J)] = GLFW_KEY_J;
        map[static_cast<size_t>(inp::Key::K)] = GLFW_KEY_K;
        map[static_cast<size_t>(inp::Key::L)] = GLFW_KEY_L;
        map[static_cast<size_t>(inp::Key::M)] = GLFW_KEY_M;
        map[static_cast<size_t>(inp::Key::N)] = GLFW_KEY_N;
        map[static_cast<size_t>(inp::Key::O)] = GLFW_KEY_O;
        map[static_cast<size_t>(inp::Key::P)] = GLFW_KEY_P;
        map[static_cast<size_t>(inp::Key::Q)] = GLFW_KEY_Q;
        map[static_cast<size_t>(inp::Key::R)] = GLFW_KEY_R;
        map[static_cast<size_t>(inp::Key::S)] = GLFW_KEY_S;
        map[static_cast<size_t>(inp::Key::T)] = GLFW_KEY_T;
        map[static_cast<size_t>(inp::Key::U)] = GLFW_KEY_U;
        map[static_cast<size_t>(inp::Key::V)] = GLFW_KEY_V;
        map[static_cast<size_t>(inp::Key::W)] = GLFW_KEY_W;
        map[static_cast<size_t>(inp::Key::X)] = GLFW_KEY_X;
        map[static_cast<size_t>(inp::Key::Y)] = GLFW_KEY_Y;
        map[static_cast<size_t>(inp::Key::Z)] = GLFW_KEY_Z;

        // Numbers
        map[static_cast<size_t>(inp::Key::Num0)] = GLFW_KEY_0;
        map[static_cast<size_t>(inp::Key::Num1)] = GLFW_KEY_1;
        map[static_cast<size_t>(inp::Key::Num2)] = GLFW_KEY_2;
        map[static_cast<size_t>(inp::Key::Num3)] = GLFW_KEY_3;
        map[static_cast<size_t>(inp::Key::Num4)] = GLFW_KEY_4;
        map[static_cast<size_t>(inp::Key::Num5)] = GLFW_KEY_5;
        map[static_cast<size_t>(inp::Key::Num6)] = GLFW_KEY_6;
        map[static_cast<size_t>(inp::Key::Num7)] = GLFW_KEY_7;
        map[static_cast<size_t>(inp::Key::Num8)] = GLFW_KEY_8;
        map[static_cast<size_t>(inp::Key::Num9)] = GLFW_KEY_9;

        // Navigation an control
        map[static_cast<size_t>(inp::Key::Space)] = GLFW_KEY_SPACE;
        map[static_cast<size_t>(inp::Key::Escape)] = GLFW_KEY_ESCAPE;
        map[static_cast<size_t>(inp::Key::Enter)] = GLFW_KEY_ENTER;
        map[static_cast<size_t>(inp::Key::Tab)] = GLFW_KEY_TAB;
        map[static_cast<size_t>(inp::Key::Backspace)] = GLFW_KEY_BACKSPACE;
        map[static_cast<size_t>(inp::Key::Insert)] = GLFW_KEY_INSERT;
        map[static_cast<size_t>(inp::Key::Delete)] = GLFW_KEY_DELETE;
        map[static_cast<size_t>(inp::Key::Right)] = GLFW_KEY_RIGHT;
        map[static_cast<size_t>(inp::Key::Left)] = GLFW_KEY_LEFT;
        map[static_cast<size_t>(inp::Key::Down)] = GLFW_KEY_DOWN;
        map[static_cast<size_t>(inp::Key::Up)] = GLFW_KEY_UP;
        map[static_cast<size_t>(inp::Key::PageUp)] = GLFW_KEY_PAGE_UP;
        map[static_cast<size_t>(inp::Key::PageDown)] = GLFW_KEY_PAGE_DOWN;
        map[static_cast<size_t>(inp::Key::Home)] = GLFW_KEY_HOME;
        map[static_cast<size_t>(inp::Key::End)] = GLFW_KEY_END;
        map[static_cast<size_t>(inp::Key::CapsLock)] = GLFW_KEY_CAPS_LOCK;
        map[static_cast<size_t>(inp::Key::ScrollLock)] = GLFW_KEY_SCROLL_LOCK;
        map[static_cast<size_t>(inp::Key::NumLock)] = GLFW_KEY_NUM_LOCK;
        map[static_cast<size_t>(inp::Key::PrintScreen)] = GLFW_KEY_PRINT_SCREEN;
        map[static_cast<size_t>(inp::Key::Pause)] = GLFW_KEY_PAUSE;

        // Modificators
        map[static_cast<size_t>(inp::Key::LeftShift)] = GLFW_KEY_LEFT_SHIFT;
        map[static_cast<size_t>(inp::Key::LeftControl)] = GLFW_KEY_LEFT_CONTROL;
        map[static_cast<size_t>(inp::Key::LeftAlt)] = GLFW_KEY_LEFT_ALT;
        map[static_cast<size_t>(inp::Key::LeftSuper)] = GLFW_KEY_LEFT_SUPER;
        map[static_cast<size_t>(inp::Key::RightShift)] = GLFW_KEY_RIGHT_SHIFT;
        map[static_cast<size_t>(inp::Key::RightControl)] =
            GLFW_KEY_RIGHT_CONTROL;
        map[static_cast<size_t>(inp::Key::RightAlt)] = GLFW_KEY_RIGHT_ALT;
        map[static_cast<size_t>(inp::Key::RightSuper)] = GLFW_KEY_RIGHT_SUPER;
        map[static_cast<size_t>(inp::Key::Menu)] = GLFW_KEY_MENU;

        // Symbols
        map[static_cast<size_t>(inp::Key::Apostrophe)] =
            GLFW_KEY_APOSTROPHE;                                       // '
        map[static_cast<size_t>(inp::Key::Comma)] = GLFW_KEY_COMMA;    // ,
        map[static_cast<size_t>(inp::Key::Minus)] = GLFW_KEY_MINUS;    // -
        map[static_cast<size_t>(inp::Key::Period)] = GLFW_KEY_PERIOD;  // .
        map[static_cast<size_t>(inp::Key::Slash)] = GLFW_KEY_SLASH;    // /
        map[static_cast<size_t>(inp::Key::Semicolon)] =
            GLFW_KEY_SEMICOLON;                                      // ;
        map[static_cast<size_t>(inp::Key::Equal)] = GLFW_KEY_EQUAL;  // =
        map[static_cast<size_t>(inp::Key::LeftBracket)] =
            GLFW_KEY_LEFT_BRACKET;  // [
        map[static_cast<size_t>(inp::Key::Backslash)] =
            GLFW_KEY_BACKSLASH;  // \
            map[static_cast<size_t>(input::Key::RightBracket)] = GLFW_KEY_RIGHT_BRACKET; // ]
        map[static_cast<size_t>(inp::Key::GraveAccent)] =
            GLFW_KEY_GRAVE_ACCENT;  // `

        // F1 - F25
        map[static_cast<size_t>(inp::Key::F1)] = GLFW_KEY_F1;
        map[static_cast<size_t>(inp::Key::F2)] = GLFW_KEY_F2;
        map[static_cast<size_t>(inp::Key::F3)] = GLFW_KEY_F3;
        map[static_cast<size_t>(inp::Key::F4)] = GLFW_KEY_F4;
        map[static_cast<size_t>(inp::Key::F5)] = GLFW_KEY_F5;
        map[static_cast<size_t>(inp::Key::F6)] = GLFW_KEY_F6;
        map[static_cast<size_t>(inp::Key::F7)] = GLFW_KEY_F7;
        map[static_cast<size_t>(inp::Key::F8)] = GLFW_KEY_F8;
        map[static_cast<size_t>(inp::Key::F9)] = GLFW_KEY_F9;
        map[static_cast<size_t>(inp::Key::F10)] = GLFW_KEY_F10;
        map[static_cast<size_t>(inp::Key::F11)] = GLFW_KEY_F11;
        map[static_cast<size_t>(inp::Key::F12)] = GLFW_KEY_F12;
        map[static_cast<size_t>(inp::Key::F13)] = GLFW_KEY_F13;
        map[static_cast<size_t>(inp::Key::F14)] = GLFW_KEY_F14;
        map[static_cast<size_t>(inp::Key::F15)] = GLFW_KEY_F15;
        map[static_cast<size_t>(inp::Key::F16)] = GLFW_KEY_F16;
        map[static_cast<size_t>(inp::Key::F17)] = GLFW_KEY_F17;
        map[static_cast<size_t>(inp::Key::F18)] = GLFW_KEY_F18;
        map[static_cast<size_t>(inp::Key::F19)] = GLFW_KEY_F19;
        map[static_cast<size_t>(inp::Key::F20)] = GLFW_KEY_F20;
        map[static_cast<size_t>(inp::Key::F21)] = GLFW_KEY_F21;
        map[static_cast<size_t>(inp::Key::F22)] = GLFW_KEY_F22;
        map[static_cast<size_t>(inp::Key::F23)] = GLFW_KEY_F23;
        map[static_cast<size_t>(inp::Key::F24)] = GLFW_KEY_F24;
        map[static_cast<size_t>(inp::Key::F25)] = GLFW_KEY_F25;

        // Numpad
        map[static_cast<size_t>(inp::Key::Kp0)] = GLFW_KEY_KP_0;
        map[static_cast<size_t>(inp::Key::Kp1)] = GLFW_KEY_KP_1;
        map[static_cast<size_t>(inp::Key::Kp2)] = GLFW_KEY_KP_2;
        map[static_cast<size_t>(inp::Key::Kp3)] = GLFW_KEY_KP_3;
        map[static_cast<size_t>(inp::Key::Kp4)] = GLFW_KEY_KP_4;
        map[static_cast<size_t>(inp::Key::Kp5)] = GLFW_KEY_KP_5;
        map[static_cast<size_t>(inp::Key::Kp6)] = GLFW_KEY_KP_6;
        map[static_cast<size_t>(inp::Key::Kp7)] = GLFW_KEY_KP_7;
        map[static_cast<size_t>(inp::Key::Kp8)] = GLFW_KEY_KP_8;
        map[static_cast<size_t>(inp::Key::Kp9)] = GLFW_KEY_KP_9;
        map[static_cast<size_t>(inp::Key::KpDecimal)] = GLFW_KEY_KP_DECIMAL;
        map[static_cast<size_t>(inp::Key::KpDivide)] = GLFW_KEY_KP_DIVIDE;
        map[static_cast<size_t>(inp::Key::KpMultiply)] = GLFW_KEY_KP_MULTIPLY;
        map[static_cast<size_t>(inp::Key::KpSubtract)] = GLFW_KEY_KP_SUBTRACT;
        map[static_cast<size_t>(inp::Key::KpAdd)] = GLFW_KEY_KP_ADD;
        map[static_cast<size_t>(inp::Key::KpEnter)] = GLFW_KEY_KP_ENTER;
        map[static_cast<size_t>(inp::Key::KpEqual)] = GLFW_KEY_KP_EQUAL;

        return map;
      }();

  auto index = static_cast<size_t>(key);
  if (index >= k_key_map.size()) return GLFW_KEY_UNKNOWN;

  return k_key_map[index];
}

}  // namespace

namespace inp {

void init(const win::Window& window) {
  g_window = static_cast<GLFWwindow*>(win::get_native_window(window));

  g_current_state.reset();
  g_previous_state.reset();
  g_pressed_latch.reset();
  g_released_latch.reset();
}

void update() {
  if (!g_window) return;

  g_previous_state = g_current_state;
  g_current_state.reset();

  for (size_t i = 0; i < ACTION_COUNT; ++i) {
    const auto& binding = g_key_bindings[i];

    for (uint32_t k = 0; k < binding.count; ++k) {
      int glfw_key = to_glfw_key(binding.keys[k]);
      if (glfw_key != GLFW_KEY_UNKNOWN &&
          glfwGetKey(g_window, glfw_key) == GLFW_PRESS) {
        g_current_state.set(i);
        break;
      }
    }

    bool curr = g_current_state.test(i);
    bool prev = g_previous_state.test(i);

    if (curr && !prev) g_pressed_latch.set(i);
    if (!curr && prev) g_released_latch.set(i);
  }
}

void post_fixed_update() {
  g_pressed_latch.reset();
  g_released_latch.reset();
}

void shutdown() {
  g_window = nullptr;
  g_key_bindings.fill({});

  g_current_state.reset();
  g_previous_state.reset();
  g_pressed_latch.reset();
  g_released_latch.reset();
}

void bind(Action action, Key key) {
  auto idx = static_cast<size_t>(action);
  assert(idx < ACTION_COUNT && "Action out of bounds!");
  if (idx >= ACTION_COUNT) return;

  auto& binding = g_key_bindings[idx];

  if (binding.count >= MAX_KEYS_PER_ACTION) return;

  for (uint32_t i = 0; i < binding.count; ++i) {
    if (binding.keys[i] == key) return;
  }

  binding.keys[binding.count++] = key;
}

void unbind(Action action, Key key) {
  auto idx = static_cast<size_t>(action);
  assert(idx < ACTION_COUNT && "Action out of bounds!");
  if (idx >= ACTION_COUNT) return;

  auto& binding = g_key_bindings[idx];

  for (uint32_t i = 0; i < binding.count; ++i) {
    if (binding.keys[i] == key) {
      for (uint32_t j = i + 1; j < binding.count; ++j) {
        binding.keys[j - 1] = binding.keys[j];
      }
      binding.keys[--binding.count] = {};
      return;
    }
  }
}

bool down(Action action) {
  auto idx = static_cast<size_t>(action);
  assert(idx < ACTION_COUNT && "Action out of bounds!");
  if (idx >= ACTION_COUNT) return false;

  return g_current_state.test(idx);
}

bool pressed(Action action) {
  auto idx = static_cast<size_t>(action);
  assert(idx < ACTION_COUNT && "Action out of bounds!");
  if (idx >= ACTION_COUNT) return false;

  return g_pressed_latch.test(idx) ||
         (g_current_state.test(idx) && !g_previous_state.test(idx));
}

bool released(Action action) {
  auto idx = static_cast<size_t>(action);
  assert(idx < ACTION_COUNT && "Action out of bounds!");
  if (idx >= ACTION_COUNT) return false;

  return g_released_latch.test(idx) ||
         (!g_current_state.test(idx) && g_previous_state.test(idx));
}

}  // namespace inp