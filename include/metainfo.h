#ifndef METAINFO_H
#define METAINFO_H

typedef struct {
  char *name;
  char *description;
  char *sbu;
  char **provides;
  size_t provides_size;
  char **deps;
  size_t deps_size;
  char **downloads;
  size_t downloads_size;
} Metainfo;

void free_metainfo(Metainfo *metainfo);
Metainfo *parse_metainfo(char *pkgid);

#endif
