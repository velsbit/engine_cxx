// src/input/action.cpp

#include "input/action.hpp"

#include <string>
#include <unordered_map>
#include <vector>

#include "device_internal.hpp"
#include "input/binding.hpp"


namespace inp {

namespace {

struct ActionSlot {
  uint16_t generation = 1;
  bool alive = false;
  std::string name;
};

std::vector<ActionSlot> g_actions;
std::unordered_map<std::string, uint16_t> g_name_to_index;

bool valid(Action action) noexcept {
  if (!action) return false;

  if (action.index >= g_actions.size()) return false;

  const auto& slot = g_actions[action.index];

  return slot.alive && slot.generation == action.gen;
}

}  // namespace

Action create_action(std::string_view name) noexcept {
  if (auto it = g_name_to_index.find(std::string(name));
      it != g_name_to_index.end()) {
    const auto& slot = g_actions[it->second];
    return {it->second, slot.generation};
  }

  uint16_t index = static_cast<uint16_t>(g_actions.size());

  g_actions.push_back({
      .generation = 1,
      .alive = true,
      .name = std::string(name),
  });

  g_name_to_index.emplace(g_actions.back().name, index);

  return {index, 1};
}

void destroy_action(Action action) noexcept {
  if (!valid(action)) return;

  auto& slot = g_actions[action.index];

  g_name_to_index.erase(slot.name);

  clear_binding(action);

  slot.alive = false;
  ++slot.generation;
  slot.name.clear();
}

bool down(Action action) noexcept {
  if (!valid(action)) return false;

  for (KeyCode key : get_keys(action))
    if (key_down(key)) return true;

  return false;
}

bool pressed(Action action) noexcept {
  if (!valid(action)) return false;

  for (KeyCode key : get_keys(action))
    if (key_pressed(key)) return true;

  return false;
}

bool released(Action action) noexcept {
  if (!valid(action)) return false;

  for (KeyCode key : get_keys(action))
    if (key_released(key)) return true;

  return false;
}

std::string_view get_action_name(Action action) noexcept {
  if (!valid(action)) return {};

  return g_actions[action.index].name;
}

}  // namespace inp