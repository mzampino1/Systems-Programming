/* moveit.c: Interactive Move Command */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

/* Macros */

#define streq(a, b) (strcmp(a, b) == 0)
#define strchomp(s) (s)[strlen(s) - 1] = 0

/* Functions */

/**
 * Display usage message and exit.
 * @param   status      Exit status.
 **/
void    usage(int status) {
    fprintf(stderr, "Usage: moveit files...\n");
    exit(status);
}

/**
 * Save list of file paths to temporary file.
 * @param   files       Array of path strings.
 * @param   n           Number of path strings.
 * @return  Newly allocated path to temporary file (must be freed).
 **/
char *  save_files(char **files, size_t n) {
    // Create temporary file
    char tpath[] = "moveit.XXXXXX";
    int tfd = mkstemp(tpath);
    if (tfd < 0) {
        fprintf(stderr, "mkstemp(): %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Open temporary file for writing
    FILE *stream = fdopen(tfd, "w");
    if (!stream) {
        close(tfd);
        exit(EXIT_FAILURE);
    }

    // Write paths to temporary file
    for (int i = 0; i < n; i++) {
        fputs(files[i], stream);
        fputc('\n', stream);
    }

    fclose(stream);

    return strdup(tpath);
}

/**
 * Run $EDITOR on specified path.
 * @param   path        Path to file to edit.
 * @return  Whether or not the $EDITOR process terminated successfully.
 **/
bool    edit_files(const char *path) {
    // Get EDITOR from environment (default to vim if not found)
    char *editor = getenv("EDITOR");
    if (editor == NULL) editor = "vim";

    // Fork process
    //  1. Child: execute editor on path
    //  2. Parent: wait for child
    pid_t pid = fork();

    int status;

    switch (pid) {
        case 0:     // Child
            execlp(editor, editor, path, NULL);
            exit(EXIT_FAILURE);
        case -1:    // Parent - Failure
            fprintf(stderr, "fork(): %s\n", strerror(errno));
            return false;
        default:    // Parent - Success
            pid = wait(&status);
            break;
    }

    // Return exit status of child process
    return WEXITSTATUS(status) == EXIT_SUCCESS;
}

/**
 * Rename files as specified in contents of path.
 * @param   files       Array of old path names.
 * @param   n           Number of old path names.
 * @param   path        Path to file with new names.
 * @return  Whether or not all rename operations were successful.
 **/
bool    move_files(char **files, size_t n, const char *path) {
    // Open temporary file at path for reading
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "open(%s): %s\n", path, strerror(errno));
        return false;
    }

    // Rename each file in array according to new name in temporary file
    // (if the names do not match)
    FILE *stream = fdopen(fd, "r");
    if (!stream) {
        close(fd);
        return false;
    }

    char new_name[BUFSIZ];
    for (int i = 0; i < n; i++) {
        if (!fgets(new_name, BUFSIZ, stream))
            break;
        strchomp(new_name);
        if (new_name[0] == '\0')
            continue; // don't rename files corresponding to empty lines
        if (rename(files[i], new_name) < 0) {
            fclose(stream);
            return false;
        }
    }

    fclose(stream);

    return true;
}

/* Main Execution */

int     main(int argc, char *argv[]) {
    // Parse command line options
    char *files[argc - 1];
    if (argc < 2) {
        usage(1);
    } else if (streq(argv[1], "-h")) {
        usage(0);
    } else {
        for (int i = 0; i < argc - 1; i++) {
            files[i] = argv[i + 1];
        }
    }

    // Save files
    char *temp_path = save_files(files, argc - 1);

    // Edit files
    if (!edit_files(temp_path)) {
        unlink(temp_path);
        free(temp_path);
        return EXIT_FAILURE;
    }

    // Move files
    if (!move_files(files, argc - 1, temp_path)) {
        unlink(temp_path);
        free(temp_path);
        return EXIT_FAILURE;
    }

    // Cleanup temporary file
    if (unlink(temp_path) < 0) {
        free(temp_path);
        return EXIT_FAILURE;
    }

    free(temp_path);

    return EXIT_SUCCESS;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
