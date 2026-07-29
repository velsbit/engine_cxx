#pragma once

class Engine {
 public:
  Engine() {}
  ~Engine() {}

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;

  void run();
};