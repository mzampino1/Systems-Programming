/* seqit.c: Print a sequence of numbers */

#include "list.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

/* Functions */

void usage(int status) {
    fprintf(stderr, "Usage: seqit LAST\n");
    fprintf(stderr, "       seqit FIRST LAST\n");
    fprintf(stderr, "       seqit FIRST INCREMENT LAST\n");
    exit(status);
}

List *generate_sequence(ssize_t first, ssize_t increment, ssize_t last) {
    // make list where each node's value is an element of the sequence
    List *l = list_create();
    int64_t num = first;
    if (increment > 0) {
        while (num <= last) {
            list_append(l, (Value)num);
            num += increment;
        }
    } else {
        while (num >= last) {
            list_append(l, (Value)num);
            num += increment;
        }
    }

    return l;
}

/* Main Execution */

int main(int argc, char *argv[]) {
    // Parse command line arguments
    int last = 0;
    int first = 1;
    int increment = 1;
    switch (argc) {
        case 2:
            last = atoi(argv[1]);
            break;
        case 3:
            first = atoi(argv[1]);
            last = atoi(argv[2]);
            break;
        case 4:
            first = atoi(argv[1]);
            increment = atoi(argv[2]);
            last = atoi(argv[3]);
            break;
        default:
            usage(1);
            break;
    }
    
    // Generate sequence
    List *l = generate_sequence(first, increment, last);

    // Print out sequence
    while (l->size > 0) {
        printf("%ld\n", list_pop(l, 0).number);
    }

    list_delete(l, false);

    return EXIT_SUCCESS;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
