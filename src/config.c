/*

    Configuration Handler

*/

#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "../include/error.h"
#include "../include/config.h"
#include "../include/schema/host.h"

int filter(const struct dirent *file_name) {

  char *name = file_name->d_name;
  char *token = strtok(name, ".");
  while (token != NULL) {

    if (strcmp(token, "conf") == 0) {

      return 1;

    }

    token = strtok(NULL, ".");

  }

  return 0;

}

host_t *load_hosts(char path[]) {

    struct dirent **entries;
    int index = scandir(path, &entries, filter, NULL);
    if (index == -1) {

        log_error("scandir", path);
        exit(EXIT_FAILURE);

    }

    while (index--) {



    }

    return NULL;

};

void main() {

  load_hosts("../conf");

}