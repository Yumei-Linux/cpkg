#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "../include/metainfo.h"
#include "../include/utils.h"

char* try_to_get_id(int argc, char *argv[]) {
  if (argc < 3)
    error("Invalid arguments, please supply the positional argument <id>");

  char *id = strdup(argv[2]);
  if (id == NULL)
    error("Mem error.");

  return id;
}

void download_source(char *id, char *download_url) {
  printf("Downloading %s for package %s with wget...\n", download_url, id);

  char *cache_directory = cache_dir();
  if (!cache_directory)
    error("Cannot get the cache directory.");

  size_t cmd_size = strlen("cd ")
    + strlen(cache_directory)
    + strlen("/cpkg-packages/")
    + strlen(id)
    + strlen(" && wget ")
    + strlen(download_url);

  char *cmd = malloc(cmd_size + 1);
  if (cmd == NULL)
    error("Mem error.");

  sprintf(
    cmd,
    "cd %s/cpkg-packages/%s && wget %s",
    cache_directory,
    id, download_url
  );

  if (system(cmd) != 0) {
    if (!confirm("Command failed, do you still want to continue the build process?")) {
      free(cache_directory);
      free(cmd);
      exit(1);
    }
  }
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
    free(id);
    error(
      "Inexistent package on the cpkg pkgs store, please make a PR to https://github.com/Yumei-Linux/cpkg-packages if needed!"
    );
  }

  printf("Building package %s...\n", path);

  Metainfo *metainfo = parse_metainfo(id);

  printf("----------------------------------\n");
  printf("Displaying info for pkg %s:\n", id);
  printf("----------------------------------\n");

  printf("Name: %s\n", metainfo->name);
  printf("Description: %s\n", metainfo->description);
  printf("Estimated build time: %s\n", metainfo->sbu);
  printf("Downloads:\n");

  for (size_t i = 0; i < metainfo->downloads_size; i++) {
    printf("  -> Download #%ld: %s\n", i + 1, metainfo->downloads[i]);
  }

  free(path);

  printf("--------------------------------\n");
  if (!confirm("Do you wish to install it?")) {
    free(id);
    exit(0);
  }

  printf("--------------------------------\n");
  printf("Downloading sources...\n");

  for (size_t i = 0; i < metainfo->downloads_size; i++) {
    download_source(id, metainfo->downloads[i]);
  }

  printf("--------------------------------\n");
  printf("Executing build script...\n");
  
  size_t build_path_size = strlen("cd ")
    + strlen(cache_directory)
    + strlen("/cpkg-packages/")
    + strlen(id)
    + strlen(" && ./build.sh");

  char *build_path = malloc(build_path_size + 1);
  if (build_path == NULL)
    error("Mem error.");

  sprintf(
    build_path,
    "cd %s/cpkg-packages/%s && ./build.sh",
    cache_directory, id
  );

  if (system(build_path) != 0) {
    printf("[FATAL] **Build of the package failed.**");
  }

  printf("--------------------------------\n");

  free(cache_directory);
  free(metainfo->name);
  free(metainfo->description);
  free(metainfo->sbu);

  for (size_t i = 0; i < metainfo->downloads_size; i++)
    free(metainfo->downloads[i]);

  free(metainfo->downloads);
}
