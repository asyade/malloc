#ifndef BHP_H
# define BHP_H

# define _GNU_SOURCE
# include <sys/mman.h>
# include <limits.h>
# include <string.h>

# define BH_CHKSZ     4096
# define BH_MINVAL    INT_MIN
# define BH_MAXVAL    INT_MAX
# define BH_DEFVAL    BH_MINVAL
# define BH_LCHILD(i) (2 * i + 1)
# define BH_RCHILD(i) (2 * i + 2)
# define BH_PARENT(i) ((i - 1) >> 1)

typedef struct  s_value
{             
    size_t      min;
    size_t      max;
}
                t_value;

typedef struct      s_bhnode {
    void            *content;
    t_value         value;
}                   t_bhnode;

typedef struct      s_bh {
    t_bhnode        *buffer;
    size_t          buffer_size;
    size_t          heap_size;
}                   t_bh;

int                 bh_cmp(t_bhnode *a, t_bhnode *b);

//
// Return 0 on success
// -1 on memory allocation error 
//
int                 bh_init(t_bh *heap);

int                 bh_clear(t_bh *bh);

//0 if equals 1 if a is lower that b
int                bh_cmp(t_bhnode *a, t_bhnode *b);


//
// Return 0 on success
// -1 on memory allocation error
//
size_t               bh_insert(t_bh *heap, t_bhnode *node);

int                 bh_remove(t_bh *heap, size_t node);

void                bh_filter_down(t_bh *heap, t_bhnode node, size_t cursor);

size_t              bh_filter_up(t_bh *heap, t_bhnode node, size_t cursor);

void                bh_filter(t_bh *heap, t_bhnode node, size_t cursor);


//
// Warning this functions can't grows more thant BH_CHKSZ
// return -1 on failure, 1 if the map has grows and 0 if nothings appening
//
int                 bh_ensure_buffer(t_bh *heap, size_t needed);

t_bhnode            *bh_find(t_bh *heap, size_t value);

#endif