#include "../inc/main.h"

int main(int ac, char** av, char** env) {
  UNUSED(ac);
  UNUSED(av);
  char** new_env = create_env(env);
  open_shell(new_env);
  return 0;
}
