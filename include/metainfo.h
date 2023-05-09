#ifndef METAINFO_H
#define METAINFO_H

typedef struct {
  char *name;
  char *description;
  char **downloads;
  size_t downloads_size;
} Metainfo;

Metainfo *parse_metainfo(char *pkgid);

#endif
