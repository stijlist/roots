#include "roots.h"
#define INPUT_BUFFER_SIZE 120

char* get_line() {
    char* s = malloc(INPUT_BUFFER_SIZE * sizeof(char));
    return fgets(s, INPUT_BUFFER_SIZE, stdin);
}

void repl() {
    char* input;
    do { 
      input = get_line();
      printValue(eval_empty(read(input).value));
      printf("\n");
    } while (input != NULL);
}

int main() { repl(); }
