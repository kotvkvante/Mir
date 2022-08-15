#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../math/point.h"
#include "../utils/utils.h"
#include "../graphics/texture_map.h"
#include "../kernel/error_handler.h"

#include "mir.h"

#include "unit.h"
#include "tile.h"
#include "team.h"



unit_t* _units_red[10]; int red_id = 0;
unit_t* _units_blue[10]; int blue_id = 0;

unit_t* units_by_team[2] = {[TEAM_RED] = _units_red, [TEAM_BLUE] = _units_blue};


typedef enum {
    FORTIFIED = 1 << 0,
    MOVEABLE  = 1 << 1,
    STARTUP = (~FORTIFIED) || MOVEABLE,

} UNIT_STATE_FLAGS;

//enum unit_types      { NO_UNIT, WARRIOR = 1, ARCHER, SHIP, UNIT_COUNT };


static int _unit_textures[UNIT_COUNT][2] =
{
    [NO_UNIT] = {GET_TEXTURE(TM_SELECTED),    GET_TEXTURE(TM_SELECTED)},
    [WARRIOR] = {GET_TEXTURE(TM_RED_WARRIOR), GET_TEXTURE(TM_BLUE_WARRIOR)},
    [ARCHER]  = {GET_TEXTURE(TM_RED_ARCHER),  GET_TEXTURE(TM_BLUE_ARCHER)},
    [SHIP]    = {GET_TEXTURE(TM_RED_SHIP),    GET_TEXTURE(TM_BLUE_SHIP)},
};


int units_count = 0;
unit_t* units[10];

void unit_init(unit_t* unit, int type, int team, int x, int y);
void unit_print_info(unit_t* unit);

//static void _warrior_attack_to(unit_t* unit, int x, int y);
//static bool _warrior_can_attack_to(unit_t* unit, int x, int y);
//static void _warrior_move_to(unit_t* unit, int x, int y);
//static bool _warrior_can_move_to(unit_t* unit, int x, int y);
//static void _warrior_draw(unit_t* unit);

static int _is_units_init = false;
unit_t test_unit;
unit_t test_unit2;
unit_t test_unit3;
unit_t test_unit4;


void units_init()
{
//    if(_is_units_init) return;
    unit_init(&test_unit,  WARRIOR, TEAM_RED, 4, 4);
    unit_init(&test_unit2, ARCHER, TEAM_RED, 5, 5);
    unit_init(&test_unit3, WARRIOR, TEAM_RED, 3, 4);
    unit_init(&test_unit4, WARRIOR, TEAM_BLUE, 2, 4);

    unit_print_info(&test_unit);
//    _is_units_init = true;
}

void unit_init(unit_t* unit, int type, int team, int x, int y)
{
    unit->x = x;
    unit->y = y;
    unit->type = type;
    unit->team = team;

    unit->texture = _unit_textures[type][team];

//    unit->attack = attack;
//    unit->move = move;
//    unit->draw = draw;

    unit->tile = mir_map_get_tile(x, y);
    unit->tile->unit = unit;
    unit->tile->entities[UNIT] = type;

    units[units_count] = unit;
    units_count++;

    if(team == TEAM_BLUE)
    {
        _units_blue[blue_id] = unit;
        blue_id++;
    }

    if(team == TEAM_RED)
    {
        _units_red[red_id] = unit;
        red_id++;
    }


}

int unit_get_texture(unit_t* unit)
{
    return unit->texture;
}


//void units_draw()
//{
//    for(int i = 0; i < units_count; i++)
//        units[i]->draw(units[i]);
//}

void unit_print_info(unit_t* unit)
{
//    printf("Unit: (%d %d) %d [Base tile type: %d]\n", unit->x, unit->y, unit->type, unit->tile->entities[FIELD]);
}

bool unit_can_attack(unit_t* unit, tile_t* tile_dest)
{
    return false;
}

bool unit_cat_move(unit_t* unit, tile_t* tile_dest)
{

}

