#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "../include/utils.h"

char* try_to_get_id(int argc, char *argv[]) {
  if (argc < 3)
    error("Invalid arguments, please supply the positional argument <id>");

  char *id = strdup(argv[2]);
  if (id == NULL)
    error("Mem error.");

  return id;
}

void build_pkg(char *id) {
  printf("Attempting to build %s.\n", id);

  char *cache_directory = cache_dir();
  if (!cache_directory)
    error("Cannot get the cache directory");

  size_t path_size = strlen(cache_directory)
    + strlen("/cpkg-packages/")
    + strlen(id);

  char *path = malloc(path_size + 1);
  if (path == NULL)
    error("Cannot allocate memory");

  sprintf(path, "%s/cpkg-packages/%s", cache_directory, id);

  struct stat st = {0};
  if (stat(path, &st) == -1) {
    free(path);
    error(
      "Inexistent package on the cpkg pkgs store, please make a PR to https://github.com/Yumei-Linux/cpkg-packages if needed!"
    );
  }

  printf("Building package %s...\n", path);

  // ...

  free(path);
}
