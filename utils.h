#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define INPUT_BUFFER_SIZE 1024

typedef enum {
  false,
  true,
} bool;

bool balanced(char* expr);
char* get_line();
