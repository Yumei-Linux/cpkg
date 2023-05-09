#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMMANDS_LEN 2

typedef struct {
  char *command;
} Arguments;

Arguments *parse_arguments(int argc, char *argv[]) {
  Arguments *args = malloc(sizeof(Arguments*));

  args->command = NULL;

  char *valid_commands[COMMANDS_LEN] = {
    "help", "setup"
  };

  for (int i = 0; i < argc; i++) {
    if (i > 0) {
      for (int j = 0; j < COMMANDS_LEN; j++) {
        if (strcmp(argv[i], valid_commands[j]) == 0)
          args->command = valid_commands[j];
      }
    }
  }

  if (!args->command)
    args->command = "help";

  return args;
}
