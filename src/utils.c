#include <stdio.h>
#include <stdlib.h>

void error(char *message) {
  printf("[FATAL] %s\n", message);
  exit(1);
}
