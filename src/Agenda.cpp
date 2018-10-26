#include "AgendaUI.hpp"
#include <signal.h>
#include <stdlib.h>
using namespace std;

AgendaUI UI;

void func(int sig) {
  cout << "You have stop this Agenda illegal\n";
  cout << "Your file have been saved!\n";
  UI.quitAgenda();
  exit(0);
}

int main() {
  signal(SIGINT, func);
  UI.OperationLoop();
}
