// core/input.cpp

#include "core/input.hpp"

#include <GLFW/glfw3.h>

#include <algorithm>
#include <array>
#include <unordered_map>
#include <vector>

#include "core/window.hpp"

namespace inp {

enum class KeyFlags : uint8_t {
  None = 0,
  Down = 1 << 0,
  Pressed = 1 << 1,
  Released = 1 << 2,
  Repeated = 1 << 3,
};

struct ScanBinding {
  ScanCode scancode = invalidScanCode;
  Modifier modifier = Modifier::None;
};

struct KeyBinding {
  KeyCode keycode = invalidKeyCode;
  Modifier modifier = Modifier::None;
};

struct ActionState {
  KeyFlags flags = KeyFlags::None;
  std::vector<KeyBinding> keyBindings;
  std::vector<ScanBinding> scanBindings;
};

struct ActionLookup {
  Action action;
  Modifier modifier;
};

constexpr KeyFlags operator|(KeyFlags lhs, KeyFlags rhs) noexcept {
  return static_cast<KeyFlags>(static_cast<uint8_t>(lhs) |
                               static_cast<uint8_t>(rhs));
}

constexpr KeyFlags operator&(KeyFlags lhs, KeyFlags rhs) noexcept {
  return static_cast<KeyFlags>(static_cast<uint8_t>(lhs) &
                               static_cast<uint8_t>(rhs));
}

constexpr KeyFlags operator~(KeyFlags value) noexcept {
  return static_cast<KeyFlags>(~static_cast<uint8_t>(value));
}

constexpr KeyFlags& operator|=(KeyFlags& lhs, KeyFlags rhs) noexcept {
  return lhs = lhs | rhs;
}

constexpr KeyFlags& operator&=(KeyFlags& lhs, KeyFlags rhs) noexcept {
  return lhs = lhs & rhs;
}

constexpr bool has_flag(KeyFlags value, KeyFlags flag) noexcept {
  return (value & flag) != KeyFlags::None;
}

struct InputContext {
  GLFWwindow* window = nullptr;

  std::vector<ActionState> actionStates;
  std::unordered_map<std::string_view, Action> actionLookup;

  std::array<std::vector<ActionLookup>, GLFW_KEY_LAST + 1> keyMap;
  std::unordered_map<ScanCode, std::vector<ActionLookup>> scanMap;
};

static InputContext g_context;

static void update_state(KeyFlags& state, int action) {
  switch (action) {
    case GLFW_PRESS:
      state &= ~KeyFlags::Released;

      if (!has_flag(state, KeyFlags::Down)) state |= KeyFlags::Pressed;

      state |= KeyFlags::Down;
      break;

    case GLFW_RELEASE:
      state &= ~(KeyFlags::Down | KeyFlags::Repeated);
      state |= KeyFlags::Released;
      break;

    case GLFW_REPEAT:
      state &= ~KeyFlags::Released;
      state |= KeyFlags::Down | KeyFlags::Repeated;
      break;
  }
}

bool modifier_matches(Modifier binding, Modifier current) {
  if (has_flag(binding, Modifier::Any)) return true;
  return (current & binding) == binding;
}

// GLFW callback
static void keyCallback(GLFWwindow*, int key, int scan, int action, int mods) {
  Modifier current = static_cast<Modifier>(mods);

  auto process_lookups = [&](const std::vector<ActionLookup>& lookups) {
    for (const auto& lookup : lookups) {
      if (modifier_matches(lookup.modifier, current)) {
        update_state(g_context.actionStates[lookup.action].flags, action);
      }
    }
  };

  if (key >= 0 && key <= GLFW_KEY_LAST) {
    process_lookups(g_context.keyMap[key]);
  }

  if (scan != static_cast<int>(invalidScanCode)) {
    auto sc = static_cast<ScanCode>(scan);
    if (auto it = g_context.scanMap.find(sc); it != g_context.scanMap.end()) {
      process_lookups(it->second);
    }
  }
}

void init(const win::Window& window) {
  g_context.window = static_cast<GLFWwindow*>(win::get_native_window(window));
  clear_actions();

  if (g_context.window) {
    glfwSetKeyCallback(g_context.window, keyCallback);
  }
}

void shutdown() {
  if (g_context.window) {
    glfwSetKeyCallback(g_context.window, nullptr);
    g_context.window = nullptr;
  }

  clear_actions();
}

//------------------------------------------------------------------------------
// Actions
//------------------------------------------------------------------------------

Action create_action(std::string_view name) {
  if (auto it = g_context.actionLookup.find(name);
      it != g_context.actionLookup.end()) {
    return it->second;
  }

  Action id = static_cast<Action>(g_context.actionStates.size());

  g_context.actionStates.emplace_back();
  g_context.actionLookup.emplace(name, id);

  return id;
}

void clear_action(Action action) {
  if (!has_action(action)) return;

  unbind_all(action);
  g_context.actionStates[action].flags = KeyFlags::None;

  for (auto it = g_context.actionLookup.begin();
       it != g_context.actionLookup.end(); ++it) {
    if (it->second == action) {
      g_context.actionLookup.erase(it);
      break;
    }
  }
}

void clear_action(std::string_view name) {
  Action act = find_action(name);
  if (act != invalidAction) {
    clear_action(act);
  }
}

bool has_action(Action action) {
  return action < g_context.actionStates.size();
}

bool has_action(std::string_view name) {
  return find_action(name) != invalidAction;
}

Action find_action(std::string_view name) {
  auto it = g_context.actionLookup.find(name);
  return it == g_context.actionLookup.end() ? invalidAction : it->second;
}

std::string_view action_name(Action action) {
  if (!has_action(action)) return {};

  for (const auto& [name, id] : g_context.actionLookup) {
    if (id == action) return name;
  }

  return {};
}

//------------------------------------------------------------------------------
// Key / Scan code conversion
//------------------------------------------------------------------------------

ScanCode scancode_from_string(std::string_view name) {
  for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key) {
    const char* keyName = glfwGetKeyName(key, 0);
    if (keyName && name == keyName) {
      return static_cast<ScanCode>(glfwGetKeyScancode(key));
    }
  }
  return invalidScanCode;
}

