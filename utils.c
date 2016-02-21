#include "utils.h"

char* get_line() {
  char* s = malloc(INPUT_BUFFER_SIZE * sizeof(char));
  return fgets(s, INPUT_BUFFER_SIZE, stdin);
}

bool balanced(char* expr) {
  char open_paren = 40, close_paren = 41;

  int parenlevel = 0;
  for (char *c = expr; *c != '\0' && *c != EOF; c++) {
    if (*c == open_paren) parenlevel++;
    if (*c == close_paren) parenlevel--;
  }

  return parenlevel == 0;
}
