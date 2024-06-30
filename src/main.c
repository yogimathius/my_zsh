#include "../inc/main.h"

int main(int ac, char** av, char** env) {
  UNUSED(ac);
  UNUSED(av);
  char** new_env = create_env(env);
  open_shell(ac, av, new_env);
  return 0;
}
