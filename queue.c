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
    if (!head || list_empty(head))
        return false;
    struct list_head *pos, *n;
    bool flag = false;
    list_for_each_safe (pos, n, head) {
        element_t *pos_element = list_entry(pos, element_t, list);
        char *n_val = list_entry(n, element_t, list)->value;
        if (pos->next != head && strcmp(pos_element->value, n_val) == 0) {
            flag = true;
            list_del(&pos_element->list);
            q_release_element(pos_element);
        } else if (flag) {
            flag = false;
            list_del(&pos_element->list);
            q_release_element(pos_element);
        }
    }
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
    struct list_head *pos, *n;
    list_for_each_safe (pos, n, head) {
        list_move(pos, head);
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head || list_empty(head))
        return;
    int count = 0;
    struct list_head *pos, *n;
    list_for_each_safe (pos, n, head) {
        count += 1;
        if (count <= k) {
            list_move(pos, head);
        } else
            break;
        pos->prev = head;
    }
}

int q_merge_two(struct list_head *left, struct list_head *right)
{
    if (!left || !right)
        return 0;
    int size = q_size(left) + q_size(right);
    struct list_head new_list;
    INIT_LIST_HEAD(&new_list);
    while (!list_empty(left) && !list_empty(right)) {
        element_t *left_element = list_first_entry(left, element_t, list);
        element_t *right_element = list_first_entry(right, element_t, list);
        element_t *min = strcmp(left_element->value, right_element->value) < 0
                             ? left_element
                             : right_element;
        list_move_tail(&min->list, &new_list);
    }
    list_splice_tail_init(left, &new_list);
    list_splice_tail_init(right, &new_list);
    list_splice(&new_list, left);
    return size;
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;
    struct list_head *slow = head, *fast = head->next;
    for (; fast != head && fast->next != head; fast = fast->next->next)
        slow = slow->next;
    struct list_head left;
    list_cut_position(&left, head, slow);
    q_sort(head, descend);
    q_sort(&left, descend);
    q_merge_two(head, &left);
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;
    int len = 1;
    struct list_head *cursor = head->prev;
    if (cursor == head)
        return len;
    char *record = list_entry(cursor, element_t, list)->value;
    for (cursor = head->prev; cursor->prev != head;) {
        len++;
        element_t *cur_element = list_entry(cursor, element_t, list);
        struct list_head *prev_cursor = cursor->prev;
        if (strcmp(cur_element->value, record) > 0) {
            list_del(&cur_element->list);
            q_release_element(cur_element);
            len--;
        } else
            record = cur_element->value;
        cursor = prev_cursor;
    }
    element_t *cur_element = list_entry(cursor, element_t, list);
    if (strcmp(cur_element->value, record) > 0) {
        list_del(&cur_element->list);
        q_release_element(cur_element);
        len--;
    }
    return len;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;
    int len = 1;
    struct list_head *cursor = head->prev;
    if (cursor == head)
        return len;
    char *record = list_entry(cursor, element_t, list)->value;
    for (cursor = head->prev; cursor->prev != head;) {
        len++;
        element_t *cur_element = list_entry(cursor, element_t, list);
        struct list_head *prev_cursor = cursor->prev;
        if (strcmp(cur_element->value, record) < 0) {
            list_del(&cur_element->list);
            q_release_element(cur_element);
            len--;
        } else
            record = cur_element->value;
        cursor = prev_cursor;
    }
    element_t *cur_element = list_entry(cursor, element_t, list);
    if (strcmp(cur_element->value, record) < 0) {
        list_del(&cur_element->list);
        q_release_element(cur_element);
        len--;
    }
    return len;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    if (!head || list_empty(head))
        return 0;
    if (list_is_singular(head))
        return q_size(list_first_entry(head, queue_contex_t, chain)->q);
    int queue_size = 0;
    queue_contex_t *first, *second;
    first = list_first_entry(head, queue_contex_t, chain),
    second = list_entry(first->chain.next, queue_contex_t, chain);
    while (second->q) {
        queue_size = q_merge_two(first->q, second->q);
        second->q = NULL;
        list_move_tail(&second->chain, head);
        second = list_entry(first->chain.next, queue_contex_t, chain);
    }
    return queue_size;
}
