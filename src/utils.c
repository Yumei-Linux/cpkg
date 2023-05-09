#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

void error(char *message) {
  printf("[FATAL] %s\n", message);
  exit(1);
}

char *cache_dir() {
  const char *cache_dir = "/var/cache/cpkg";
  char *cache_directory = strdup(cache_dir);
  if (cache_directory == NULL) {
    error("Cannot get the cache directory!");
  }

  return cache_directory;
}

void setup_cache() {
  char *cache_directory = cache_dir();

  struct stat st = {0};
  if (stat(cache_directory, &st) == -1) {
    if (mkdir(cache_directory, S_IRWXU) == -1) {
      error("Couldn't create cache directory.");
    }
  }

  free(cache_directory);
}

int confirm(const char *message) {
  printf("%s (y/n) ", message);
  char answer = getchar();
  while (getchar() != '\n'); // Clear input buffer
  return (answer == 'y' || answer == 'Y');
}
