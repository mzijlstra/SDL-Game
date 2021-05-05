#ifndef LINKEDLIST_INCLUDED
#define LINKEDLIST_INCLUDED

#include <stdlib.h>

typedef struct LinkNode {
    struct LinkNode *next;
    struct LinkNode *prev;
    void *data;
} LinkNode;

/* This assumes a cirular linked list with a sentinel node */
void addLink(LinkNode *sentinel, void *data);
/* This assumes that the data in the node has been freed already */
void removeLink(LinkNode *node);

#endif