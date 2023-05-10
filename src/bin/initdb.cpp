#include "catalog.h"
#include <iostream>
#include <string.h>

int main(int argc, char **argv) {
  if (argc == 1) {
    printf("Usage: initdb -D path/to/folder\n");
    return 0;
  }

  if (strcmp(argv[1], "-D") == 0) {
    char *path = argv[2];
    card::init_db(path);
  }

  return 0;
}