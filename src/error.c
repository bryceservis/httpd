#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

void error(char *call, char *parameter) {

    size_t size = strlen(call) + strlen(parameter) + 1;
    char *error = (char *) malloc(size);
    if (!error) {

      perror(parameter);

    } else {

      strcpy(error, call);
      strcat(error, " '");
      strcat(error, parameter);
      strcat(error, "'");
      perror(error);
      free(error);

    }

}

void serror(char *call) {

    perror(call);

}