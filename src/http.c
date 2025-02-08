/*

    HTTP Handler

*/

#include "../include/http.h"
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

char *serve(char *request, size_t buffer_size) {

    if (!request) {

        return NULL;

    }

    char *response = (char *) malloc(buffer_size);
    return "This is a test.";

}
