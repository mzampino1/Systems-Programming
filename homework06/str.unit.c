/* str.unit.c: string library unit test */

#include "str.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Structures */

typedef struct {
    const char *source;
    const char *target;
} Pair;

typedef struct {
    const char *source;
    const char *chars;
    const char *target;
} Triple;

typedef struct {
    const char *source;
    const char *from;
    const char *to;
    const char *target;
} Quad;

/* Macros */

#define streq(a, b) (strcmp(a, b) == 0)

/* Tests */

int test_00_str_lower() {
    Pair cases[] = {
        {"", ""},
        {" " , " "},
        {"abc", "abc"},
        {"ABC", "abc"},
        {"AbC", "abc"},
        {NULL, NULL},
    };

    for (Pair *c = cases; c->source && c->target; c++) {
        char writer[BUFSIZ];
        str_lower(c->source, writer);

        assert(streq(writer, c->target));
    }

    return EXIT_SUCCESS;
}

int test_01_str_upper() {
    Pair cases[] = {
        {"", ""},
        {" " , " "},
        {"abc", "ABC"},
        {"ABC", "ABC"},
        {"AbC", "ABC"},
        {NULL, NULL},
    };

    for (Pair *c = cases; c->source && c->target; c++) {
        char writer[BUFSIZ];
        str_upper(c->source, writer);

        assert(streq(writer, c->target));
    }

    return EXIT_SUCCESS;
}

int test_02_str_title() {
    Pair cases[] = {
        {"", ""},
        {"abc", "Abc"},
        {"ABC", "Abc"},
        {"AbC", "Abc"},
        {"a b c", "A B C"},
        {"aa b0b c!!c", "Aa B0B C!!C"},
        {NULL, NULL},
    };

    for (Pair *c = cases; c->source && c->target; c++) {
        char writer[BUFSIZ];
        str_title(c->source, writer);

        assert(streq(writer, c->target));
    }

    return EXIT_SUCCESS;
}

int test_03_str_rstrip() {
    Pair cases1[] = {
        {"", ""},
        {"\n", ""},
        {" abc ", " abc"},
        {" abc\n", " abc"},
        {"\na b c", "\na b c"},
        {NULL, NULL},
    };

    for (Pair *c = cases1; c->source && c->target; c++) {
        char writer[BUFSIZ];
        str_rstrip(c->source, NULL, writer);

        assert(streq(writer, c->target));
    }
    
    Pair cases2[] = {
        {"", ""},
        {"\n", ""},
        {" abc ", " abc "},
        {" abc\n", " abc"},
        {"\na b c", "\na b c"},
        {NULL, NULL},
    };

    for (Pair *c = cases2; c->source && c->target; c++) {
        char writer[BUFSIZ];
        str_rstrip(c->source, "\n", writer);

        assert(streq(writer, c->target));
    }

    return EXIT_SUCCESS;
}

int test_04_str_delete() {
    Triple cases[] = {
        {"", "", ""},
        {"hello", "", "hello"},
        {"hello", "le", "ho"},
        {"hello world", "le", "ho word"},
        {" hello world ", " aeiou", "hllwrld"},
        {NULL, NULL, NULL},
    };

    for (Triple *c = cases; c->source && c->target; c++) {
        char writer[BUFSIZ];
        str_delete(c->source, c->chars, writer);

        assert(streq(writer, c->target));
    }

    return EXIT_SUCCESS;
}

int test_05_str_translate() {
    Quad cases[] = {
        {"", "", "", ""},
        {"hello", "", "430", "hello"},
        {"hello", "aeo", "", "hello"},
        {"hello", "aeo", "430", "h3ll0"},
        {" hello world ", "aeo", "430", " h3ll0 w0rld "},
        {" hello world ", "aeol", "4301", " h3110 w0r1d "},
        {" hello ", "oleh", "ymam", " mammy "},
        {NULL, NULL, NULL, NULL},
    };

    for (Quad *c = cases; c->source && c->target; c++) {
        char writer[BUFSIZ];
        str_translate(c->source, c->from, c->to, writer);

        assert(streq(writer, c->target));
    }

    return EXIT_SUCCESS;
}

/* Main Execution */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s NUMBER\n\n", argv[0]);
        fprintf(stderr, "Where NUMBER is right of the following:\n");
        fprintf(stderr, "    0  Test str_lower\n");
        fprintf(stderr, "    1  Test str_upper\n");
        fprintf(stderr, "    2  Test str_title\n");
        fprintf(stderr, "    3  Test str_rstrip\n");
        fprintf(stderr, "    4  Test str_delete\n");
        fprintf(stderr, "    5  Test str_translate\n");
        return EXIT_FAILURE;
    }

    int number = atoi(argv[1]);
    int status = EXIT_FAILURE;

    switch (number) {
        case 0:  status = test_00_str_lower(); break;
        case 1:  status = test_01_str_upper(); break;
        case 2:  status = test_02_str_title(); break;
        case 3:  status = test_03_str_rstrip(); break;
        case 4:  status = test_04_str_delete(); break;
        case 5:  status = test_05_str_translate(); break;
        default: fprintf(stderr, "Unknown NUMBER: %d\n", number); break;
    }

    return status;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
