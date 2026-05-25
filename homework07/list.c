/* list.c: List Structure */

#include "list.h"

/* List Functions */

/**
 * Create a List structure.
 *
 * @return  Pointer to new List structure (must be deleted later).
 **/
List *	list_create() {
    List *new_list = calloc(1, sizeof(List));
    new_list->sentinel.next = &new_list->sentinel;
    new_list->sentinel.prev = &new_list->sentinel;
    return new_list;
}

/**
 * Delete List structure.
 *
 * @param   l       Pointer to List structure.
 * @param   release Whether or not to release the string values.
 **/
void	list_delete(List *l, bool release) {
    Node *next_node = NULL;
    // free each node
    for (Node *curr = l->sentinel.next; curr != &l->sentinel; curr = next_node) {
        next_node = curr->next;
        node_delete(curr, release);
    }
    // free list struct
    free(l);
}

/**
 * Add new Value to back of List structure.
 *
 * @param   l       Pointer to List structure.
 * @param   v       Value to add to back of List structure.
 **/
void    list_append(List *l, Value v) {
    Node *new_node = node_create(v, &l->sentinel, l->sentinel.prev);
    l->sentinel.prev->next = new_node;
    l->sentinel.prev = new_node;
    l->size++;
}

/**
 * Remove Value at specified index from List structure.
 *
 * @param   l       Pointer to List structure.
 * @param   index   Index of Value to remove from List structure.
 *
 * @return  Value at index in List structure (-1 if index is out of bounds).
 **/
Value   list_pop(List *l, size_t index) {
    // if index is out of range, return -1L
    if (index >= l->size) return (Value)-1L;
    
    // find node at given index
    int i = 0;
    Node *node = l->sentinel.next;
    while (i != index) {
        node = node->next;
        i++;
    }
    Value val = node->value;

    // remove node from list
    Node *next_node = node->next;
    Node *prev_node = node->prev;
    prev_node->next = next_node;
    next_node->prev = prev_node;

    free(node);
    l->size--;

    // return node's value
    return val;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
