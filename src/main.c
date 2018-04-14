#include "malloc.h"
#include <stdio.h>
#include<stdlib.h>
#include "bhp.h"
#include<time.h>



#define LEN 50000

int main() {
    
    for (int i = 0; i < 10; i++)
    {
        char *buffer = malloc(LEN);
        if (buffer == NULL){write(1, "ERROR\n", 6);}
        memset(buffer, 42, LEN);
        buffer[LEN - 1] = '\0';
        write(1, buffer, strlen(buffer));
        free(buffer);
    }
}
/*
int main(
{
    char *str = malloc_(20);
    strcpy(str, "123456789");
    //printf("%s\n", str);
    str = realloc_(str, 20);
    //printf("%s\n", str);
    strcpy(str, "1234567890123456789");
    //printf("%s\n", str);
}
*/
/*
int main() {
    t_subarea area;

    sar_init(&area, 4096);
    void *ptr = sar_get_chunk(&area, 4096);
    //printf("%p\n", ptr);
    sar_free_chunk(&area, ptr);
}
*/

/*
int main() {
        void   *arr[LEN];
        for (int i = 0; i < LEN; i++) {
        for (int i = 0; i < LEN; i++)
        {
            //printf("%p\n", (arr[i] = malloc_(i)));
        }
        for (int i = 0; i < LEN; i++)
        {
            free_(arr[i]);
        }
        }
}
*/

/*int main()
{
    t_bh heap;
    t_bhnode node;

    bh_init(&heap);

    node.content = 0x010;
    node.value.min = 10;
    node.value.max = 100;
    bh_insert(&heap, &node);
    node.content = 0x020;
    node.value.min = 100;
    node.value.max = 1000;
    bh_insert(&heap, &node);
    node.content = 0x40;
    node.value.min = 200;
    node.value.max = 2000;
    bh_insert(&heap, &node);
    node.content = 0x5;
    node.value.min = 55;
    node.value.max = 55;
    bh_insert(&heap, &node);
    node.content = 0x5;
    node.value.min = 35;
    node.value.max = 35;
    bh_insert(&heap, &node);
    node.content = 0x5;
    node.value.min = 49;
    node.value.max = 49;
    bh_insert(&heap, &node);
    node.content = 0x5;
    node.value.min = 29;
    node.value.max = 29;
    bh_insert(&heap, &node);
    bh_dump(&heap);
    node.content = 0x5;
    node.value.min = 19;
    node.value.max = 19;
    bh_insert(&heap, &node);
    node.content = 0x5;
    node.value.min = 55;
    node.value.max = 55;
    bh_insert(&heap, &node);
    node.content = 0x5;
    node.value.min = 35;
    node.value.max = 35;
    bh_insert(&heap, &node);
    node.content = 0x5;
    node.value.min = 49;
    node.value.max = 49;
    bh_insert(&heap, &node);
    node.content = 0x5;
    node.value.min = 29;
    node.value.max = 29;
    bh_insert(&heap, &node);
    bh_dump(&heap);
    node.content = 0x5;
    node.value.min = 19;
    node.value.max = 19;
    bh_insert(&heap, &node);
    node.content = 0x5;
    node.value.min = 55;
    node.value.max = 55;
    bh_insert(&heap, &node);
    node.content = 0x5;
    node.value.min = 35;
    node.value.max = 35;
    bh_insert(&heap, &node);
    bh_dump(&heap);
}*/

/*int main() {
    t_memheap heap;

    mh_init(&heap, 128*4094);
    for (int i = 0; i < 2000 ; i ++) {
    void    *addr[10];
    for (int i = 0; i < 3; i++)
    {
        //printf("%p\n", (addr[i] =  mh_reserv_chunk(&heap, 1024)));
    }
    for (int i = 0; i < 3; i++)
    {
       if ( mh_free_chunk(&heap, addr[i]) != 0)
        //printf("can't freed");
    }    
    
}
}*/
/*int main() {

    void    *addr[12500];
    for (int i = 0; i < 2; i++)
    {
       addr[i] = malloc_(10);
       //printf("%p\n", addr[i]);
    }
    show_alloc_mem();
    for (int i = 0; i < 2; i++)
    {
        free_(addr[i]);
    }
}*/

/*    
    for (int i = 0; i < 10 ; i ++) {


    void    *addr[12500];
    for (int i = 0; i < 12500; i++)
    {
        if ((addr[i] = sar_get_chunk(&area, 4096)) == NULL)
            //printf("Can't alloc\n");
    }

    for (int i = 0; i < 12500; i++)
    {
        if (sar_free_chunk(&area, addr[i]) != 0)
            //printf("Can't free %zu\n", addr[i]);
    }
   
    }
    */


/*
int main(int ac, char **av)
{
    t_bh        heap;

    if (mh_init(&heap) == -1)
    {
        //printf("Can't init heap\n");
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
        //printf("%d, ", heap.buffer[i].value);
    }
    //printf("\n");
int x = 0;
    for (int i = 0; i < arrlen; i++) {
    if (mh_find(&heap, arr[i]) != NULL) {
        //printf("OULOULOU %d\n", arr[i]);
        x++;
    }
    }

    if (x != arrlen-1) {
        //printf("KO\n");

    }
    else //printf("OK\n");

}

*/