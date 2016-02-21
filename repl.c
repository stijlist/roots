#include "roots.h"

void repl() {
  char *input, *more;
  Value env = nil();
  while ((input = get_line()) != NULL) {
    // while (unbalanced(input) && (more = get_line()) != NULL) {
      // input = append(input, more);
    // }
    print(eval_mutating_env(read(input), &env)); // read -> eval -> print -> loop
    printf("\n");
  } 
}

int main() { repl(); }
