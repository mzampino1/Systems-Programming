/* list.unit.c: List Structure Unit Test */

#include "list.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Constants */

const int64_t NUMBERS[] = {5, 7, 4, -1};
const char   *STRINGS[] = {"Orange", "South Bend", "Eau Claire", NULL};

/* Tests */

int test_00_list_create() {
    List *l = list_create();
    assert(l);
    assert(l->sentinel.value.number == 0);
    assert(l->sentinel.next         == &l->sentinel);
    assert(l->sentinel.prev         == &l->sentinel);
    assert(l->size                  == 0);
    free(l);
    return EXIT_SUCCESS;
}

int test_01_list_delete() {
    List *l0 = list_create();
    assert(l0);
    list_delete(l0, false);

    List *l1 = list_create();
    assert(l1);
    for (const int64_t *n = NUMBERS; *n >= 0; n++) {
        l1->sentinel.next = node_create((Value)*n, l1->sentinel.next, &l1->sentinel);
        l1->size++;
    }
    list_delete(l1, false);

    List *l2 = list_create();
    assert(l2);
    for (const char **s = STRINGS; *s; s++) {
        l2->sentinel.next = node_create((Value)strdup(*s), l2->sentinel.next, &l2->sentinel);
        l2->size++;
    }
    list_delete(l2, true);
    return EXIT_SUCCESS;
}

int test_02_list_append() {
    List *l1 = list_create();
    assert(l1);
    for (size_t i = 0; NUMBERS[i] >= 0; i++) {
        list_append(l1, (Value)NUMBERS[i]);
        assert(l1->size == (i + 1));
    }
    assert(l1->sentinel.next->value.number              == NUMBERS[0]);
    assert(l1->sentinel.next->next->value.number        == NUMBERS[1]);
    assert(l1->sentinel.next->next->next->value.number  == NUMBERS[2]);
    assert(l1->sentinel.prev->value.number              == NUMBERS[2]);
    assert(l1->sentinel.prev->prev->value.number        == NUMBERS[1]);
    assert(l1->sentinel.prev->prev->prev->value.number  == NUMBERS[0]);
    list_delete(l1, false);

    List *l2 = list_create();
    assert(l2);
    for (size_t i = 0; STRINGS[i]; i++) {
        list_append(l2, (Value)strdup(STRINGS[i]));
        assert(l2->size == (i + 1));
    }
    assert(strcmp(l2->sentinel.next->value.string            , STRINGS[0]) == 0);
    assert(strcmp(l2->sentinel.next->next->value.string      , STRINGS[1]) == 0);
    assert(strcmp(l2->sentinel.next->next->next->value.string, STRINGS[2]) == 0);
    assert(strcmp(l2->sentinel.prev->value.string            , STRINGS[2]) == 0);
    assert(strcmp(l2->sentinel.prev->prev->value.string      , STRINGS[1]) == 0);
    assert(strcmp(l2->sentinel.prev->prev->prev->value.string, STRINGS[0]) == 0);
    list_delete(l2, true);
    return EXIT_SUCCESS;
}

int test_03_list_pop() {
    List *l1 = list_create();
    assert(l1);
    for (size_t i = 0; NUMBERS[i] >= 0; i++) {
        list_append(l1, (Value)NUMBERS[i]);
        assert(l1->size == (i + 1));
    }

    Value v1 = list_pop(l1, 1);
    assert(v1.number == NUMBERS[1]);
    assert(l1->sentinel.next->value.number              == NUMBERS[0]);
    assert(l1->sentinel.next->next->value.number        == NUMBERS[2]);
    assert(l1->sentinel.prev->value.number              == NUMBERS[2]);
    assert(l1->sentinel.prev->prev->value.number        == NUMBERS[0]);
    assert(l1->size == 2);

    Value v2 = list_pop(l1, 1);
    assert(v2.number == NUMBERS[2]);
    assert(l1->sentinel.next->value.number              == NUMBERS[0]);
    assert(l1->sentinel.prev->value.number              == NUMBERS[0]);
    assert(l1->size == 1);

    Value vX= list_pop(l1, 1);
    assert(vX.number == -1L);

    Value v0 = list_pop(l1, 0);
    assert(v0.number == NUMBERS[0]);
    assert(l1->size == 0);

    list_delete(l1, false);
    return EXIT_SUCCESS;
}

/* Main Execution */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s NUMBER\n\n", argv[0]);
        fprintf(stderr, "Where NUMBER is right of the following:\n");
        fprintf(stderr, "    0  Test list_create\n");
        fprintf(stderr, "    1  Test list_delete\n");
        fprintf(stderr, "    2  Test list_append\n");
        fprintf(stderr, "    3  Test list_pop\n");
        return EXIT_FAILURE;
    }

    int number = atoi(argv[1]);
    int status = EXIT_FAILURE;

    switch (number) {
        case 0:  status = test_00_list_create(); break;
        case 1:  status = test_01_list_delete(); break;
        case 2:  status = test_02_list_append(); break;
        case 3:  status = test_03_list_pop(); break;
        default: fprintf(stderr, "Unknown NUMBER: %d\n", number); break;
    }

    return status;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
