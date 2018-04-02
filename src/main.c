#include "memheap.h"
#include "bhp.h"
#include "area.h"
#include <stdio.h>
#include<stdlib.h>
#include<time.h>

/*int main() {
    t_memheap heap;

    mh_init(&heap, 128*4094);
    for (int i = 0; i < 2000 ; i ++) {
    void    *addr[10];
    for (int i = 0; i < 3; i++)
    {
        printf("%p\n", (addr[i] =  mh_reserv_chunk(&heap, 1024)));
    }
    for (int i = 0; i < 3; i++)
    {
       if ( mh_free_chunk(&heap, addr[i]) != 0)
        printf("can't freed");
    }    
    
}
}*/
int main() {

    t_area  area;

    ar_init(&area);
    void    *addr[12500];
    for (int i = 0; i < 1200; i++)
    {
        ar_get_chunk(&area, 16);
        ar_get_chunk(&area, 500);
        ar_get_chunk(&area, 10);
        if ((addr[i] = ar_get_chunk(&area, 4096)) == NULL)
            printf("Can't alloc\n");
        printf("%p\n", addr[i]);
    }
    

/*    
    for (int i = 0; i < 10 ; i ++) {


    void    *addr[12500];
    for (int i = 0; i < 12500; i++)
    {
        if ((addr[i] = sar_get_chunk(&area, 4096)) == NULL)
            printf("Can't alloc\n");
    }

    for (int i = 0; i < 12500; i++)
    {
        if (sar_free_chunk(&area, addr[i]) != 0)
            printf("Can't free %zu\n", addr[i]);
    }
   
    }
    */
}



/*
int main(int ac, char **av)
{
    t_bh        heap;

    if (mh_init(&heap) == -1)
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
        mh_insert(&heap, &tmp);
    }

    mh_remove(&heap, 1);

    for (int i = 0; i < heap.heap_size; i++) {
        printf("%d, ", heap.buffer[i].value);
    }
    printf("\n");
int x = 0;
    for (int i = 0; i < arrlen; i++) {
    if (mh_find(&heap, arr[i]) != NULL) {
        printf("OULOULOU %d\n", arr[i]);
        x++;
    }
    }

    if (x != arrlen-1) {
        printf("KO\n");

    }
    else printf("OK\n");

}

*/