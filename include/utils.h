#ifndef UTILS_H
#define UTILS_H

void error(char *message);
void setup_cache();
char *cache_dir();
int confirm(const char *message);

#endif // !UTILS_H
