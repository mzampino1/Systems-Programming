/* tailit.c: Output the last part of files */

#include "list.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Functions */

void usage(int status) {
    fprintf(stderr, "Usage: tailit [-n NUMBER]\n\n");
    fprintf(stderr, "    -n NUMBER  Output the last NUMBER of lines (default is 10)\n");
    exit(status);
}

List *tail_stream(FILE *stream, size_t limit) {
    char buffer[BUFSIZ];
    int total_lines = 0;
    List *l = list_create();
    while (fgets(buffer, sizeof(buffer), stream)) {
        list_append(l, (Value)strdup(buffer));
        total_lines++;
        // remove first node if size exceeds limit
        if (l->size > limit)
            free(list_pop(l, 0).string);
    }

    return l;
}

/* Main Execution */

int main(int argc, char *argv[]) {
    // Parse command line arguments
    int num = 10;
    switch (argc) {
        case 1:
            break;
        case 2:
            if (strcmp(argv[1], "-h") == 0)
                usage(0);
            else
                usage(1);
            break;
        case 3:
            if (strcmp(argv[1], "-n") == 0)
                num = atoi(argv[2]);
            break;
        default:
            usage(1);
            break;
    }

    // Construct tail of stream
    List *tail = tail_stream(stdin, num);

    // Print out tail
    while (tail->size != 0) {
        char *str = list_pop(tail, 0).string;
        printf("%s", str);
        free(str);
    }

    list_delete(tail, true);

    return EXIT_SUCCESS;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
