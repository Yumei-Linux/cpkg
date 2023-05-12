#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "../include/metainfo.h"
#include "../include/utils.h"
#include "../include/colors.h"
#include "../include/query.h"

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

int build_pkg(char *id, int with_confirm) {
  printf("%s[I] Attempting to build %s%s.\n", BLUE, id, RESET);

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

  printf("%s[W] Building package %s...%s\n", YELLOW, path, RESET);

  Metainfo *metainfo = parse_metainfo(id);

  print_pkginfo(id, metainfo);

  free(path);

  // check if it's already installed
  if (metainfo->provides_size > 0) {
    for (size_t i = 0; i < metainfo->provides_size; i++) {
      char *filename = metainfo->provides[i];

      // TODO: Check for -f to rebuild anyways
      if (access(filename, F_OK) == 0) {
        if (with_confirm == 1) {
          printf("\n%s[W] This package is already installed, if you want to rebuild, write 'y'.%s\n", YELLOW, RESET);
        } else {
          free(cache_directory);
          free(metainfo);
          printf(
            "\n%s[W] Skipping building for %s because it's already installed, use -f to force the rebuild of these packages.%s\n", YELLOW, id, RESET
          );

          return 0;
        }

        break;
      }
    }
  }

  if (with_confirm == 1) {
    if (!confirm("Do you wish to install it?")) {
      free(id);
      exit(0);
    }
  }

  // recursive deps building process.
  if (metainfo->deps_size > 0) {
    for (size_t i = 0; i < metainfo->deps_size; i++) {
      if (build_pkg(metainfo->deps[i], 0) == 1) {
        printf("%s[F] Subdep (%s) for package %s failed...%s\n", RED, metainfo->deps[i], id, RESET);
        free(cache_directory);
        free_metainfo(metainfo);
        return 1;
      }
    }
  }

  printf("%s[I] Downloading sources...%s\n", BLUE, RESET);

  for (size_t i = 0; i < metainfo->downloads_size; i++)
    download_source(id, metainfo->downloads[i]);

  printf("%s[I] Executing build script...%s\n", BLUE, RESET);
  
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

  int statuscode = 0; // ok

  if (system(build_path) != 0) {
    printf("%s[F] Build of the package failed.%s\n", RED, RESET);
    statuscode = 1; // fatal
  }

  free(cache_directory);
  free_metainfo(metainfo);

  if (statuscode == 0)
    printf("%s[S] Package building for %s ended successfully!%s\n", GREEN, id, RESET);

  return statuscode;
}
