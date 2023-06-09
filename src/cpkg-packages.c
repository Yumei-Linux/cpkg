#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../include/utils.h"

int check_store() {
  char *cache_directory = cache_dir();
  if (cache_directory == NULL)
    error("Cannot get the cache directory");

  size_t path_size = strlen(cache_directory) + strlen("/cpkg-packages");
  char *path = malloc(path_size + 1);

  sprintf(path, "%s/cpkg-packages", cache_directory);

  struct stat st = {0};
  if (stat(path, &st) == -1) {
    free(path);
    return 0;
  }

  free(cache_directory);
  free(path);

  return 1;
}

void validate_store() {
  if (check_store() == 0)
    error("Please, first, configure the store by running `sudo cpkg setup`");
}

void clone_cpkg_packages() {
  printf("Starting the cpkg-packages setup...\n");

  char *cache_directory = cache_dir();
  if (cache_directory == NULL) {
    error("Cannot get the cache directory.");
  }

  // if it already exists, skipping it.
  if (check_store() == 1) {
    free(cache_directory);
    return;
  }
  
  size_t command_size = strlen("cd ")
    + strlen(cache_directory)
    + strlen(" && git clone --depth=1 https://github.com/Yumei-Linux/cpkg-packages.git") + 1;

  char *command = malloc(command_size);
  if (command == NULL) {
    error("Cannot allocate memory for command.");
  }

  sprintf(command, "cd %s && git clone --depth=1 https://github.com/Yumei-Linux/cpkg-packages.git", cache_directory);

  if (system(command) != 0)
    error("Failed to clone the cpkg-packages repository.");

  free(command);
  free(cache_directory);
}
