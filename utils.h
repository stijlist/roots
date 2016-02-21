#include <stdlib.h>
#include <stdio.h>
#define INPUT_BUFFER_SIZE 1024

typedef enum {
  false,
  true,
} bool;

bool unbalanced(char* expr);
char* get_line();
char* append(char* original, char* additional);
