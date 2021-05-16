#include "linkedlist.h"
#include <SDL2/SDL.h>

/* This will assume a cirular linked list with sentinel node */
void addLink(LinkNode *sentinel, void *data) {
    LinkNode *node = SDL_malloc(sizeof(LinkNode));
    node->data = data;
    node->prev = sentinel;
    node->next = sentinel->next;
    // works even if only the sentinel exists
    sentinel->next->prev = node;
    sentinel->next = node;
}

/* This will assume that the data in the node has been freed already */
void removeLink(LinkNode *node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
    SDL_free(node);
}