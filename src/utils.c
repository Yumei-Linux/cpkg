#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../include/colors.h"

void error(char *message) {
  printf("%s[F] %s%s\n", RED, message, RESET);
  exit(1);
}

char* format(const char* str, int max_len) {
  static char buf[256]; // static buffer to hold the result
  int len = strlen(str);

  if (len <= max_len) {
    strcpy(buf, str); // copy the whole string if it's shorter than the max length
  } else {
    strncpy(buf, str, max_len); // copy the first max_len characters
    buf[max_len] = '\0'; // null-terminate the result
    strcat(buf, "..."); // append "..."
  }

  return buf;
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
  printf("\n%s[C] %s%s %s(y/n)%s ", BLUE, GREEN, message, YELLOW, RESET);
  char answer = getchar();
  while (getchar() != '\n'); // Clear input buffer
  return (answer == 'y' || answer == 'Y');
}