int unit_can_move(unit_t* unit, tile_t* tile_dest)
{
    switch(unit->type)
    {
    case WARRIOR:
        if(mir_map_get_distance(unit->x, unit->y, tile_dest->x, tile_dest->y) > 2) { return false; }
        if(tile_dest->entities[UNIT] != 0) if(tile_dest->unit->team == unit->team) { return false; }
        if(tile_dest->entities[FIELD] == SEA)
            if(!mir_map_get_team()->is_navigation_reseached) { return false; }
        if(tile_dest->entities[LANDSCAPE] == MOUNTAIN || tile_dest->entities[LANDSCAPE] == MOUNTAIN_FOREST)
            if(!mir_map_get_team()->is_rock_climbing_reseached) { return false; }

        return true;

        break;

    case ARCHER:
        if(mir_map_get_distance(unit->x, unit->y, tile_dest->x, tile_dest->y) > 8) return false;
        if(tile_dest->entities[UNIT] != 0) if(tile_dest->unit->team == unit->team) return false;
        if( (tile_dest->entities[FIELD] )== SEA) return false;

        return true;

        break;


    }

    return true;
}


void unit_move(unit_t* unit, tile_t* tile_dest)
{
    if(unit == NULL) { error_msg(DEFAULT_C, "Unit NULL."); return; }
    if(tile_dest == NULL) { error_msg(DEFAULT_C, "Tile NULL."); return; }

    unit->x = tile_dest->x;
    unit->y = tile_dest->y;

    unit->tile->entities[UNIT] = NO_UNIT;
    unit->tile->unit = NULL;

    tile_dest->entities[UNIT] = unit->type;
    tile_dest->unit = unit;
    unit->tile = tile_dest;

    if(tile_dest->entities[FIELD] == SEA) unit->texture = _unit_textures[SHIP][unit->team];
    else unit->texture = _unit_textures[unit->type][unit->team];
}

void unit_move_xy(int sx, int sy, int ex, int ey)
{
    print_2i(sx, sy);
    tile_t* s = mir_map_get_tile(sx, sy);
    tile_t* e = mir_map_get_tile(ex, ey);

    if(s == NULL) {error_msg(DEFAULT_C, "Null tile."); }

    unit_move(s->unit, e);
}

void unit_move_e(event_arg_t* arg)
{
    unit_move_xy(arg->move_unit.sx, arg->move_unit.sy, arg->move_unit.ex, arg->move_unit.ey);
}

void unit_move_e_random(event_arg_t* arg)
{
    int dx = 0, dy = 0;
    do
    {
        dx = rand_index(-1, 1);
        dy = rand_index(-1, 1);
    } while(((dx == 0) && (dy == 0))); // or: ((ex != 0) || (ey != 0))

    unit_move_xy(arg->move_unit.sx, arg->move_unit.sy, arg->move_unit.sx + dx, arg->move_unit.sy + dy);
}

//static bool _warrior_can_move_to(unit_t* unit, int x, int y)
//{
//    if(math_dist(unit->x, unit->y, x, y) > 2) return false;
//
//    tile_t* t = mir_map_get_tile(x, y);
//    if(t->entities[LANDSCAPE] == MOUNTAIN) return false;
//    if(t->entities[FIELD] == SEA) return false;
//
//    return true;
//}

//static void _warrior_move_to(unit_t* unit, int x, int y)
//{
//    if(!(unit->state & MOVEABLE)) return;
//
//    tile_t* dest_tile = mir_map_get_tile(x, y);
//    unit_t* dest_unit = dest_tile->unit;
//
//    if(dest_unit)
//    {
//        if(dest_unit->team == unit->team) return;
//        else if(!_warrior_can_attack_to(unit, x, y)) return;
//        _warrior_attack_to(unit, x, y);
//    }
//    if(!_warrior_can_move_to(unit, x, y)) return;
//
//    unit->state &= !MOVEABLE;
//
//    unit->tile->unit = NULL;
//    dest_tile->unit = unit;
//
//    unit->x = x;
//    unit->y = y;
//
//    if(dest_tile->entities[BUILDING] == TOWER) unit->state |= FORTIFIED;
//    else unit->state &= !FORTIFIED;
//}
//
//static bool _warrior_can_attack_to(unit_t* unit, int x, int y)
//{
//    if(math_dist(unit->x, unit->y, x, y) > 2) return false;
//
//    return true;
//}
//
//static void _warrior_attack_to(unit_t* unit, int x, int y)
//{
//    // attack enemy
//}
//
//static void _warrior_draw(unit_t* unit)
//{
////    unit_print_info(unit);
//}



void unit_gen_active_tiles()
{
//    _push_
}









