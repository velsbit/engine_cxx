// src/input/bind.cpp

#include "input/binding.hpp"

#include <algorithm>
#include <unordered_map>

namespace inp {

namespace {

struct KeyHash {
  std::size_t operator()(KeyCode key) const noexcept { return key.value; }
};

struct ActionHash {
  std::size_t operator()(Action action) const noexcept {
    return (static_cast<uint32_t>(action.gen) << 16) | action.index;
  }
};

std::unordered_map<KeyCode, std::vector<Action>, KeyHash> g_key_to_actions;
std::unordered_map<Action, std::vector<KeyCode>, ActionHash> g_action_to_keys;

template <typename T>
bool contains(const std::vector<T>& vec, const T& value) noexcept {
  return std::find(vec.begin(), vec.end(), value) != vec.end();
}

template <typename T>
void erase(std::vector<T>& vec, const T& value) noexcept {
  vec.erase(std::remove(vec.begin(), vec.end(), value), vec.end());
}

}  // namespace

void bind(KeyCode key, Action action) noexcept {
  auto& actions = g_key_to_actions[key];
  if (!contains(actions, action)) actions.push_back(action);

  auto& keys = g_action_to_keys[action];
  if (!contains(keys, key)) keys.push_back(key);
}

void unbind(KeyCode key, Action action) noexcept {
  if (auto it = g_key_to_actions.find(key); it != g_key_to_actions.end()) {
    erase(it->second, action);
    if (it->second.empty()) g_key_to_actions.erase(it);
  }

  if (auto it = g_action_to_keys.find(action); it != g_action_to_keys.end()) {
    erase(it->second, key);
    if (it->second.empty()) g_action_to_keys.erase(it);
  }
}

void clear_binding(KeyCode key) noexcept {
  auto it = g_key_to_actions.find(key);
  if (it == g_key_to_actions.end()) return;

  for (Action action : it->second) {
    auto ait = g_action_to_keys.find(action);
    if (ait == g_action_to_keys.end()) continue;

    erase(ait->second, key);

    if (ait->second.empty()) g_action_to_keys.erase(ait);
  }

  g_key_to_actions.erase(it);
}

void clear_binding(Action action) noexcept {
  auto it = g_action_to_keys.find(action);
  if (it == g_action_to_keys.end()) return;

  for (KeyCode key : it->second) {
    auto kit = g_key_to_actions.find(key);
    if (kit == g_key_to_actions.end()) continue;

    erase(kit->second, action);

    if (kit->second.empty()) g_key_to_actions.erase(kit);
  }

  g_action_to_keys.erase(it);
}

void clear_binding_all() noexcept {
  g_key_to_actions.clear();
  g_action_to_keys.clear();
}

bool bound(KeyCode key, Action action) noexcept {
  auto it = g_key_to_actions.find(key);
  if (it == g_key_to_actions.end()) return false;

  return contains(it->second, action);
}

std::span<const KeyCode> get_keys(Action action) noexcept {
  auto it = g_action_to_keys.find(action);
  if (it == g_action_to_keys.end()) return {};

  return it->second;
}

std::span<const Action> get_actions(KeyCode key) {
  auto it = g_key_to_actions.find(key);
  if (it == g_key_to_actions.end()) return {};

  return it->second;
}

}  // namespace inp