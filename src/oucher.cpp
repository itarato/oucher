#include <cstdlib>
#include <ctime>

#include "app.h"
#include "debug.h"

using namespace std;

int main() {
  LOG("Oucher started");

  srand(time(nullptr));

  App app{};
  app.init();
  app.loop();

  LOG("Oucher exit");

  return EXIT_SUCCESS;
}
