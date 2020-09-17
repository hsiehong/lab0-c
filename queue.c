#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (q == NULL)
        return NULL;
    q->size = 0;
    q->head = NULL;
    q->tail = NULL;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (q == NULL)
        return;
    while (q->head) {
        list_ele_t *tmp;
        tmp = q->head;
        q->head = q->head->next;
        if (tmp->value)
            free(tmp->value);
        free(tmp);
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *newh;

    newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        free(newh);
        return false;
    }
    int len = strlen(s) + 1;
    newh->value = (char *) malloc(len * sizeof(char));
    if (!newh->value) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, len);

    newh->next = q->head;
    q->head = newh;
    if (!q->tail)
        q->tail = newh;
    ++q->size;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *newt;
    newt = malloc(sizeof(list_ele_t));
    if (!newt) {
        free(newt);
        return false;
    }
    size_t len = strlen(s) + 1;
    newt->value = (char *) malloc(len * sizeof(char));
    if (!newt->value) {
        free(newt);
        return false;
    }
    strncpy(newt->value, s, len);
    newt->next = NULL;
    if (!q->tail) {
        q->head = newt;
    } else
        q->tail->next = newt;
    q->tail = newt;
    ++q->size;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head)
        return false;
    if (q->head->value)
        strncpy(sp, q->head->value, bufsize);
    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    free(tmp->value);
    free(tmp);
    --q->size;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (q != NULL)
        return q->size;
    else
        return 0;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->head)
        return;
    list_ele_t *curr, *pre, *nxt;
    curr = q->head;
    pre = NULL;
    q->head = q->tail;
    q->tail = curr;
    while (curr) {
        nxt = curr->next;
        curr->next = pre;
        pre = curr;
        curr = nxt;
    }
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || q->size == 1)
        return;

    // bubble sort
    list_ele_t *curr, *pre, *tmp, *tail;
    tail = NULL;
    while (q->head != q->tail) {
        curr = pre = q->head;
        while (curr && curr->next && curr->next != tail) {
            if (strcmp(curr->value, curr->next->value) > 0) {
                tmp = curr->next;
                curr->next = tmp->next;
                tmp->next = curr;
                if (curr == q->head) {
                    q->head = tmp;
                    pre = tmp;
                } else {
                    pre->next = tmp;
                    pre = pre->next;
                }
            } else {
                if (curr != q->head)
                    pre = pre->next;
                curr = curr->next;
            }
        }
        tail = curr;
    }
}
