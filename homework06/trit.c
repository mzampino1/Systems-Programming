/* trit.c: translation utility */

#include "str.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Constants */

enum {
    LOWER   = 1<<1,
    UPPER   = 1<<2,
    TITLE   = 1<<3,
    STRIP   = 1<<4,
    DELETE  = 1<<5,
};

/* Functions */

void usage(int status) {
    fprintf(stderr, "Usage: trit SET1 SET2\n\n");
    fprintf(stderr, "Post Translation filters:\n\n");
    fprintf(stderr, "   -l      Convert to lowercase\n");
    fprintf(stderr, "   -u      Convert to uppercase\n");
    fprintf(stderr, "   -t      Convert to titlecase\n");
    fprintf(stderr, "   -s      Strip trailing whitespace\n");
    fprintf(stderr, "   -d      Delete letters in SET1\n");
    exit(status);
}

void translate_stream(FILE *stream, const char *set1, const char *set2, int flags) {
    char line[BUFSIZ];
    while(fgets(line, BUFSIZ, stream)) {
        // perform actions based on flags
        if (flags & DELETE) {
            if (set1) str_delete(line, set1, line);
        } else {
            if (set1 && set2) str_translate(line, set1, set2, line);
        }
        
        if (flags & LOWER) {
            str_lower(line, line);
        }
        if (flags & UPPER) {
            str_upper(line, line);
        }
        if (flags & TITLE) {
            str_title(line, line);
        }
        if (flags & STRIP) {
            str_rstrip(line, NULL, line);
            // need to print with newline after stripping
            printf("%s\n", line);
        } else {
            printf("%s", line);
        }
    }
}

/* Main Execution */

int main(int argc, char *argv[]) {
    // Parse command line arguments
    int flags = 0;
    char *set1 = NULL;
    char *set2 = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-l") == 0) {
            flags |= LOWER;
        } else if (strcmp(argv[i], "-u") == 0) {
            flags |= UPPER;
        } else if (strcmp(argv[i], "-t") == 0) {
            flags |= TITLE;
        } else if (strcmp(argv[i], "-s") == 0) {
            flags |= STRIP;  
        } else if (strcmp(argv[i], "-d") == 0) {
            flags |= DELETE;
        } else if (strcmp(argv[i], "-h") == 0) {
            usage(0);
        } else {
            if (!set1) {
                set1 = argv[i];
            } else if (!set2) {
                set2 = argv[i];
            } else {
                usage(1);
            }
        }
    }

    // Translate standard input
    translate_stream(stdin, set1, set2, flags);
    return EXIT_SUCCESS;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */

