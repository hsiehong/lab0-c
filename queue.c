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

    if (q->size == 0) {
        q->tail = newh;
        newh->next = NULL;
    } else {
        newh->next = q->head;
    }
    q->head = newh;
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
    if (q->size == 0) {
        q->head = newt;
    } else {
        q->tail->next = newt;
    }
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
    if (q->head->value && sp) {
        strncpy(sp, q->head->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    } else
        return false;
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
/*
    return new head of two linked list
*/
list_ele_t *merge_2_list(list_ele_t *l1, list_ele_t *l2)
{
    list_ele_t *n_head = NULL;
    list_ele_t **tmp = &n_head;
    while (l1 && l2) {
        if (strcmp(l1->value, l2->value) > 0) {
            *tmp = l2;
            l2 = l2->next;
        } else {
            *tmp = l1;
            l1 = l1->next;
        }
        tmp = &((*tmp)->next);
    }
    if (l1)
        *tmp = l1;
    else
        *tmp = l2;
    return n_head;
}

list_ele_t *merge_sort(list_ele_t *head)
{
    if (!head || !head->next)
        return head;
    list_ele_t *fast, *slow;
    fast = head->next;
    slow = head;
    // split list into 2 half list
    while (fast && fast->next) {
        fast = fast->next->next;
        slow = slow->next;
    }
    fast = slow->next;
    slow->next = NULL;
    slow = head;

    list_ele_t *left = merge_sort(slow);
    list_ele_t *right = merge_sort(fast);

    return merge_2_list(left, right);
}

void q_sort(queue_t *q)
{
    if (!q || q->size <= 1)
        return;
    // sorting by merge sort
    q->head = merge_sort(q->head);

    q->tail = q->head;
    while (q->tail && q->tail->next)
        q->tail = q->tail->next;
}