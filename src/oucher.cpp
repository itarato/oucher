#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "app.h"
#include "debug.h"

using namespace std;

int main(int argc, const char** argv) {
  LOG("Oucher started");

  srand(time(nullptr));

  App app{};
  app.init();

  if (argc == 2) {
    int mapIdx;
    sscanf(argv[1], "%d", &mapIdx);
    app.setMap(mapIdx);
  }

  app.loop();

  LOG("Oucher exit");

  return EXIT_SUCCESS;
}
