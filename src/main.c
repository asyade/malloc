#include "bhp.h"
#include <stdio.h>
#include<stdlib.h>
#include<time.h>

int main(int ac, char **av)
{
    t_bh        heap;

    if (bh_init(&heap) == -1)
    {
        printf("Can't init heap\n");
        return 1;
    }

    t_bhnode tmp;

    tmp.content = NULL;
    tmp.value = 1;
    srand(time(NULL));

    int arrlen = 7;
    int arr[7] = {
        1,
        21,
        32,
        8,
        3,
        50,
        2,
    };
    for (int i = 0; i < arrlen; i++) {
        tmp.value = arr[i];
        bh_insert(&heap, &tmp);
    }

    bh_remove(&heap, 1);

    for (int i = 0; i < heap.heap_size; i++) {
        printf("%d, ", heap.buffer[i].value);
    }
    printf("\n");
int x = 0;
    for (int i = 0; i < arrlen; i++) {
    if (bh_find(&heap, arr[i]) != NULL) {
        printf("OULOULOU %d\n", arr[i]);
        x++;
    }
    }

    if (x != arrlen-1) {
        printf("KO\n");

    }
    else printf("OK\n");

}

