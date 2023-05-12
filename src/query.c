#include <stdio.h>

#include "../include/colors.h"
#include "../include/metainfo.h"
#include "../include/utils.h"

void print_pkginfo(char *id, Metainfo *metainfo) {
  printf("%s[S] Displaying info for pkg %s:%s\n\n", GREEN, id, RESET);

  printf(" %sName:%s                 %s%s\n", MAGENTA, RESET, metainfo->name, RESET);
  printf(" %sDescription:%s          %s%s\n", MAGENTA, RESET, format(metainfo->description, 60), RESET);
  printf(" %sEstimated build time:%s %s%s\n\n", MAGENTA, RESET, metainfo->sbu, RESET);

  if (metainfo->deps_size > 0) {
    printf(" %sDependencies:%s\n", CYAN, RESET);

    for (size_t i = 0; i < metainfo->deps_size; i++)
      printf("   %s-> %s%s\n", MAGENTA, RESET, metainfo->deps[i]);
  }

  if (metainfo->provides_size > 0) {
    printf(" %sProvides:%s\n", CYAN, RESET);

    for (size_t i = 0; i < metainfo->provides_size; i++)
      printf("   %s-> %s%s\n", MAGENTA, RESET, metainfo->provides[i]);
  }

  if (metainfo->downloads_size > 0) {
    printf(" %sDownloads:%s\n", CYAN, RESET);

    for (size_t i = 0; i < metainfo->downloads_size; i++)
      printf("   %s-> %s%s\n", MAGENTA, RESET, metainfo->downloads[i]);
  }
}
