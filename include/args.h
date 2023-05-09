#ifndef ARGS_H
#define ARGS_H

typedef struct {
  char *command;
} Arguments;

Arguments* parse_arguments(int argc, char *argv[]);

#endif
