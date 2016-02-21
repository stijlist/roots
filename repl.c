#include "roots.h"

void repl() {
  char *input, *more;
  Value env = nil();
  while ((input = get_line()) != NULL) {

    while (!balanced(input) && (more = get_line()) != NULL) {
      // printf("input is unbalanced\n");
      // printf("there is more input: %s \n", more);

      // TODO: this 140 constant needs to go
      strlcat(input, more, 140);

      // printf("appending unbalanced with more input yields: %s\n", input);
    }
    print(eval_mutating_env(read(input), &env)); // read -> eval -> print -> loop
    printf("\n");
  } 
}

int main() { repl(); }
