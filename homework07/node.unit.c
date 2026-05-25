/* node.unit.c: Node Structure Unit Test */

#include "list.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Tests */

int test_00_node_create() {
    Value v0 = {.number = 42};
    Node *n0 = node_create(v0, NULL, NULL);
    assert(n0);
    assert(n0->value.number == v0.number);
    assert(n0->next == NULL);
    assert(n0->prev == NULL);

    Value v1 = {.string = "better now"};
    Node *n1 = node_create(v1, n0, NULL);
    assert(n1);
    assert(n1->value.string == v1.string);
    assert(n1->next == n0);
    assert(n1->prev == NULL);

    Node *n2 = node_create((Value)strdup(v1.string), n0, n1);
    assert(n2);
    assert(strcmp(n2->value.string, v1.string) == 0);
    assert(n2->next == n0);
    assert(n2->prev == n1);

    free(n0);
    free(n1);
    free(n2->value.string);
    free(n2);
    return EXIT_SUCCESS;
}

int test_01_node_delete() {
    Value v0 = {.number = 42};
    Node *n0 = node_create(v0, NULL, NULL);
    assert(n0);

    Value v1 = {.string = "better now"};
    Node *n1 = node_create(v1, n0, NULL);
    assert(n1);

    Node *n2 = node_create((Value)strdup(v1.string), n0, n1);
    assert(n2);

    node_delete(n0, false);
    node_delete(n1, false);
    node_delete(n2, true);

    return EXIT_SUCCESS;
}

/* Main Execution */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s NUMBER\n\n", argv[0]);
        fprintf(stderr, "Where NUMBER is right of the following:\n");
        fprintf(stderr, "    0  Test node_create\n");
        fprintf(stderr, "    1  Test node_delete\n");
        return EXIT_FAILURE;
    }

    int number = atoi(argv[1]);
    int status = EXIT_FAILURE;

    switch (number) {
        case 0:  status = test_00_node_create(); break;
        case 1:  status = test_01_node_delete(); break;
        default: fprintf(stderr, "Unknown NUMBER: %d\n", number); break;
    }

    return status;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
