#include "linkedlist.h"

/* This will assume a cirular linked list with sentinel node */
void addLink(LinkNode *sentinel, void *data) {
    LinkNode *node = malloc(sizeof(LinkNode));
    node->data = data;
    node->prev = sentinel;
    node->next = sentinel->next;
    sentinel->next->prev = node;
    sentinel->next = node;
}

/* This will assume that the data in the node has been freed already */
void removeLink(LinkNode *node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
    free(node);
}