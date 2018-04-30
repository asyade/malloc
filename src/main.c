#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include<unistd.h>
#include "malloc.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"


int rand_rang(int max, int min)
{
    return (rand() % (max + 1 - min) + min);
}

# define PTR_COUNT 10

void        randomize_array(void **array, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        int     to =rand() % len;
        void    *tmp;

        tmp = array[to];
        array[to] = array[i];
        array[i] = tmp;
    }
}


void        dump_chunk(t_mem_chunk *chunk)
{
    while (chunk)
    {
        char    *lcol;
        if (chunk->prev && chunk->prev->real_size + (size_t)chunk->prev > (size_t)chunk)
            lcol = KRED;
        else if (chunk->real_size < chunk->user_size)
            lcol = KRED;
        else if (!chunk->prev || !chunk->next)
            lcol = KCYN;
        else if (chunk->status == FREE)
            lcol = KMAG;
        else 
            lcol = chunk->real_size > chunk->user_size ? KGRN : KYEL;
        if (chunk->real_size < chunk->user_size)
            lcol = KCYN;
        printf("[%s%lx\x1B[0m]" ,(char *)lcol, (unsigned long int)chunk);
        chunk = chunk->next;
    }
}

void        dump_arena(t_mem_arena *arena)
{
    t_mem_arena *tmp;
    tmp = arena;
    while (1)
    {
        dump_chunk(arena->buffer);
        printf("\n<->\n");
        arena = arena->next;
        if (arena == tmp)
            return ;
    }
}

void print(char *s) 
{ 
write(1, s, strlen(s)); 
} 

int main() 
{ 
char *addr; 

    addr = malloc(16); 
    free(NULL); 
    free((void *)addr + 5); 
    if (realloc((void *)addr + 5, 10) == NULL) 
    print("Bonjours\n"); 
} 

int m2ain(int ac, char **av)
{
    if (ac > 1)
        srand(atoi(av[1]));
    else
        srand(54684);


    void        *ptr[PTR_COUNT];


    for (int i = 0; i < PTR_COUNT; i++)
    {
        ptr[i] = malloc(rand_rang(10, 200));
        memset(ptr[i], 42, DPTR_TO_CHK(ptr[i])->user_size);
        dump_arena(DPTR_TO_CHK(ptr[0])->arena);
    }
    dump_arena(DPTR_TO_CHK(ptr[0])->arena);
    //randomize_array(ptr, PTR_COUNT);
    /*for (int i = 0; i < PTR_COUNT / 2; i++)
        free(ptr[i]);
    for (int i =0; i< PTR_COUNT / 2; i++)
    {
        ptr[i] = malloc(16);
        memset(ptr[i], 42, 16);
    }*/
    for (int i = 6; i < PTR_COUNT; i++)
    {
        ptr[i] = realloc(ptr[i], DPTR_TO_CHK(ptr[i])->user_size * 2);
        dump_arena(DPTR_TO_CHK(ptr[0])->arena);
        memset(ptr[i], 42, DPTR_TO_CHK(ptr[i])->user_size);
    }
    /*randomize_array(ptr, PTR_COUNT);

    for(int i = 0; i < PTR_COUNT; i++)
    {
        free(ptr[i]);
        dump_arena(DPTR_TO_CHK(ptr[0])->arena);
        
    }*/
    return (0);   
}