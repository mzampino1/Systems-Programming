/* cat.c */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Globals */

char * PROGRAM_NAME = NULL;

/* Functions */

void usage(int status) {
    fprintf(stderr, "Usage: %s\n", PROGRAM_NAME);
    exit(status);
}

bool grep_stream(FILE *stream, char *str) {
    char buffer[BUFSIZ];
    bool match_found = false;

    while (fgets(buffer, BUFSIZ, stream)) {
        if (strstr(buffer, str)) {
            match_found = true;
            fputs(buffer, stdout);
        }
    }

    return match_found;
}

/* Main Execution */

int main(int argc, char *argv[]) {
    int argind = 1;

    /* Parse command line arguments */
    PROGRAM_NAME = argv[0];
    while (argind < argc && strlen(argv[argind]) > 1 && argv[argind][0] == '-') {
        char *arg = argv[argind++];
        switch (arg[1]) {
            case 'h':
                usage(0);
                break;
            default:
                usage(1);
                break;
        }
    }

    /* Ensure pattern is given */
    if (argc == 1) {
        usage(1);
    }

    /* Process input */
    return !grep_stream(stdin, argv[1]);
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
