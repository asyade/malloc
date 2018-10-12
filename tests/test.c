#include "libr.h"
#include "malloc.h"
#define N 31

typedef struct s_sized_ptr
{
    void *ptr;
    size_t size;
} t_sized_ptr;

int random_range(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}

void random_test(size_t max_ptr, size_t nbr_ops)
{


    t_sized_ptr *ptrs =malloc(max_ptr* 2 *sizeof(t_sized_ptr));

    size_t nbr_ptr = 0;
    for (size_t i = 0; i < nbr_ops; i++)
    {
        switch (random_range(0, 5))
        {
        // Free
        case 2:
            if (nbr_ptr > 0)
            {
                if (random() % 2 && random() % 2)
                {
                    ft_putfmt(CL_GREEN "SHIFTING PTRS\n" CL_RESET);
                    ft_shift_array(ptrs, nbr_ptr, sizeof(t_sized_ptr), random);
                }
                nbr_ptr--;
                free(ptrs[nbr_ptr].ptr);
                ft_putfmt(CL_RED "FREE(%p)\n" CL_RESET, ptrs[nbr_ptr].ptr);
            }
            break;
        case 4:
        {
            size_t to = random_range(2, 1000);
            if (random() % 2 && random() % 2)
            {
                ft_putfmt(CL_GREEN "SHIFTING PTRS\n" CL_RESET);
                ft_shift_array(ptrs, nbr_ptr, sizeof(t_sized_ptr), random);
                break;
            }
            for (size_t i = 0; i < to && nbr_ptr > 0; i++)
            {
                nbr_ptr--;
                free(ptrs[nbr_ptr].ptr);
                ft_putfmt(CL_YELLOW "FREE_RANGE(%p)\n" CL_RESET, ptrs[nbr_ptr].ptr);
            }
        }
        break;
        ///Realloc
        case 5:
        {
            size_t r = random_range(1, 20);
            while (r--)
            {
                if (nbr_ptr <= 0)
                    break;
                size_t new_size = ptrs[0].size * random_range(1, 4);
                size_t old_size = ptrs[0].size;
                if (new_size >= MAX_ALLOC_SIZE)
                    break ;
                ptrs[0].size = new_size;
                ptrs[0].ptr = reallocf(ptrs[0].ptr, new_size);
                if (ptrs[0].ptr == NULL)
                {
                    ft_putfmt("Realloc failed\n");
                    exit(1);
                }
                ft_putfmt(CL_CYAN "EXAPND(%p, %u-%u, real %u)" CL_GREEN " SUCCESS\n" CL_RESET, ptrs[0].ptr, old_size,new_size, ((t_memmagic *)ptrs[0].ptr - 1)->size);
                ft_memset(ptrs[0].ptr, 42, new_size);
            }
        }
        break;
        //Alloc
        case 1:
            if (nbr_ptr >= max_ptr)
                break;
            ptrs[nbr_ptr].size = random_range(128, 1024 * 512);
            ptrs[nbr_ptr].ptr = malloc(ptrs[nbr_ptr].size);
            if (!ptrs[nbr_ptr].ptr)
                break;

            ft_putfmt(CL_RED "ALLOC_MED(%u, real %u)=%p\n" CL_RESET, ptrs[nbr_ptr].size, ((t_memmagic *)ptrs[nbr_ptr].ptr - 1)->size, ptrs[nbr_ptr].ptr);
            nbr_ptr++;
            break;
        case 3:
        {
            size_t to = random_range(4, 20);
            size_t sz = (random_range(1, 4) + 1) * 16;
            for (size_t i = 0; i < to && nbr_ptr < max_ptr; i++)
            {
                ptrs[nbr_ptr].size = sz;
                ptrs[nbr_ptr].ptr = malloc(ptrs[nbr_ptr].size);
                if (reallocf(ptrs[nbr_ptr].ptr, sz) != ptrs[nbr_ptr].ptr) {
                    ft_putfmt("ptr change but size is the same\n");
                    exit(1);
                }
                if (!ptrs[nbr_ptr].ptr)
                    continue;
                ft_putfmt(CL_GREEN "ALLOC_RANG(%u, real=%u)=%p\n" CL_RESET, ptrs[nbr_ptr].size, ((t_memmagic *)ptrs[nbr_ptr].ptr - 1)->size, ptrs[nbr_ptr].ptr);
                nbr_ptr++;
            }
        }
        break;
        default:
        {
            size_t to = random_range(4, 20);
            for (size_t i = 0; i < to && nbr_ptr < max_ptr; i++)
            {
                ptrs[nbr_ptr].size = random_range(12*128, 128*128);
                ptrs[nbr_ptr].ptr = calloc(ptrs[nbr_ptr].size / 10, 10);
                if (!ptrs[nbr_ptr].ptr)
                    continue;
                ft_putfmt(CL_BLUE "ALLOC_RND_RANG(%u, real=%u)=%p\n" CL_RESET, ptrs[nbr_ptr].size, ((t_memmagic *)ptrs[nbr_ptr].ptr - 1)->size, ptrs[nbr_ptr].ptr);
                nbr_ptr++;
            }
        }
        }
    }
    while (nbr_ptr--)
    {
        free(ptrs[nbr_ptr].ptr);
    }
    free(ptrs);
    ft_putfmt("# DONE #\n");
}

