#include <stdio.h>

int main() {
  const char *test = "12";
  char *c = test;
  printf("%c", *c);
  c++;
  printf("%c", *c);
  c++;
  printf("is the pointer now null? %s", *c == 0 ? "true" : "false");
  puts("done.");
}
