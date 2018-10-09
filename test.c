#include "libr.h"
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
        switch (random_range(0, 9))
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
            if (nbr_ptr <= 0)
                break;
            int fail_reason;
            size_t new_size = ptrs[0].size * random_range(0, 4);
            size_t old_size = ptrs[0].size;
            if (reallocf(ptrs[0].ptr, new_size) != NULL)
                ft_putfmt(CL_CYAN "EXAPND(%p, %u-%u, real %u)" CL_GREEN " SUCCESS\n" CL_RESET, ptrs[0].ptr, old_size, ptrs[0].size, new_size);
            else {
                ft_putfmt(CL_CYAN "EXAPND(%p, %u-%u)" CL_RED " ERROR %d\n" CL_RESET, ptrs[0].ptr, old_size, new_size, fail_reason);

                return;
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
                ptrs[nbr_ptr].size = random_range(12, 128);
                ptrs[nbr_ptr].ptr = calloc(ptrs[nbr_ptr].size / 10, 10);
                if (!ptrs[nbr_ptr].ptr)
                    continue;
                ft_putfmt(CL_BLUE "ALLOC_RND_RANG(%u, real=%u)=%p\n" CL_RESET, ptrs[nbr_ptr].size, ((t_memmagic *)ptrs[nbr_ptr].ptr - 1)->size, ptrs[nbr_ptr].ptr);
                nbr_ptr++;
            }
        }
        }
    }
    ft_putfmt("# DONE #\n");
}

pthread_t thrads[2];

void *lt(void *d)
{
    ft_putfmt("begin\n");
    random_test(1024, 1000);
    return (NULL);
}

int main(int ac, char **av)
{
    srand(ac > 1 ? atoi(av[1]) : 1);
    //  t_memalloc *allocator = memalloc_new(1024 * 1024, 4096, (t_szrange){1024, 1024});
    //  safe_memalloc_alloc(allocator, 10, 1);
    //   safe_memalloc_alloc(allocator, 10, 1);
    pthread_create(&thrads[0], NULL, lt, NULL);
    pthread_create(&thrads[1], NULL, lt, NULL);
    pthread_join(thrads[0], NULL);
    pthread_join(thrads[1], NULL);
}