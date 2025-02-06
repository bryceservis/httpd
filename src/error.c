/*

    Error Helper

*/

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

void error(char *reason) {

    if (errno != 0) {

        perror(reason);

    } else {

        printf("%s\n", reason);

    }

    exit(EXIT_FAILURE);

}