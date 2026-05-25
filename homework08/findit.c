/* findit.c: Search for files in a directory hierarchy */

#include "findit.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <unistd.h>

/* Macros */

#define	streq(a, b) (strcmp(a, b) == 0)

/* Functions */

/**
 * Print usage message and exit with status
 * @param   status      Exit status
 **/
void usage(int status) {
    fprintf(stderr, "Usage: findit PATH [OPTIONS]\n\n");
    fprintf(stderr, "Options:\n\n");
    fprintf(stderr, "   -type [f|d]	File is of type f for regular file or d for directory\n");
    fprintf(stderr, "   -name pattern	Name of file matches shell pattern\n");
    fprintf(stderr, "   -executable	File is executable or directory is searchable by user\n");
    fprintf(stderr, "   -readable	File is readable by user\n");
    fprintf(stderr, "   -writable	File is writable by user\n");
    exit(status);
}

/**
 * Recursively walk specified directory, adding all file system entities to
 * specified files list.
 * @param   root        Directory to walk
 * @param   files       List of files found
 **/
void	find_files(const char *root, List *files) {
    // Add root to files
    list_append(files, (Data)strdup(root));

    // Walk directory
    //  - Skip current and parent directory entries
    //  - Form full path to entry
    //  - Recursively walk directories or add entry to files list
    DIR *d = opendir(root);
    if (!d) {
        fprintf(stderr, "Error in opendir(%s)\n", root);
        return;
    }

    for (struct dirent *e = readdir(d); e; e = readdir(d)) {
        if (streq(e->d_name, ".") || streq(e->d_name, "..")) {
            continue;
        } 
        
        char path[BUFSIZ];
        sprintf(path, "%s/%s", root, e->d_name);

        struct stat s;
        if (lstat(path, &s) != 0) {
            fprintf(stderr, "Error in stat(%s)\n", path);
            continue;
        }
        // if entry is a directory, recurse
        // otherwise, add entry to files
        if ((s.st_mode & S_IFMT) == S_IFDIR)
            find_files(path, files);
        else
            list_append(files, (Data)strdup(path));
    }
    closedir(d);
}

/**
 * Iteratively filter list of files with each filter in list of filters.
 * @param   files       List of files
 * @param   filters     List of filters
 * @param   options     Pointer to options structure
 **/
void	filter_files(List *files, List *filters, Options *options) {
    // Apply each filter to list of files
    Node *curr = filters->head;
    while (curr) {
        list_filter(files, curr->data.function, options, true);
        curr = curr->next;
    }
}

/* Main Execution */

int main(int argc, char *argv[]) {
    // Parse command line arguments
    char root[BUFSIZ];
    int start_index;
    if (argc >= 2 && streq(argv[1], "cake")) {
        // print out cake.txt, then print "the cake is a lie." 4 times
        char *path = "/escnfs/home/mzampino/public/cake.txt";
        FILE *fptr = fopen(path, "r");

        char buffer[BUFSIZ];
        while (fgets(buffer, BUFSIZ, fptr)) {
            printf("%s", buffer);
        }
        fclose(fptr);
        
        for (int i = 0; i < 4; i++) {
            sleep(1);
            puts("the cake is a lie.");
        }
        return EXIT_SUCCESS;
    }

    if (argc >= 2 && argv[1][0] != '-') {
        strcpy(root, argv[1]);
        start_index = 2;
    } else {
        strcpy(root, ".");
        start_index = 1;
    }

    List *filters = calloc(1, sizeof(List));
    Options options = {0};

    for (int i = start_index; i < argc; i++) {
        if (streq(argv[i], "-type")) {
            list_append(filters, (Data)filter_by_type);
            // consume additional argument
            i++;
            if (i >= argc) usage(1);
            if (strcmp(argv[i], "f") == 0)
                options.type = S_IFREG;
            if (strcmp(argv[i], "d") == 0)
                options.type = S_IFDIR;
        }
        else if (streq(argv[i], "-name")) {
            list_append(filters, (Data)filter_by_name);
            // consume additional argument
            i++;
            if (i >= argc) usage(1);
            options.name = argv[i];
        }
        else if (streq(argv[i], "-executable")) {
            list_append(filters, (Data)filter_by_mode);
            options.mode = X_OK;
        }
        else if (streq(argv[i], "-readable")) {
            list_append(filters, (Data)filter_by_mode);
            options.mode = R_OK;
        }
        else if (streq(argv[i], "-writable")) {
            list_append(filters, (Data)filter_by_mode);
            options.mode = W_OK;
        }
        else if (streq(argv[i], "-h")) {
            usage(0);
        }
        else {
            usage(1);
        }
    }

    // Find files, filter files, print files
    List *files = calloc(1, sizeof(List));
    find_files(root, files);
    filter_files(files, filters, &options);
    list_output(files, stdout);

    // free allocated memory from the heap
    node_delete(filters->head, false, true);
    free(filters);
    node_delete(files->head, true, true);
    free(files);
    return EXIT_SUCCESS;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
