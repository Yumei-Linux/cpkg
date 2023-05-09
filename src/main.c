#include <stdio.h>
#include "../include/root.h"
#include "../include/utils.h"
#include "../include/cpkg-packages.h"

int main() {
  check_root();
  setup_cache();
  clone_cpkg_packages();
}