#define THREAD_COUNT 1
pthread_t thrads[16];

void *lt(void *d)
{
    ft_putfmt("begin\n");
    random_test(1024, 100);
    return (d);
}

void test_threads(int seed)
{
    srand(seed);
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_create(&thrads[i], NULL, lt, NULL);
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(thrads[i], NULL);
    }
}

void test_alloc_limits()
{
    size_t fillsize = 4000;

    size_t page_before = mmemalloc_heap()->size;
    void *ptr =NULL;// malloc(fillsize);
    // if (ptr == NULL)
    // {
        // ft_putfmt("Can't alloc !\n");
        // exit(1);
    // }
    ptr = reallocf(ptr, fillsize);// != ptr)
    {
    //     ft_putfmt("Realloc change addr but size is the same\n");
    //     exit(1);
    }
    t_memmagic *magic = (t_memmagic *)ptr - 1;
    if (magic->size != fillsize + (2*sizeof(t_memmagic)))
    {
        ft_putfmt("Allocator use %u instaed of %u\n", magic->size, fillsize);
        exit(1);
    }
    ft_putfmt("Realloc test 1 (# means addr has changed): \n");
    ft_memset(ptr, 42, fillsize);
    for (int i = 0; i < 10; i++)
    {
        fillsize += 4096;
        void *old = ptr;
        ptr = reallocf(ptr, fillsize);
        ft_memset(ptr, 42, fillsize);
        if (ptr == old)
            ft_putchar_fd('.', 1);
        else
            ft_putchar_fd('#', 1);

        
       // magic = (t_memmagic *)ptr - 1;
    }
    ft_putfmt("\n");
    free(ptr);
    if (page_before != mmemalloc_heap()->size) {
        ft_putfmt("Big heap leak !\n");
        exit(1);
    }
}

void    test_tiny_realloc()
{
    void *ptr = malloc(0);
    if (((t_memmagic *)ptr -1)->size < 32)
    {
        ft_putfmt("Min alloc size is too small !\n");
        exit(1);
    }
    ft_memset(ptr, 42, 32);
    if (reallocf(ptr, 64) != ptr)
    {
        ft_putfmt("realloc prediction dosent work !\n");
        exit(1);
    }
    ft_memset((unsigned char *)ptr + 32, 43, 32);
    if (reallocf(ptr, 64) != ptr)
    {
        ft_putfmt("address change but size is the same\n");
        exit(1);
    }
    free(ptr);
}

void        test_show()
{
    #define NBR 4096
    void *ptr[NBR];

    size_t before = mmemalloc_heap()->size;
    
    for (size_t i = 0; i < NBR; i++)
    {
        ptr[i] = malloc(10);
    }
    show_alloc_mem();
    for (size_t i = 0; i < NBR; i++)
    {
        free(ptr[i]);
    }
    if (mmemalloc_heap()->size - before > 2)
    {
        ft_putfmt("Allocator leak ! before %u after %u\n", before, mmemalloc_heap()->size);
        exit (1);
    }
}

void       test_42()
{
    t_bheap     *heap = mmemalloc_heap();
    size_t      old = heap->size;

    if (old > 2)
    {
        ft_putfmt("Allocator leak, begin with %u pages()\n", old);
        exit(1);
    }
    ft_putfmt("begin with %u pages\n", old);
    
}

int main()
{

    //test_tiny_realloc();
    //test_alloc_limits();
    test_threads(42);

    show_alloc_mem();
    exit(1);

    test_show();
//    test_42();
}