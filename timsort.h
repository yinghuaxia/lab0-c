struct list_head;

typedef int (*list_cmp_func_t)(void *,
                               const struct list_head *,
                               const struct list_head *);

void timsort(void *priv, struct list_head *head, list_cmp_func_t cmp);

__attribute__((nonnull(1, 2))) int cmp(void *priv,
                                       const struct list_head *a,
                                       const struct list_head *b);