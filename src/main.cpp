#include <cstdio>
#include <stdexcept>

#include "engine.hpp"


int main() {
  try {
    Engine engine;
    engine.run();
  } catch (const std::exception& e) {
    std::fprintf(stderr, "[FATAL ERROR]: %s\n", e.what());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