std::string_view scancode_to_string(ScanCode scancode) {
  const char* name =
      glfwGetKeyName(GLFW_KEY_UNKNOWN, static_cast<int>(scancode));
  return name ? std::string_view{name} : std::string_view{};
}

KeyCode keycode_from_string(std::string_view name) {
  for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key) {
    const char* keyName = glfwGetKeyName(key, 0);
    if (keyName && name == keyName) {
      return static_cast<KeyCode>(key);
    }
  }
  return invalidKeyCode;
}

std::string_view keycode_to_string(KeyCode keycode) {
  if (!is_valid_keycode(keycode)) return {};

  const char* name = glfwGetKeyName(static_cast<int>(keycode), 0);
  return name ? std::string_view{name} : std::string_view{};
}

KeyCode keycode_from_scancode(ScanCode scancode) {
  for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key) {
    if (glfwGetKeyScancode(key) == static_cast<int>(scancode))
      return static_cast<KeyCode>(key);
  }

  return invalidKeyCode;
}

ScanCode scancode_from_keycode(KeyCode keycode) {
  if (!is_valid_keycode(keycode)) return invalidScanCode;

  return static_cast<ScanCode>(glfwGetKeyScancode(static_cast<int>(keycode)));
}

[[nodiscard]] bool is_valid_scancode(ScanCode scancode) {
  return scancode != invalidScanCode;
}

[[nodiscard]] bool is_valid_keycode(KeyCode keycode) {
  return keycode != invalidKeyCode &&
         static_cast<int>(keycode) <= GLFW_KEY_LAST;
}

//------------------------------------------------------------------------------
// Bindings
//------------------------------------------------------------------------------

void bind_scancode(Action action, ScanCode scancode, Modifier modifier) {
  if (!has_action(action) || !is_valid_scancode(scancode)) return;

  auto& bindings = g_context.actionStates[action].scanBindings;
  for (const auto& b : bindings) {
    if (b.scancode == scancode && b.modifier == modifier) return;
  }

  bindings.push_back({scancode, modifier});
  g_context.scanMap[scancode].push_back({action, modifier});
}

void bind_scancode(Action action, std::string_view name, Modifier modifier) {
  bind_scancode(action, scancode_from_string(name), modifier);
}

void unbind_scancode(Action action, ScanCode scancode, Modifier modifier) {
  if (!has_action(action) || !is_valid_scancode(scancode)) return;

  if (auto it = g_context.scanMap.find(scancode);
      it != g_context.scanMap.end()) {
    std::erase_if(it->second, [&](const ActionLookup& lookup) {
      return lookup.action == action && lookup.modifier == modifier;
    });
  }

  std::erase_if(g_context.actionStates[action].scanBindings,
                [&](const ScanBinding& b) {
                  return b.scancode == scancode && b.modifier == modifier;
                });
}

void unbind_scancode(Action action, std::string_view name, Modifier modifier) {
  unbind_scancode(action, scancode_from_string(name), modifier);
}

