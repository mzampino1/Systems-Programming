/* curlit.c: Simple HTTP client*/

#include "socket.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <netdb.h>

/* Constants */

#define HOST_DELIMITER  "://"
#define PATH_DELIMITER  '/'
#define PORT_DELIMITER  ':'
#define BILLION         (1000000000.0)
#define MEGABYTES       (1<<20)

/* Macros */

#define streq(a, b) (strcmp(a, b) == 0)

/* Structures */

typedef struct {
    char host[NI_MAXHOST];
    char port[NI_MAXSERV];
    char path[PATH_MAX];
} URL;

/* Functions */

/**
 * Display usage message and exit.
 * @param   status      Exit status.
 **/
void    usage(int status) {
    fprintf(stderr, "Usage: curlit [-h] URL\n");
    exit(status);
}

/**
 * Parse URL string into URL structure.
 * @param   s       URL string
 * @param   url     Pointer to URL structure
 **/
void    parse_url(const char *s, URL *url) {
    // Copy data to local buffer
    char buffer[BUFSIZ];
    strcpy(buffer, s);

    // Skip scheme to host
    char *host = strstr(buffer, HOST_DELIMITER);
    if (host) {
        host += strlen(HOST_DELIMITER);
    } else {
        host = buffer;
    }

    // Split host:port from path
    char *path = strchr(host, PATH_DELIMITER);
    if (path) {
        *path = '\0';
        path++;
    } else {
        path = "";
    }

    // Split host and port
    char *port = strchr(host, PORT_DELIMITER);
    if (port) {
        *port = '\0';
        port++;
    } else {
        port = "80";
    }

    // Copy components to URL
    strcpy(url->host, host);
    strcpy(url->port, port);
    strcpy(url->path, path);
}

/**
 * Fetch contents of URL and print to standard out.
 *
 * Print elapsed time and bandwidth to standard error.
 * @param   s       URL string
 * @param   url     Pointer to URL structure
 * @return  true if client is able to read all of the content (or if the
 * content length is unset), otherwise false
 **/
bool    fetch_url(URL *url) {
    bool read_success = true;

    // Grab start time
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Connect to remote host and port
    FILE *client_file = socket_dial(url->host, url->port);
    if (!client_file) {
        return false;
    }

    // Send request to server
    fprintf(client_file, "GET /%s HTTP/1.0\r\n", url->path);
    fprintf(client_file, "Host: %s\r\n", url->host);
    fprintf(client_file, "\r\n");

    // Read status response from server
    char buffer[BUFSIZ];
    fgets(buffer, BUFSIZ, client_file);
    if (!strstr(buffer, "200 OK"))
        read_success = false;

    // Read response headers from server
    int content_length = -1;
    while (fgets(buffer, BUFSIZ, client_file) && strlen(buffer) > 2) {
        // Check for Content-Length
        sscanf(buffer, "Content-Length: %d", &content_length);
    }

    // Read response body from server
    int total_bytes_read = 0;
    int bytes_read;
    while ((bytes_read = fread(buffer, sizeof(char), BUFSIZ, client_file))) {
        total_bytes_read += bytes_read;
        fwrite(buffer, sizeof(char), bytes_read, stdout);
    }
    
    if (content_length > -1 && total_bytes_read != content_length)
        read_success = false;

    // Grab end time
    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed = end.tv_sec - start.tv_sec;
    elapsed += (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    // Output metrics
    double bandwidth = (total_bytes_read / elapsed) / MEGABYTES;

    fprintf(stderr, "Time Elapsed: %.2lf s\n", elapsed);
    fprintf(stderr, "Bandwidth:    %.2lf MB/s\n", bandwidth);

    fclose(client_file);

    return read_success;
}

/* Main Execution */

int     main(int argc, char *argv[]) {
    // Parse command line options
    char *url_str;
    if (argc != 2) {
        usage(1);
    } else {
        if (streq(argv[1], "-h")) {
            usage(0);
        } else if (argv[1][0] != '-') {
            url_str = argv[1];
        } else usage(1);
    }

    // Parse URL
    URL url;
    parse_url(url_str, &url);

    // Fetch URL
    if (fetch_url(&url))
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
