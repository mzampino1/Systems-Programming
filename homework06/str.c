/* str.c: string library */

#include "str.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

/* Functions */

/**
 * Convert string to lowercase.
 * @param   s	    String to convert
 * @param   w	    Pointer to buffer that holds result of conversion
 **/
void	str_lower(const char *s, char *w) {
    for (const char *c = s; *c; c++) {
        *w = tolower(*c);
        w++;
    }
    *w = '\0';
}

/**
 * Convert string to uppercase.
 * @param   s	    String to convert
 * @param   w	    Pointer to buffer that holds result of conversion
 **/
void	str_upper(const char *s, char *w) {
    for (const char *c = s; *c; c++) {
        *w = toupper(*c);
        w++;
    }
    *w = '\0';
}

/**
 * Convert string to titlecase.
 * @param   s	    String to convert
 * @param   w	    Pointer to buffer that holds result of conversion
 **/
void	str_title(const char *s, char *w) {
    // make first character uppercase
    *w = toupper(*s);

    // adjust capitalization for rest of string
    while(*s) {
        if (!isalpha(*s)) {
            s++;
            w++;
            *w = toupper(*s);
        } else {
            s++;
            w++;
            *w = tolower(*s);
        }
    }
    *w = '\0';
}

/**
 * Strip characters from back of string (if present).
 * @param   s	    String to strip
 * @param   chars   Characters to strip (if NULL, then all whitespace)
 * @param   w	    Pointer to buffer that holds result of strip
 **/
void	str_rstrip(const char *s, const char *chars, char *w) {
    // make lookup table where index = ASCII value, and a 1 means to strip that character
    int table[256] = {0};
    if (!chars) {
        for (int i = 0; i < 256; i++) {
            if (isspace((char)i)) table[i] = 1;
        }
    } else {
        while (*chars) {
            table[(int)*chars] = 1;
            chars++;
        }
    }
    
    char *w_start = w;
    // copy s to w
    for (const char *c = s; *c; c++) {
        *w = *c;
        w++;
    }
    *w = '\0';
    
    // go through w from right to left, putting NUL after the last
    // non-stripped character
    w--;
    while (w >= w_start) {
        if (table[(int)*w] == 0) break;
        w--;
    }
    w++;
    *w = '\0';
}

/**
 * Delete characters from string.
 * @param   s	    String to delete from
 * @param   chars   Characters to delete
 * @param   w	    Pointer to buffer that holds result of deletion
 **/
void	str_delete(const char *s, const char *chars, char *w) {
    // make lookup table where index = ASCII value, and a 1 means to delete that character
    int table[256] = {0};
    while(*chars) {
        table[(int)*chars] = 1;
        chars++;
    }

    // go through s, copying only non-deleted characters to w
    while (*s) {
        if (table[(int)*s] == 0) {
            *w = *s;
            w++;
        }
        s++;
    }
    *w = '\0';
}

/**
 * Translate characters in 'from' with corresponding characters in 'to'.
 * @param   s       String to translate
 * @param   from    String with characters to translate
 * @param   to      String with corresponding translation characters
 * @param   w	    Pointer to buffer that holds result of translation
 **/
void	str_translate(const char *s, const char *from, const char *to, char *w) {
    // make lookup table where index = ASCII value, a 0 means to leave character alone
    // and a non-zero value means to translate to that char
    int table[256] = {0};
    while(*from && *to) {
        table[(int)*from] = *to;
        from++;
        to++;
    }
    
    // go through s, copying to w and translating when necessary
    while (*s) {
        if (table[(int)*s] == 0) {
            *w = *s;
        } else {
            *w = table[(int)*s];
        }
        w++;
        s++;
    }
    *w = '\0';
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
