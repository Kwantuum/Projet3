/* ========================================================================= *
 * LinkedList interface:
 * ========================================================================= */

#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include <stddef.h>
#include <stdbool.h>


typedef struct llnode_t {
    const void* value;
    struct llnode_t* next;

} LLNode;

typedef struct linkedlist_t {
    size_t size;
    LLNode* head;
    LLNode* last;
}LinkedList;

/* ------------------------------------------------------------------------- *
 * Creates an empty LinkedList
 *
 * The LinkeedList must later be deleted by calling `freeLinkedList.
 *
 * RETURN
 * linkedList    A pointer to the LinkedList, or NULL in case of error
 *
 * ------------------------------------------------------------------------- */

LinkedList* newLinkedList(void);


/* ------------------------------------------------------------------------- *
 * Frees the allocated memory of the given LinkedList.
 *
 * PARAMETERS
 * ll               A valid pointer to a LinkedList object
 * freeValue_fn_t   A function which can free any individual value of the list
 *                  if NULL, only the structure (i.e. not the content) is freed
 *
 * EXAMPLE OF USAGES
 * freeLinkedList(ll, &free);
 * freeLinkedList(ll, (void (*)(void *)) &freeSignal); //Notice the cast
 *
 *
 * NOTE
 * The const qualifier will exceptionally be discarded freeValue_fn_t is not
 * NULL
 * ------------------------------------------------------------------------- */

void freeLinkedList(LinkedList* ll, void freeValue_fn_t(void*));

/* ------------------------------------------------------------------------- *
 * Counts the number of elements stored in the given LinkedList.
 *
 * PARAMETERS
 * ll           A valid pointer to a LinkedList object
 *
 * RETURN
 * nb           The amount of elements stored in linked list
 * ------------------------------------------------------------------------- */

size_t sizeOfLinkedList(const LinkedList* ll);


/* ------------------------------------------------------------------------- *
 * Inserts a new element in the linked list.
 *
 * PARAMETERS
 * ll           A valid pointer to a LinkedList object
 * value        The value to store
 *
 * RETURN
 * res          A boolean equal to true if the new element was successfully
 *              inserted, false otherwise (error)
 * ------------------------------------------------------------------------- */

bool insertInLinkedList(LinkedList* ll, const void* value);



#endif // !_LINKED_LIST_H_
