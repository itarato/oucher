
#include <cstdlib>
#include <optional>
#include <vector>

#include "app.h"
#include "debug.h"
#include "raylib.h"

using namespace std;

int main() {
  LOG("Oucher started");

  App app{};
  app.init();
  app.loop();

  LOG("Oucher exit");

  return EXIT_SUCCESS;
}
