/*

    HTTP

*/

#include "../include/http.h"
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

void *serve(char *request, size_t buffer_size) {

    if (!request) {

        return NULL;

    }

    regex_t regex;
    regcomp(&regex, "^GET /([^ ]*) HTTP/1", REG_EXTENDED);
    regmatch_t matches[2];
    char *file_name;
    if (regexec(&regex, request, 2, matches, 0) == 0) {

        request[matches[1].rm_eo] = '\0';
        file_name = request + matches[1].rm_so;

    }

    char *header = (char *)malloc(buffer_size);
    snprintf(header, buffer_size, "HTTP/1.1 200 OK\r\n" "Content-Type: %s\r\n", "\r\n\r\n", "text/html");
    int fd = open(file_name, O_RDONLY);
    struct stat st;
    fstat(fd, &st);
    off_t size = st.st_size;
    size_t length = 0;
    char *response = (char*) malloc(buffer_size);
    memcpy(response, header, strlen(header));
    length += strlen(header);
    ssize_t bytes_read = read(fd, response, buffer_size);
    length += bytes_read;
    return response;

}
