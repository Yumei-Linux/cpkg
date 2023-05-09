#include <stdio.h>
#include <unistd.h>
#include "../include/utils.h"

int is_root() {
  return getuid() == 0;
}

void check_root() {
  if (!is_root()) {
    error("This program needs to be run by the root user, try by using `sudo` or by switching to the `root` account instead.");
  }
}
