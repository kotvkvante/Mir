#include <stdlib.h>
#include <stdio.h>

#include "../kernel/error_handler.h"

#include "mir.h"
#include "unit_search_path.h"

#define path_lenght 2

#define _trav(x, y) trav.g[(2 * path_lenght + 1) * (y + 2) + (x + 2)]

#define USP_PROCESS_TILE_XYR(x, y, r) \
    if(_f(start_x + x, start_y + y)) _trav(x, y).range = r;

#define USP_PROCESS_TILE(x, y) USP_PROCESS_TILE_XYR(x, y, 2)
#define USP_PROCESS_TILE_XY1(x, y) USP_PROCESS_TILE_XYR(x, y, 1)


#define _AFTER_PROCESS_TILE_3(x, y) \
    USP_PROCESS_TILE(_USP_arr3[x + 1][y + 1][0], _USP_arr3[x + 1][y + 1][1])   \
    USP_PROCESS_TILE(_USP_arr3[x + 1][y + 1][2], _USP_arr3[x + 1][y + 1][3])   \
    USP_PROCESS_TILE(_USP_arr3[x + 1][y + 1][4], _USP_arr3[x + 1][y + 1][5])

#define _AFTER_PROCESS_TILE_5(x, y) \
    USP_PROCESS_TILE(_USP_arr5[x + 1][y + 1][0], _USP_arr5[x + 1][y + 1][1])   \
    USP_PROCESS_TILE(_USP_arr5[x + 1][y + 1][4], _USP_arr5[x + 1][y + 1][5])   \
    USP_PROCESS_TILE(_USP_arr5[x + 1][y + 1][2], _USP_arr5[x + 1][y + 1][3])   \
    USP_PROCESS_TILE(_USP_arr5[x + 1][y + 1][6], _USP_arr5[x + 1][y + 1][7])   \
    USP_PROCESS_TILE(_USP_arr5[x + 1][y + 1][8], _USP_arr5[x + 1][y + 1][9])


#define USP_PROCESS_TILE_3(x, y)             \
    if(_f(x + start_x, y + start_y))         \
    {                                       \
        _trav(x, y).range = 1;              \
        _AFTER_PROCESS_TILE_3(x, y)         \
    }

#define USP_PROCESS_TILE_5(x, y)             \
    if(_f(x + start_x, y + start_y))         \
    {                                       \
        _trav(x, y).range = 1;              \
        _AFTER_PROCESS_TILE_5(x, y)         \
    }


static int _USP_arr5[3][3][5 * 2] =
{
    [2][2] = {
        2, 0,
        2, 1,
        2, 2,
        1, 2,
        0, 2,
    },
    [0][2] = {
        0, 2,
       -1, 2,
       -2, 2,
       -2, 1,
       -2, 0,
   },
    [0][0] = {
        -2, 0,
        -2,-1,
        -2,-2,
        -1,-2,
         0,-2,
    },
    [2][0] = {
        0,-2,
        1,-2,
        2,-2,
        2,-1,
        2, 0,
    },

};

static int _USP_arr3[3][3][3 * 2] =
{
    [2][1] = {
         2,-1,
         2, 0,
         2, 1,
    },
    [1][2] = {
        1, 2,
        0, 2,
       -1, 2,
    },
    [0][1] = {
        -2, 1,
        -2, 0,
        -2,-1,
    },
    [1][0] = {
        -1,-2,
         0,-2,
         1,-2,
    }
};

static trav_t trav;

void unit_search_path_init()
{
    log_msg(DEFAULT_C, "Unit search path init.");

    trav.g = malloc(sizeof(trav_tile_t) * (2 * path_lenght + 1) * (2 * path_lenght + 1) );
    memset(trav.g, 0, sizeof(trav_tile_t) * (2 * path_lenght + 1) * (2 * path_lenght + 1));

//    _trav(1, -1).range = 1;

    trav_print();
}

void unit_search_path_clear()
{
    memset(trav.g, 0, sizeof(trav_tile_t) * (2 * path_lenght + 1) * (2 * path_lenght + 1));
}

void unit_search_path(unit_t* unit)
{
    int start_x = unit->x, start_y = unit->y;

#define _f unit_warrior_can_visit_xy

    USP_PROCESS_TILE_5( 1, 1);
    USP_PROCESS_TILE_3( 0, 1);
    USP_PROCESS_TILE_5(-1, 1);
    USP_PROCESS_TILE_3(-1, 0);

    USP_PROCESS_TILE_5(-1,-1);
    USP_PROCESS_TILE_3( 0,-1);
    USP_PROCESS_TILE_5( 1,-1);
    USP_PROCESS_TILE_3( 1, 0);

#undef _f
}

void unit_search_path_8(unit_t* unit)
{
    int start_x = unit->x, start_y = unit->y;

#define _f unit_warrior_can_visit_xy

    USP_PROCESS_TILE_XY1( 1, 1);
    USP_PROCESS_TILE_XY1( 0, 1);
    USP_PROCESS_TILE_XY1(-1, 1);
    USP_PROCESS_TILE_XY1(-1, 0);

    USP_PROCESS_TILE_XY1(-1,-1);
    USP_PROCESS_TILE_XY1( 0,-1);
    USP_PROCESS_TILE_XY1( 1,-1);
    USP_PROCESS_TILE_XY1( 1, 0);
#undef _f

}

bool trav_is_on_map_xy(int x, int y)
{
    if( (x > path_lenght) || (x < - path_lenght) ) return false;
    if( (y > path_lenght) || (y < - path_lenght) ) return false;

    return true;
}

int trav_map_xy(int x, int y)
{
    return _trav(x, y).range;
}

int trav_map_xy_safe(int x, int y)
{
    if( !trav_is_on_map_xy(x, y) ) { log_msg_va(DEFAULT_C, "%s: point isn't on trav map: %d %d.", __func__, x, y); return 0; }
    return _trav(x, y).range;
}

void trav_print()
{
    for (size_t i = 0; i < 2 * path_lenght + 1; i++)
    {
        for (size_t j = 0; j < 2 * path_lenght + 1; j++)
        {
            printf("%d ", trav.g[(2 * path_lenght - i) * (2 * path_lenght + 1) + j].range);
        }
        printf("\n");
    }
}
