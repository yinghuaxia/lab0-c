#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *new_queue = malloc(sizeof(struct list_head));
    if (!new_queue)
        return NULL;
    INIT_LIST_HEAD(new_queue);
    return new_queue;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;
    element_t *pos, *n;
    list_for_each_entry_safe (pos, n, head, list) {
        q_release_element(pos);
        // free(pos->value);
        // free(pos);
    }
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *new = malloc(sizeof(element_t));
    if (!new)
        return false;
    int size_s = (strlen(s) + 1) * sizeof(char);
    new->value = malloc(size_s);
    if (!new->value) {
        free(new);
        return false;
    }
    memcpy(new->value, s, size_s - 1);
    new->value[size_s - 1] = '\0';
    list_add(&new->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *new = malloc(sizeof(element_t));
    if (!new)
        return false;
    int size_s = (strlen(s) + 1) * sizeof(char);
    new->value = malloc(size_s);
    if (!new->value) {
        free(new);
        return false;
    }
    memcpy(new->value, s, size_s - 1);
    new->value[size_s - 1] = '\0';
    list_add_tail(&new->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *add = list_first_entry(head, element_t, list);
    if (sp && bufsize) {
        strncpy(sp, add->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
        list_del(&add->list);
        return add;
    }
    return NULL;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *add = list_last_entry(head, element_t, list);
    if (sp && bufsize) {
        strncpy(sp, add->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
        list_del(&add->list);
        return add;
    }
    return NULL;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head) {
        return -1;
    }
    int count = 0;
    struct list_head *curser;
    list_for_each (curser, head) {
        count += 1;
    }
    return count;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;
    struct list_head *front = head->next, *back = head->prev;
    while (front != back && front != back->prev) {
        front = front->next;
        back = back->prev;
    }
    list_del_init(front);
    element_t *entry = list_entry(front, element_t, list);
    q_release_element(entry);

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head)
        return;
    struct list_head *pos, *n;
    bool odd = false;
    list_for_each_safe (pos, n, head) {
        if (odd) {
            list_move(pos, pos->prev->prev);
        }
        odd = !odd;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;
    struct list_head *c, *n;
    list_for_each_safe (c, n, head) {
        list_del(c);
        list_add(c, head);
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
