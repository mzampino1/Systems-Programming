/* walk.c */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

int walk(const char *root) {
    // Open directory handle
    DIR *d = opendir(root);
    if (!d) {
        fprintf(stderr, "%s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    // For each directory entry, check if it is a file, and print out its name
    // and file size
    for (struct dirent *e = readdir(d); e; e = readdir(d)) {
        // Skip current and parent directories
        if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0) {
            continue;
        }

        // Skip non-regular files
        if (e->d_type != DT_REG) {
            continue;
        }

        // Construct full path to file
        char path[BUFSIZ];
        sprintf(path, "%s/%s", root, e->d_name);

        // Get file meta-data
        struct stat s;
        if (stat(path, &s) < 0) {
            fprintf(stderr, "%s\n", strerror(errno));
            continue;
        }

        // Display file name and size
        printf("%s %lu\n", e->d_name, s.st_size);
    }

    // Close directory handle
    closedir(d);

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    char *path = ".";
    if (argc > 1) {
        path = argv[1];
    }

    return walk(path);
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
