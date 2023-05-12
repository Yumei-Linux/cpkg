#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <yaml.h>

#include "../include/utils.h"

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

void free_metainfo(Metainfo *metainfo) {
  free(metainfo->name);
  free(metainfo->description);
  free(metainfo->sbu);

  for (size_t i = 0; i < metainfo->provides_size; i++)
    free(metainfo->provides[i]);

  free(metainfo->provides);

  for (size_t i = 0; i < metainfo->deps_size; i++)
    free(metainfo->deps[i]);

  free(metainfo->deps);

  for (size_t i = 0; i < metainfo->downloads_size; i++)
    free(metainfo->downloads[i]);

  free(metainfo->downloads);
}

Metainfo* parse_metainfo(char *pkgid) {
  char *cache_directory = cache_dir();
  if (cache_directory == NULL)
    error("Cannot get the cache directory.");

  size_t path_size = strlen(cache_directory)
    + strlen("/cpkg-packages/")
    + strlen(pkgid)
    + strlen("/metainfo.yaml");

  char *path = malloc(path_size + 1);
  if (path == NULL)
    error("Mem error.");

  sprintf(
    path,
    "%s/cpkg-packages/%s/metainfo.yaml",
    cache_directory,
    pkgid
  );

  struct stat st = {0};
  if (stat(path, &st) == -1) {
    free(path);
    fprintf(
      stderr,
      "Cannot parse metainfo for %s: No such file or directory",
      pkgid
    );
    free(pkgid);
    exit(1);
  }

  FILE *input_file = fopen(path, "r");
  if (input_file == NULL) {
    free(path);
    free(pkgid);
    fprintf(stderr, "Failed to open metainfo file\n");
    exit(1);
  }

  yaml_parser_t parser;
  if (!yaml_parser_initialize(&parser)) {
    free(path);
    free(pkgid);
    fprintf(stderr, "Failed to initialize parser!\n");
    exit(1);
  }

  yaml_parser_set_input_file(&parser, input_file);

  yaml_document_t document;
  if (!yaml_parser_load(&parser, &document)) {
    free(path);
    free(pkgid);
    fprintf(stderr, "Failed to parse YAML!\n");
    exit(1);
  }

  Metainfo metainfo = {0};
  yaml_node_t *root = yaml_document_get_root_node(&document);
  if (root && root->type == YAML_MAPPING_NODE) {
    for (yaml_node_pair_t *pair = root->data.mapping.pairs.start; pair < root->data.mapping.pairs.top; pair++) {
      char *key = (char *)yaml_document_get_node(&document, pair->key)->data.scalar.value;
      yaml_node_t *value = yaml_document_get_node(&document, pair->value);

      if (strcmp(key, "name") == 0 && value->type == YAML_SCALAR_NODE) {
        metainfo.name = strdup((const char*)value->data.scalar.value);
      } else if (strcmp(key, "description") == 0 && value->type == YAML_SCALAR_NODE) {
        metainfo.description = strdup((const char*)value->data.scalar.value);
      } else if (strcmp(key, "sbu") == 0 && value->type == YAML_SCALAR_NODE) {
        metainfo.sbu = strdup((const char*)value->data.scalar.value);
      } else if (strcmp(key, "provides") == 0 && value->type == YAML_SEQUENCE_NODE) {
        metainfo.provides_size = value->data.sequence.items.top - value->data.sequence.items.start;
        metainfo.provides = malloc(sizeof(char *) * metainfo.provides_size);
        if (metainfo.provides) {
          for (size_t i = 0; i < metainfo.provides_size; i++) {
            yaml_node_t *item = yaml_document_get_node(&document, value->data.sequence.items.start[i]);
            if (item->type == YAML_SCALAR_NODE)
              metainfo.provides[i] = strdup((const char *) item->data.scalar.value);
          }
        }
      } else if (strcmp(key, "deps") == 0 && value->type == YAML_SEQUENCE_NODE) {
        metainfo.deps_size = value->data.sequence.items.top - value->data.sequence.items.start;
        metainfo.deps = malloc(sizeof(char *) * metainfo.deps_size);
        if (metainfo.deps) {
          for (size_t i = 0; i < metainfo.deps_size; i++) {
            yaml_node_t *item = yaml_document_get_node(&document, value->data.sequence.items.start[i]);
            if (item->type == YAML_SCALAR_NODE)
              metainfo.deps[i] = strdup((const char *) item->data.scalar.value);
          }
        }
      } else if (strcmp(key, "downloads") == 0 && value->type == YAML_SEQUENCE_NODE) {
        metainfo.downloads_size = value->data.sequence.items.top - value->data.sequence.items.start;
        metainfo.downloads = malloc(sizeof(char *) * metainfo.downloads_size);
        if (metainfo.downloads) {
          for (size_t i = 0; i < metainfo.downloads_size; i++) {
            yaml_node_t *item = yaml_document_get_node(&document, value->data.sequence.items.start[i]);
            if (item->type == YAML_SCALAR_NODE)
              metainfo.downloads[i] = strdup((const char *)item->data.scalar.value);
          }
        }
      }
    }
  }

  fclose(input_file);
  free(path);

  yaml_document_delete(&document);
  yaml_parser_delete(&parser);

  if (metainfo.name == NULL ||
      metainfo.description == NULL) {
    free(pkgid);
    fprintf(stderr, "Invalid metainfo.yaml!\n");
    exit(1);
  }

  Metainfo *copy = malloc(sizeof(Metainfo));
  if (copy == NULL)
    error("Mem error.");

  *copy = metainfo;

  return copy;
}
