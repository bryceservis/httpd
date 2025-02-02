/*

    Hypertext Transfer Protocol Daemon

    Configuration Handler

*/

#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "../include/error.h"
#include "../include/config.h"

#include <ctype.h>

#include "../include/schema/host.h"

typedef enum {

    HOST,
    ADDRESSES,
    NAME,
    ROOT,
    HTTPS_PORT,
    HTTP_PORT,
    CERTIFICATE,
    PRIVATE_KEY,
    UNRECOGNIZED

} config_key_t;

config_key_t parse_key(const char *str) {

    if (strcmp(str, "host") == 0) return HOST;
    if (strcmp(str, "addresses") == 0) return ADDRESSES;
    if (strcmp(str, "name") == 0) return NAME;
    if (strcmp(str, "root") == 0) return ROOT;
    if (strcmp(str, "https_port") == 0) return HTTPS_PORT;
    if (strcmp(str, "http_port") == 0) return HTTP_PORT;
    if (strcmp(str, "certificate") == 0) return CERTIFICATE;
    if (strcmp(str, "private_key") == 0) return PRIVATE_KEY;
    return UNRECOGNIZED;

}

void clean(char *str) {

    int j = 0;
    for (int i = 0; str[i] != '\0'; i++) {

        if (!isspace(str[i])) {

            str[j++] = str[i];

        }

    }

    str[j] = '\0';

}

host_t *parse_host(char *path) {

    host_t *host = NULL;
    FILE *host_file = fopen(path, "r");
    if (!host_file) {

        error("fopen", path);

    }

    char *line = NULL;
    size_t length = 0;
    while (getline(&line, &length, host_file) != -1) {

        char *segment = strtok(line, "=");
        while (segment != NULL) {

            if (segment[0] == '#') {

                segment = strtok(NULL, "=");
                continue;

            }

            clean(segment);
            switch (parse_key(segment)) {

                case HOST:
                    // We will *always* need the configuration for the host; do not free this, even if your IDE tells you that you should.
                    host = (host_t *) malloc(sizeof(host_t));
                    host->https = NULL;
                    host->http = NULL;
                    break;

                case ADDRESSES:

                    if (!host) {

                        break;

                    }

                    segment = strtok(NULL, "=");
                    clean(segment);
                    char *addresses = strdup(segment);
                    host->addresses = addresses;
                    break;

                case NAME:

                    if (!host) {

                        break;

                    }

                    segment = strtok(NULL, "=");
                    clean(segment);
                    char *name = strdup(segment);
                    host->name = name;
                    break;

                case ROOT:

                    if (!host) {

                        break;

                    }

                    segment = strtok(NULL, "=");
                    clean(segment);
                    char *root = strdup(segment);
                    host->root = root;
                    break;

                case HTTPS_PORT:

                    if (!host) {

                        break;

                    }

                    segment = strtok(NULL, "=");
                    clean(segment);
                    if (!host->https) {

                        host->https = (https_t *) malloc(sizeof(https_t));

                    }

                    host->https->port = strtol(segment, NULL, 10);
                    break;

                case HTTP_PORT:

                    if (!host) {

                        break;

                    }

                    segment = strtok(NULL, "=");
                    clean(segment);
                    if (!host->http) {

                        host->http = (http_t *) malloc(sizeof(http_t));

                    }

                    host->http->port = strtol(segment, NULL, 10);
                    break;

                case CERTIFICATE:

                    if (!host) {

                        break;

                    }

                    segment = strtok(NULL, "=");
                    clean(segment);
                    if (!host->https) {

                        host->https = (https_t *) malloc(sizeof(http_t));

                    }

                    char *https_certificate = strdup(segment);
                    host->https->certificate = https_certificate;
                    break;

                case PRIVATE_KEY:

                    if (!host) {

                        break;

                    }

                    segment = strtok(NULL, "=");
                    clean(segment);
                    if (!host->https) {

                        host->https = (https_t *) malloc(sizeof(http_t));

                    }

                    char *https_private_key = strdup(segment);
                    host->https->private_key = https_private_key;
                    break;

                default:

                    break;

            }

            segment = strtok(NULL, "=");

        }

    }

    return host;

}