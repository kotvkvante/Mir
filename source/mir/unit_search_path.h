#ifndef UNIT_SEARCH_PATH_H_INCLUDED
#define UNIT_SEARCH_PATH_H_INCLUDED

#include "unit.h"

typedef struct trav_tile_t
{
    int range;
} trav_tile_t;

typedef struct trav_t
{
    trav_tile_t* g;   // graph
} trav_t;


void unit_search_path_init();
void unit_search_path(unit_t* unit);
void unit_search_path_8(unit_t* unit);
int trav_map_xy_safe(int x, int y);
int trav_map_xy(int x, int y);

void trav_print();

void unit_search_path_clear();

#endif // UNIT_SEARCH_PATH_H_INCLUDED
