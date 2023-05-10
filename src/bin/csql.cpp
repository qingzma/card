#include "../../include/catalog.h"
#include <iostream>
#include <string.h>

int main(int argc, char **argv) {
  if (argc == 1) {
    printf("Usage: csql -D path/to/folder\n");
    return 0;
  }

  if (strcmp(argv[1], "-D") == 0) {
    char *path = argv[2];
    printf("Welcome to Card!\n");
    printf("Enter exit() to exit!\n");

    // char *EXIT_STR = "exit()";
    bool b_received_query = false;
    bool b_exit = false;
    std::string query;
    std::string buffer;
    do {
      getline(std::cin, buffer);

      if (buffer.find("exit()") != std::string::npos)
        b_exit = true;
      if (b_exit)
        break;
      if (buffer.find(";") == std::string::npos) {
        query += buffer;
      } else {
        auto it = buffer.find(";");
        buffer = buffer.substr(0, it);
        query += buffer;
        b_received_query = true;
        // process query here
        printf("accepted query:\n");
        printf("%s\n", query.c_str());
        // empty query buffer
        query.clear();
      }
    } while (!b_exit || !b_received_query);
  }

  return 0;
}