#include "malloc.h"
#include <pthread.h>
#include "libr.h"

static pthread_mutex_t arena;

void show_alloc_mem()
{
}

static pthread_mutex_t lock;

void take_lock()
{
	static int initialized = 0;

	if (!initialized)
	{
		initialized = 1;
		pthread_mutex_init(&lock, NULL);
	}
//	pthread_mutex_lock(&lock);
}

void *free_lock(void *ret)
{
//	pthread_mutex_unlock(&lock);
	return (ret);
}

void        free(void *ptr)
{
	if (ptr == NULL)
		return ;
	take_lock();
	mmemalloc_free(ptr);
	free_lock(NULL);
}
void        *malloc(size_t size)
{
	take_lock();
	if (size == 0)
		size = 128;
	void *ptr = mmemalloc_alloc(size);
	return (free_lock(ptr));
}
void		*realloc(void *ptr, size_t size)
{
	ft_putfmt("Realloc(%x, %u)\n", ptr, size);
	void	*new;
	int		res;

	take_lock();
	if (ptr != NULL && size == 0)
	{
		ptr = mmemalloc_alloc(32);
		mmemalloc_free(ptr);
		return (ptr);
	}
	else if (ptr == NULL)
		return free_lock(mmemalloc_alloc(size > 0 ? size : 32));
	if ((res = mmemalloc_expande(ptr, size)) == 0)
	{
		ft_putfmt("Nothing todo\n");
		return (free_lock(ptr));
	}
	if (res < 0)
	{
		ft_putfmt("Something wrong\n");
		return (mmemalloc_alloc(size > 0 ? size : 32));//Have to check
	}
	if ((new = mmemalloc_alloc(size)) == NULL)
		return (free_lock(NULL));//TODO free ?
	ft_putfmt("Realloc ret %p\n", ptr);
	return (free_lock(ft_memcpy(new, ptr, ((t_memmagic *)ptr - 1)->size)));
}
void		*reallocf(void *ptr, size_t size)
{
	void	*new;

	if ((new = realloc(ptr, size)) == NULL)
	{
		free(ptr);
		return (NULL);
	}
	return (ptr);
}
void		*calloc(size_t nmemb, size_t size)
{
	void	*ptr;

	if ((ptr = malloc(nmemb * size)) == NULL)
		return (NULL);
	ft_memset(ptr, size, 0);
	return (ptr);
}
void        *valloc(size_t size)
{
	return (malloc(size));
}
void         *reallocarray(void *ptr, size_t nmemb, size_t size)
{
	return (realloc(ptr, nmemb * size));
}