void bind_keycode(Action action, KeyCode keycode, Modifier modifier) {
  if (!has_action(action) || !is_valid_keycode(keycode)) return;

  auto& bindings = g_context.actionStates[action].keyBindings;
  for (const auto& b : bindings) {
    if (b.keycode == keycode && b.modifier == modifier) return;
  }

  bindings.push_back({keycode, modifier});
  g_context.keyMap[static_cast<size_t>(keycode)].push_back({action, modifier});
}

void bind_keycode(Action action, std::string_view name, Modifier modifier) {
  bind_keycode(action, keycode_from_string(name), modifier);
}

void unbind_keycode(Action action, KeyCode keycode, Modifier modifier) {
  if (!has_action(action) || !is_valid_keycode(keycode)) return;

  auto& map = g_context.keyMap[static_cast<size_t>(keycode)];
  std::erase_if(map, [&](const ActionLookup& lookup) {
    return lookup.action == action && lookup.modifier == modifier;
  });

  std::erase_if(g_context.actionStates[action].keyBindings,
                [&](const KeyBinding& b) {
                  return b.keycode == keycode && b.modifier == modifier;
                });
}

void unbind_keycode(Action action, std::string_view name, Modifier modifier) {
  unbind_keycode(action, keycode_from_string(name), modifier);
}

void unbind_all(Action action) {
  if (!has_action(action)) return;

  auto& state = g_context.actionStates[action];

  for (const auto& binding : state.keyBindings) {
    if (is_valid_keycode(binding.keycode)) {
      auto& map = g_context.keyMap[static_cast<size_t>(binding.keycode)];
      std::erase_if(map, [&](const ActionLookup& lookup) {
        return lookup.action == action && lookup.modifier == binding.modifier;
      });
    }
  }

  for (const auto& binding : state.scanBindings) {
    if (is_valid_scancode(binding.scancode)) {
      if (auto it = g_context.scanMap.find(binding.scancode);
          it != g_context.scanMap.end()) {
        std::erase_if(it->second, [&](const ActionLookup& lookup) {
          return lookup.action == action && lookup.modifier == binding.modifier;
        });
      }
    }
  }

  state.keyBindings.clear();
  state.scanBindings.clear();
}

bool is_scancode_bound(Action action, ScanCode scancode, Modifier modifier) {
  if (!has_action(action)) return false;

  const auto& bindings = g_context.actionStates[action].scanBindings;
  return std::any_of(bindings.begin(), bindings.end(),
                     [&](const ScanBinding& b) {
                       return b.scancode == scancode && b.modifier == modifier;
                     });
}

bool is_keycode_bound(Action action, KeyCode keycode, Modifier modifier) {
  if (!has_action(action)) return false;

  const auto& bindings = g_context.actionStates[action].keyBindings;
  return std::any_of(bindings.begin(), bindings.end(),
                     [&](const KeyBinding& b) {
                       return b.keycode == keycode && b.modifier == modifier;
                     });
}

std::vector<ScanBinding> scancode_bindings(Action action) {
  if (!has_action(action)) return {};
  return g_context.actionStates[action].scanBindings;
}

std::vector<KeyBinding> keycode_bindings(Action action) {
  if (!has_action(action)) return {};
  return g_context.actionStates[action].keyBindings;
}

//------------------------------------------------------------------------------
// Input state
//------------------------------------------------------------------------------

void update() {
  for (auto& state : g_context.actionStates) {
    state.flags &=
        ~(KeyFlags::Pressed | KeyFlags::Released | KeyFlags::Repeated);
  }
}

static bool check_flag(Action action, KeyFlags flag) {
  if (!has_action(action)) return false;
  return has_flag(g_context.actionStates[action].flags, flag);
}

bool down(Action action) { return check_flag(action, KeyFlags::Down); }
bool pressed(Action action) { return check_flag(action, KeyFlags::Pressed); }
bool released(Action action) { return check_flag(action, KeyFlags::Released); }
bool repeated(Action action) { return check_flag(action, KeyFlags::Repeated); }

//------------------------------------------------------------------------------
// Bindings serialization
//------------------------------------------------------------------------------

bool load_bindings(std::string_view path) {
  (void)path;
  return false;
}

bool save_bindings(std::string_view path) {
  (void)path;
  return false;
}

//------------------------------------------------------------------------------
// Reset
//------------------------------------------------------------------------------

void clear_bindings() {
  for (auto& state : g_context.actionStates) {
    state.scanBindings.clear();
    state.keyBindings.clear();
  }

  g_context.scanMap.clear();

  for (auto& map : g_context.keyMap) {
    map.clear();
  }
}

void clear_actions() {
  clear_bindings();

  g_context.actionStates.clear();
  g_context.actionLookup.clear();
}

}  // namespace inp