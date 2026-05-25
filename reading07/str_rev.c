/* str_rev: Reverse all characters in string */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *str_rev(const char *s) {
    char *t = strdup(s);

    const char *r = s + strlen(s) - 1;
    char *w = t;
    while (r >= s) {
        *w++ = *r--;
    }
    return t;
}

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        char *t = str_rev(argv[i]);
        puts(t);
		free(t);
    }
    return 0;
}
