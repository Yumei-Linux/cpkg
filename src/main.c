#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/root.h"
#include "../include/utils.h"
#include "../include/help.h"
#include "../include/args.h"
#include "../include/cpkg-packages.h"

int main(int argc, char *argv[]) {
  check_root();
  setup_cache();

  Arguments *args = parse_arguments(argc, argv);

  if (strcmp(args->command, "help") == 0)
    help();

  if (strcmp(args->command, "setup") == 0)
    clone_cpkg_packages();

  free(args);

  return 0;
}
