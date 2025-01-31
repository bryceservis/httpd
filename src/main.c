/*

    Hypertext Transfer Protocol Daemon

*/

#include <unistd.h>
#include <stdio.h>

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

    }

}