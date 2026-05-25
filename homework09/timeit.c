/* timeit.c: Run command with a time limit */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <fcntl.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

/* Macros */

#define	streq(a, b) (strcmp(a, b) == 0)
#define strchomp(s) (s)[strlen(s) - 1] = 0
#define debug(M, ...) \
    if (Verbose) { \
        fprintf(stderr, "%s:%d:%s: " M, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
    }

#define BILLION 1000000000.0

/* Globals */

int  Timeout  = 10;
bool Verbose  = false;
int  ChildPid = 0;

/* Functions */

/**
 * Display usage message and exit.
 * @param   status      Exit status.
 **/
void	usage(int status) {
    fprintf(stderr, "Usage: timeit [options] command...\n");
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "    -t SECONDS  Timeout duration before killing command (default is %d)\n", Timeout);
    fprintf(stderr, "    -v          Display verbose debugging output\n");
    exit(status);
}

/**
 * Parse command line options.
 * @param   argc        Number of command line arguments.
 * @param   argv        Array of command line argument strings.
 * @return  Array of strings representing command to execute (must be freed).
 **/
char ** parse_options(int argc, char **argv) {
    // Iterate through command line arguments to determine Timeout and
    // Verbose flags

    // Keep track of where command starts in arguments
    int command_start = 1;
    if (argc < 2) {
        usage(1);
    } else if (streq(argv[1], "-h")) {
        usage(0);
    } else {
        for (int i = 1; i < argc; i++) {
            if (streq(argv[i], "-t")) {
                i++;
                Timeout = atoi(argv[i]);
                command_start += 2;
            } else if (streq(argv[i], "-v")) {
                Verbose = true;
                command_start++;
            } else {
                break;
            }
        }
    }

    debug("Timeout = %d\n", Timeout);
    debug("Verbose = %d\n", Verbose);

    // Copy remaining arguments into new array of strings
    char **command = calloc(argc - command_start + 1, sizeof(char *));
    int command_cnt = 0;
    for (int i = command_start; i < argc; i++) {
        command[command_cnt] = argv[i];
        command_cnt++;
    }
    if (command_cnt == 0) usage(1);

    if (Verbose) {
        // Print out new array of strings (to stderr)
        debug("Command = ");
        for (int i = 0; i < command_cnt; i++) {
            debug("%s", command[i]);
            if (i != command_cnt - 1)
                debug(" ");
        }
        debug("\n");
    }

    // Add NULL to end of command array
    command[command_cnt] = NULL;

    return command;
}

/**
 * Handle signal.
 * @param   signum      Signal number.
 **/
void    handle_signal(int signum) {
    // Kill child process gracefully, then forcefully
    debug("Killing child %d...\n", ChildPid);
    kill(ChildPid, SIGTERM);
    kill(ChildPid, SIGKILL);
}

/* Main Execution */

int	main(int argc, char *argv[]) {
    // Parse command line options
    char **commands = parse_options(argc, argv);

    // Register alarm handler and save start time
    debug("Registering handlers...\n");
    signal(SIGALRM, handle_signal);

    debug("Grabbing start time...\n");
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Fork child process
    ChildPid = fork();

    int status = 0;
    
    if (ChildPid < 0)    // Parent - Failure
        return EXIT_FAILURE;
    else if (ChildPid == 0) {
        //  1. Child executes command parsed from command line
        debug("Executing child...\n");
        execvp(commands[0], commands);
        exit(EXIT_FAILURE);
    } else {
        //  2. Parent sets alarm based on Timeout and waits for child
        debug("Sleeping for %d seconds...\n", Timeout);
        alarm(Timeout);
        debug("Waiting for child %d...\n", ChildPid);
        wait(&status);
    }

    // Print out child's exit status or termination signal
    debug("Child exit status: %d\n", status);

    // Print elapsed time
    debug("Grabbing end time...\n");
    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed = end.tv_sec - start.tv_sec;
    elapsed += (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("Time Elapsed: %0.1lf\n", elapsed);

    // Cleanup
    free(commands);

    if WIFEXITED(status)
        return WEXITSTATUS(status);
    else
        return WTERMSIG(status);
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
