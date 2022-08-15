#include <stdlib.h>
#include <stdio.h>

#include "point.h"

colorf_rgb_t random_cfrgb()
{
    colorf_rgb_t r = {((float)rand()) / ((float)RAND_MAX), ((float)rand()) / ((float)RAND_MAX), ((float)rand()) / ((float)RAND_MAX)};
    return;
}
