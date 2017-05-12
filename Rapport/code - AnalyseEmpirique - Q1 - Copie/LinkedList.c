/* ========================================================================= *
 * LinkedList definition
 * ========================================================================= */


#include <stddef.h>
#include <stdlib.h>
#include "LinkedList.h"



LinkedList* newLinkedList(void)
{
    LinkedList* ll = malloc(sizeof(LinkedList));
    if (!ll)
        return NULL;
    ll->head = NULL;
    ll->last = NULL;
    ll->size = 0;
    return ll;
}



void freeLinkedList(LinkedList* ll, void freeValue_fn_t(void*))
{
    // Free LLNodes
    LLNode* node = ll->head;
    LLNode* prev = NULL;
    while(node != NULL)
    {
        prev = node;
        node = node->next;
        if(freeValue_fn_t)
            freeValue_fn_t((void*)prev->value); // Discard const qualifier
        free(prev);
    }
    // Free LinkedList sentinel
    free(ll);
}


size_t sizeOfLinkedList(const LinkedList* ll)
{
    return ll->size;
}


bool insertInLinkedList(LinkedList* ll, const void* value)
{
    LLNode* node = malloc(sizeof(LLNode));
    if(!node)
        return false;
    // Initialisation
    node->next = NULL;
    node->value = value;
    // Adding the node to the list
    if(!ll->last)
    {
        // First element in the list
        ll->last = node;
        ll->head = node;
    } else {
        //At least one element in the list
        ll->last->next = node;
        ll->last = node;
    }
    // In both cases, increment size
    ll->size++;
    return true;
}
