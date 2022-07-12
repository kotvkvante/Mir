#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

int rand_index(int a, int b)
{
    return a + rand() % (b - a + 1);
}

int rand_index_zero(int b)
{
    return rand_index(0, b);

}
