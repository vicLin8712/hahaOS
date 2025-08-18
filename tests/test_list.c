#include <assert.h>
#include <stdio.h>
#include "lib/list.h"

int main(void) {
    list_t *lst = list_init();

    int a = 1, b = 2;
    list_push(lst, &a);
    assert(lst->length == 1);
    list_push(lst, &b);
    assert(lst->length == 2);

    assert(*(int *)list_pop(lst) == 1);
    assert(*(int*)list_pop(lst)  == 2);
    assert(lst->length == 0);
    assert(list_pop(lst) == NULL);

    list_destroy(lst);
    printf("All list tests passed!\n");
    return 0;
}
