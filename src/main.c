#include "../inc/main.h"

int main(int ac, char** av, char** env) {
  UNUSED(ac);
  UNUSED(av);
  open_shell(env);
  return 0;
}
