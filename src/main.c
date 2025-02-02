/*

    Hypertext Transfer Protocol Daemon

*/

#include "../include/config.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    char *file_c = NULL;
    int option;

    while ((option = getopt(argc, argv, "hc:")) != -1) {

        switch (option) {

            case 'h':
                printf("help");
                return 0;

            case 'c':
                file_c = optarg;
                break;

            default:
                break;

        }

    }

    if (!file_c) {

        printf("You must set a configuration file.\n");
        exit(EXIT_SUCCESS);

    }

    host_t *config = parse_host(file_c);

    printf("addresses %s\n", config->addresses);
    printf("name %s\n", config->name);
    printf("root %s\n", config->root);
    printf("https port %d\n", config->https->port);
    printf("https cert %s\n", config->https->certificate);
    printf("https priv %s\n", config->https->private_key);
    printf("http port %d\n", config->http->port);

}
