/*
 * Starter file for CS 208 assignment: Queues in C
 * Adapted by Aaron Bauer and then Jeff Ondich
 * from a lab developed at CMU by R. E. Bryant, 2017-2018
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

list_ele_t *insert_node(char *s);

/*
  Create empty queue.
  Return NULL if could not allocate space.
*/
queue_t *q_new()
{
    queue_t *q =  malloc(sizeof(queue_t));
    // TODO check if malloc returned NULL (this means space could not be allocated)
    if (q == NULL) {
      return NULL;
    }
    q->head = NULL;
    q->tail = NULL; 
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    // What if q is NULL?

    // TODO free the queue nodes

    /* You'll want to loop through the list nodes until the next pointer is NULL,
     * starting at the head, freeing each node and its value. 
     * Account for an empty list (head is NULL). */

    // Freeing queue structure itself
    free(q);
}

/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    // Check if q is NULL
    if (q == NULL) {
      return false;
    }

    // TODO Hey wait a second. Aren't we also gonna do q_insert_tail? Should
    // we maybe write a utility function to allocate and return a new node so
    // we can reuse it there?
    list_ele_t *new_node = insert_node(s);
    if (new_node == NULL){
      return false;
    }
    // TODO if the list was empty, the tail might also need updating, however it
    // is you're implementing the notion of tail.
    if (q->tail == NULL) {
        q->tail = new_node;
    }

    new_node->next = q->head;
    q->head = new_node;

    return true;
    
}

list_ele_t *insert_node(char *s){
    list_ele_t *new_node;
    new_node = malloc(sizeof(list_ele_t)); // allocates space on a the heap for the new node
    // check if malloc returned NULL
    if (new_node == NULL) {
      return NULL;
    }
    // use malloc to allocate space for the value and copy s to value
    new_node->value = malloc(strlen(s) + 1);
    // If this second malloc call returns NULL, you need to free new_node before returning
    if (new_node->value == NULL) {
      free(new_node);
      return NULL;
    }
    strcpy(new_node->value, s);

    return new_node;


}


/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    // TODO implement in similar fashion to q_insert_head
    // You'll certainly want to add a field to queue_t so we can access
    // the tail efficiently.
    // TODO If the list was empty, the head might also need updating

    // Check if q is NULL
    if (q == NULL) {
      return false;
    }

    list_ele_t *new_node = insert_node(s);
    if (new_node == NULL){
      return false;
    }


    if (q->tail == NULL) {
        q->head = q->tail = new_node;
    } 
    else {
        q->tail->next = new_node;
        q->tail = new_node;
    }

    return true;
}

/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If sp is non-NULL and an element is removed, copy the removed string to *sp
  (up to a maximum of bufsize-1 characters, plus a null terminator.)
  The space used by the list element and the string should be freed.
*/
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    // TODO check if q is NULL or empty
    // TODO if sp is not NULL, copy value at the head to sp
    // Use strncpy (http://www.cplusplus.com/reference/cstring/strncpy/)
    // bufsize is the number of characters already allocated for sp
    // Think about:
    //    - what should happen if q->head->value is longer than bufsize?
    //    - what should happen if bufsize == 0?
    //    - under what conditions will strncpy copy the \0 character
    //        into sp, and when will it fail to do so (so you'll have
    //        to insert a \0 manually)?

    // TODO update q->head to remove the current head from the queue
    q->head = q->head->next;
    // TODO if the last list element was removed, the tail might need updating
    // TODO hey, did you forget to free the removed list element?
    return true;
}

/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    // TODO implement this function. If you add a field to queue_t
    // to keep track of the number of nodes in the queue, then this
    // function is fast-running and easy to write. But it also means
    // you have to be very careful about keeping track of the number
    // of nodes elsewhere in your code.

    // TODO what if q == NULL or q->head == NULL?

    return 0;
}

/*
  Reverse elements in queue
  No effect if q is NULL or empty
  This function should not allocate or free any list elements
  (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
  It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    // TODO good luck! this is fun when it works
}
