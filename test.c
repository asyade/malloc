#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "malloc.h"

#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main()
{
    char *ptr = malloc(1024);
    memset(ptr, 432, 1024);
    write(1, ptr, 1024);
    free(ptr);
}