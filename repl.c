#include "roots.h"
#define INPUT_BUFFER_SIZE 120

char* get_line() {
  char* s = malloc(INPUT_BUFFER_SIZE * sizeof(char));
  return fgets(s, INPUT_BUFFER_SIZE, stdin);
}

void repl() {
  char* input;
  Value env = nil();
  while ((input = get_line()) != NULL) {
    print(eval_mutating_env(read(input), &env)); // read -> eval -> print -> loop
    printf("\n");
  } 
}

int main() { repl(); }
