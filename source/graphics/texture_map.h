#ifndef TEXTURE_MAP_H_INCLUDED
#define TEXTURE_MAP_H_INCLUDED

#include "../mir/tile.h"


#define GET_TEXTURE(a) texture_map_tiles[a][0]

enum texture_map_type_tiles
{
    TM_GRASS, TM_WATER, TM_OCEAN,
    TM_RED_TOWER, TM_BLUE_TOWER,
    TM_RED_WARRIOR, TM_BLUE_WARRIOR,
    TM_RED_ARCHER,  TM_BLUE_ARCHER,
    TM_BLUE_SHIP, TM_RED_SHIP,
    TM_HEALTH_POINT,
//    TM_HEALTH_POINT_8,
//    TM_HEALTH_POINT_6,
//    TM_HEALTH_POINT_4,
//    TM_HEALTH_POINT_2,
    TM_MOUNTAIN, TM_MOUNTAIN_FOREST,
    TM_TREE,
    TM_SELECTED,
    TM_ACTIVE_UNIT,
    TM_ACTIVE_TILE,
    TM_ACTIVE_TILE_ATTACK,
    TM_ACTIVE_TILE_WATER,
    TM_EMPTY_TEXTURE,
    TM_NO_TEXTURE,
};

                // [INDEX][COUNT]
static const int texture_map_tiles[][2] =
{
    [TM_GRASS] = {0, 1},
    [TM_WATER] = {1, 1},
    [TM_OCEAN] = {-1, -1},
    [TM_RED_TOWER]  = {8, 1},
//    [TM_BLUE_TOWER] = {9, 1},
    [TM_BLUE_WARRIOR] = {16,     1},
    [TM_BLUE_ARCHER]  = {16 + 1, 1},
    [TM_RED_ARCHER]   = {16 + 2, 1},
    [TM_RED_WARRIOR]  = {16 + 3, 1},
    [TM_RED_SHIP]     = {24, 1},
    [TM_BLUE_SHIP]    = {25, 1},
    [TM_HEALTH_POINT] = {32, 4},
    [TM_ACTIVE_UNIT]  = {16 + 5,1},
    [TM_ACTIVE_TILE]  = {53, 1},
    [TM_ACTIVE_TILE_ATTACK] = {52, 1},
    [TM_ACTIVE_TILE_WATER] = {54, 1},
    [TM_MOUNTAIN] = {40, 2},
    [TM_MOUNTAIN_FOREST] = {41, 1},
    [TM_TREE] = {48, 2},
    [TM_SELECTED] = {56, 1},
//    [TM_ACTIVE_TILE] = {29, 1},
    [TM_EMPTY_TEXTURE] = {-1, -1},
    [TM_NO_TEXTURE] = {63, 1},

};


#endif // TEXTURE_MAP_H_INCLUDED
