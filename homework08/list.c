/* list.c: Singly Linked List */

#include "findit.h"

#include <stdlib.h>

/* Node Functions */

/**
 * Allocate a new Node structure.
 * @param   data        Data value
 * @param   next        Pointer to next Node structure
 * @return  Pointer to new Node structure (must be deleted).
 **/
Node *  node_create(Data data, Node *next) {
    Node *n = calloc(1, sizeof(Node));
    n->next = next;
    n->data = data;
    return n;
}

/**
 * Deallocate Node structure.
 * @param   n           Pointer to Node structure
 * @param   release     Whether or not to free Data string
 * @param   recursive   Whether or not to recursively delete next Node structure
 **/
void    node_delete(Node *n, bool release, bool recursive) {
    // Handle base case, recursive case, and then release data and Node
    if (n == NULL)
        return;
    if (recursive)
        node_delete(n->next, release, recursive);
    if (release)
        free(n->data.string);
    free(n);
}

/* List Functions */

/**
 * Append data to end of specified List.
 * @param   l           Pointer to List structure
 * @param   data        Data value to append
 **/
void    list_append(List *l, Data data) {
    // Create new Node Structure and add to end of List

    // Handle empty list case
    if (l->tail == NULL) {
        l->head = node_create(data, NULL);
        l->tail = l->head;
    } else {
        l->tail->next = node_create(data, NULL);
        l->tail = l->tail->next;
    }
}

/**
 * Filter list by applying the filter function to each Data string in List with
 * the given options:
 *
 *  - If filter function returns true, then keep current Node.
 *  - Otherwise, remove current Node from List and delete it.
 *
 * @param   l           Pointer to List structure
 * @param   filter      Filter function to apply to each Data string
 * @param   options     Pointer to Options structure to use with filter function
 * @param   release     Whether or not to release data string when deleting Node
 **/
void    list_filter(List *l, Filter filter, Options *options, bool release) {
    // Iterate through List and apply filter function to each Data string
    // to determine whether or not to keep the Node.
    
    // If list is empty, do nothing
    if (l->head == NULL) return;

    Node *prev = NULL;
    Node *curr = l->head;
    while (curr) {
        // remove node if filter returns false
        if (!filter(curr->data.string, options)) {
            // removing head or removing from middle of list
            if (curr == l->head)
                l->head = curr->next;
            else
                prev->next = curr->next;
            
            // removing tail
            if (curr == l->tail)
                l->tail = prev;
            
            Node *to_del = curr;
            curr = curr->next;
            node_delete(to_del, release, false);
        } else {
            // only update prev if the node is kept
            prev = curr;
            curr = curr->next;
        }
    }
}

/**
 * Output each Data string in List to specified stream.
 * @param   l           Pointer to List structure
 * @param   stream      File stream to output to
 **/
void    list_output(List *l, FILE *stream) {
    // Iterate though List and output each Data string to given stream
    // (one string per line).
    Node *curr = l->head;
    while (curr) {
        fprintf(stream, "%s\n", curr->data.string);
        curr = curr->next;
    }
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
