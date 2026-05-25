/* nmapit.c: Simple network port scanner */

#include "socket.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netdb.h>
#include <signal.h>

#ifndef GNU_SOURCE
typedef void (*sighandler_t)(int);
#endif

/* Functions */

/**
 * Display usage message and exit.
 * @param   status      Exit status
 **/
void    usage(int status) {
    fprintf(stderr, "Usage: nmapit [-p START-END] HOST\n");
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "    -p START-END    Specifies the range of port numbers to scan\n");
    exit(status);
}

/**
 * Handle alarm signal.
 * @param   signum      Signal number
 **/
void sigalrm_handler(int signum) {
    // Cancel current alarm
    alarm(0);
}

/**
 * Parse port range string into start and end port integers.
 * @param   range       Port range string (ie. START-END)
 * @param   start       Pointer to starting port integer
 * @param   end         Pointer to ending port integer
 * @return  true if parsing both start and end were successful, otherwise false
 **/
bool parse_ports(char *range, int *start, int *end) {
    // Parse starting port
    char *start_str = strtok(range, "-");
    if (start_str)
        *start = atoi(start_str);
    else
        return false;

    // Parse ending port
    char *end_str = strtok(NULL, "-");
    if (end_str)
        *end = atoi(end_str);
    else
        return false;

    return true;
}

/**
 * Scan ports at specified host from starting and ending port numbers
 * (inclusive).
 * @param   host        Host to scan
 * @param   start       Starting port number
 * @param   end         Ending port number
 * @return  true if any port is found, otherwise false
 **/
bool scan_ports(const char* host, int start, int end) {
    // Register signal handler for alarm
    struct sigaction action = {.sa_handler = sigalrm_handler};
    sigaction(SIGALRM, &action, NULL);

    // For each port, set alarm, attempt to dial host and port
    bool port_found = false;
    for (int i = start; i <= end; i++) {
        char port[BUFSIZ];
        sprintf(port, "%d", i);
        alarm(1);
        FILE *client_file;
        if ((client_file = socket_dial(host, port))) {
            printf("%s\n", port);
            port_found = true;
            fclose(client_file);
        }
    }
    return port_found;
}

/* Main Execution */

int main(int argc, char *argv[]) {
    // Parse command-line arguments
    if (argc > 4 || argc < 2) usage(1);

    char *host = NULL;
    int start = 1;
    int end = 1023;

    if (argc == 2) {
        if (strcmp(argv[1], "-h") == 0)
            usage(0);
        else
            host = argv[1];
    } else if (argc == 4) {
        if (strcmp(argv[1], "-p") == 0) {
            if (!parse_ports(argv[2], &start, &end)) usage(1);
            host = argv[3];
        } else 
            usage(1);
    } else 
        usage(1);


    // Scan ports 
    if (scan_ports(host, start, end))
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
