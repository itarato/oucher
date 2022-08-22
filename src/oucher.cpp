
#include <cstdlib>
#include <optional>
#include <vector>

#include "app.h"
#include "raylib.h"

using namespace std;

int main() {
  App app{};
  app.init();
  app.loop();

  return EXIT_SUCCESS;
}
