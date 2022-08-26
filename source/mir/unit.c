#include <stdio.h>
#include <stdlib.h>

#include "../math/point.h"
#include "../math/mrand.h"
#include "../utils/utils.h"
#include "../graphics/texture_map.h"
#include "../kernel/error_handler.h"

#include "mir.h"

#include "unit.h"
#include "unit_search_path.h"
#include "tile.h"
#include "team.h"

unit_t* _units_red[10]; int red_id = 0;
unit_t* _units_blue[10]; int blue_id = 0;

unit_t** units_by_team[2] = {[TEAM_RED] = _units_red, [TEAM_BLUE] = _units_blue};


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


int units_count;
unit_t* units[10];

void unit_init(unit_t* unit, int type, int team, int x, int y);
void unit_print_info(unit_t* unit);

//static void _warrior_attack_to(unit_t* unit, int x, int y);
//static bool _warrior_can_attack_to(unit_t* unit, int x, int y);
//static void _warrior_move_to(unit_t* unit, int x, int y);
//static bool _warrior_can_move_to(unit_t* unit, int x, int y);
//static void _warrior_draw(unit_t* unit);

//static int _is_units_init = false;
unit_t test_unit;
unit_t test_unit2;
unit_t test_unit3;
unit_t test_unit4, test_unit5, test_unit6;


void units_init()
{
    unit_search_path_init();

    units_count = 0;
//    if(_is_units_init) return;
    unit_init(&test_unit,  WARRIOR, TEAM_RED, 4, 4);
    unit_init(&test_unit2, ARCHER, TEAM_RED, 5, 5);
    unit_init(&test_unit3, WARRIOR, TEAM_RED, 3, 4);
    unit_init(&test_unit4, WARRIOR, TEAM_BLUE, 0, 0);
    unit_init(&test_unit5, ARCHER, TEAM_BLUE, 0, 1);
    unit_init(&test_unit6, ARCHER, TEAM_BLUE, 1, 0);

    unit_print_info(&test_unit);
//    _is_units_init = true;
}

void units_refresh()
{
    for(int i = 0; i < units_count; i++)
    {

        units[i]->energy = 1;
    }
}

void unit_init(unit_t* unit, int type, int team, int x, int y)
{
    unit->x = x;
    unit->y = y;
    unit->type = type;
    unit->team = team;
    unit->energy = 1;
    unit->health  = MIR_UNIT_MAX_HEALTH - rand() % 6;

    unit->texture = _unit_textures[type][team];


//    unit->attack = attack;
//    unit->move = move;
//    unit->draw = draw;

    unit->tile = mir_map_get_tile(x, y);
    unit->tile->unit = unit;
    unit->tile->entities[UNIT] = type;

    units[units_count] = unit;
    unit->id = units_count;
    unit_gen_unique_devision(unit);
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
//    if(unit->team != mir_get_turn())
    if(unit->energy == 0) return false;

    switch(unit->type)
    {
    case WARRIOR:
        if(mir_map_get_distance(unit->x, unit->y, tile_dest->x, tile_dest->y) > 2) return false;

//        if(tile_dest->entities[FIELD] == SEA)
//            if(!mir_map_get_team()->is_navigation_reseached) { return false; }
//        if(tile_dest->entities[LANDSCAPE] == MOUNTAIN || tile_dest->entities[LANDSCAPE] == MOUNTAIN_FOREST)
//            if(!mir_map_get_team()->is_rock_climbing_reseached) { return false; }


    break;

    case ARCHER:
        if(mir_map_get_distance(unit->x, unit->y, tile_dest->x, tile_dest->y) > 8) return false;

//        if(tile_dest->entities[FIELD] == SEA)
//            if(!mir_map_get_team()->is_navigation_reseached) { return false; }
//        if(tile_dest->entities[LANDSCAPE] == MOUNTAIN || tile_dest->entities[LANDSCAPE] == MOUNTAIN_FOREST)
//            if(!mir_map_get_team()->is_rock_climbing_reseached) { return false; }

    break;
    }

    return true;
}


bool unit_can_move(unit_t* unit, tile_t* tile_dest)
{
//    print_2i(unit->team, mir_get_turn());
    if(unit->team != mir_get_turn()) { return false;}
    if(unit->energy == 0) return false;

    switch(unit->type)
    {
    case WARRIOR:
//        if(mir_map_get_distance(unit->x, unit->y, tile_dest->x, tile_dest->y) > 2) { return false; }
        unit_calc_active_tiles(unit);
        if(trav_map_xy_safe(tile_dest->x - unit->x, tile_dest->y - unit->y) == 0) { return false; }
        if(tile_dest->entities[FIELD] == SEA)
            if(!mir_map_get_team()->is_navigation_reseached) { return false; }
        if(tile_dest->entities[LANDSCAPE] == MOUNTAIN || tile_dest->entities[LANDSCAPE] == MOUNTAIN_FOREST)
            if(!mir_map_get_team()->is_rock_climbing_reseached) { return false; }

    break;

    case ARCHER:
//        here;
//        if(mir_map_get_distance(unit->x, unit->y, tile_dest->x, tile_dest->y) > 8) return false;
//        {
//        int dx = tile_dest->x - unit->x, dy = tile_dest->y - unit->y;
//        print_2i(dx, dy);
//        print_i(trav_map_xy_safe(dx, dy));

//        }
        unit_calc_active_tiles(unit);
        if(trav_map_xy_safe(tile_dest->x - unit->x, tile_dest->y - unit->y) == 0) { return false; }
        if(tile_dest->entities[FIELD] == SEA)
            if(!mir_map_get_team()->is_navigation_reseached) { return false; }
        if(tile_dest->entities[LANDSCAPE] == MOUNTAIN || tile_dest->entities[LANDSCAPE] == MOUNTAIN_FOREST)
            if(!mir_map_get_team()->is_rock_climbing_reseached) { return false; }


    break;
    }

    return true;
}

bool unit_warrior_can_visit_xy(int x, int y)
{
//    print_2i(x, y);
    tile_t* t = mir_map_get_tile(x, y);
    unit_warrior_can_visit_tile(t);
}

bool unit_archer_can_visit_xy(int x, int y)
{
    tile_t* t = mir_map_get_tile(x, y);
    unit_archer_can_visit_tile(t);
}

bool unit_warrior_can_visit_tile(tile_t* tile_dest)
{
    if(tile_dest->entities[FIELD] == SEA)
        if(!mir_map_get_team()->is_navigation_reseached) { return false; }
    if(tile_dest->entities[LANDSCAPE] == MOUNTAIN || tile_dest->entities[LANDSCAPE] == MOUNTAIN_FOREST)
        if(!mir_map_get_team()->is_rock_climbing_reseached) { return false; }

    return true;
}

bool unit_archer_can_visit_tile(tile_t* tile_dest)
{
    if(tile_dest->entities[FIELD] == SEA)
        if(!mir_map_get_team()->is_navigation_reseached) { return false; }
    if(tile_dest->entities[LANDSCAPE] == MOUNTAIN || tile_dest->entities[LANDSCAPE] == MOUNTAIN_FOREST)
        if(!mir_map_get_team()->is_rock_climbing_reseached) { return false; }

    return true;
}


bool unit_can_move_xy(int sx, int sy, int ex, int ey)
{
    unit_t* unit = mir_map_get_tile(sx, sy)->unit;
    tile_t* tile = mir_map_get_tile(ex, ey);

    return unit_can_move(unit, tile);
}


void unit_move(unit_t* unit, tile_t* tile_dest)
{
    if(unit == NULL) { error_msg_s(DEFAULT_C, "%s: Unit NULL.", __func__); return; }
    if(tile_dest == NULL) { error_msg_s(DEFAULT_C, "%s: Tile NULL.", __func__); return; }



    unit->x = tile_dest->x;
    unit->y = tile_dest->y;

    unit->tile->entities[UNIT] = NO_UNIT;
    unit->tile->unit = NULL;

    tile_dest->entities[UNIT] = unit->type;
    tile_dest->unit = unit;
    unit->tile = tile_dest;

    if(tile_dest->entities[FIELD] == SEA) unit->texture = _unit_textures[SHIP][unit->team];
    else unit->texture = _unit_textures[unit->type][unit->team];


    unit->energy = 0;
}

void unit_swap(unit_t* unit, tile_t* tile_dest)
{
    if(unit == NULL) { error_msg_s(DEFAULT_C, "%s: Unit NULL.", __func__); return; }
    if(tile_dest == NULL) { error_msg_s(DEFAULT_C, "%s: Tile NULL.", __func__); return; }

    tile_t* tile_from = unit->tile;

    unit_t* tmp_unit = tile_from->unit;
    tile_from->unit = tile_dest->unit;
    tile_dest->unit = tmp_unit;

//    tmp_x = unit->x;
//    tmp_y = unit->y;

    tile_from->unit->x = tile_from->x;
    tile_from->unit->y = tile_from->y;
    tile_dest->unit->x = tile_dest->x;
    tile_dest->unit->y = tile_dest->y;

    unit->tile = tile_dest;
    tile_from->unit->tile = tile_from;

//    unit->tile->entities[UNIT] = NO_UNIT;
//    unit->tile->unit = NULL;

    tile_dest->entities[UNIT] = tile_dest->unit->type;
    tile_from->entities[UNIT] = tile_from->unit->type;

    tile_dest->unit->energy = 0;
    tile_from->unit->energy = 0;
}

void unit_move_xy(int sx, int sy, int ex, int ey)
{
//    print_2i(sx, sy);
    tile_t* s = mir_map_get_tile(sx, sy);
    tile_t* e = mir_map_get_tile(ex, ey);

    if(s == NULL) {error_msg(DEFAULT_C, "Null tile."); }

    unit_move(s->unit, e);
}

void unit_attack(unit_t* unit, tile_t* tile_dest)
{
    tile_dest->unit->health--;

    unit->energy = 0;
}


void unit_event_move(event_arg_t* arg)
{
    tile_t* start = mir_map_get_tile_safe(arg->move_unit.sx, arg->move_unit.sy);
    tile_t* end   = mir_map_get_tile_safe(arg->move_unit.ex, arg->move_unit.ey);

    if( (start == NULL) || (end == NULL) ) return;


    if(start->entities[UNIT] == NO_UNIT) { log_msg_s(GAME_C, "%s: no unit.", (char*)__func__); return; }
    if(start->unit->team != mir_get_turn()) { log_msg_s(GAME_C, "%s: unit from different team.", (char*)__func__); return; }


    if(end->entities[UNIT] == NO_UNIT)
    {
        unit_move_e(arg);
        return;
    }

    if(end->unit->team == mir_get_turn())
    {
        unit_swap_e(arg);
        return;
    }

    if(end->unit->team != mir_get_turn())
    {
//        here;
        unit_attack_e(arg);
        return;
    }
}

void unit_move_e(event_arg_t* arg)
{
    if(unit_can_move_xy(arg->move_unit.sx, arg->move_unit.sy, arg->move_unit.ex, arg->move_unit.ey))
    {
        unit_move_xy(arg->move_unit.sx, arg->move_unit.sy, arg->move_unit.ex, arg->move_unit.ey);
    }

}

void unit_attack_e(event_arg_t* arg)
{
//    log_msg(DEFAULT_C, "ATTACK!");
    tile_t* tile = mir_map_get_tile(arg->move_unit.ex, arg->move_unit.ey);
    unit_t* unit = mir_map_get_tile(arg->move_unit.sx, arg->move_unit.sy)->unit;

    if(unit_can_attack(unit, tile))
    {
        unit_attack(unit, tile);
    }

//    if(unit_can_move_xy())
//    log_msg(DEFAULT_C, "Not implemented.");
}

void unit_swap_e(event_arg_t* arg)
{
    tile_t* start = mir_map_get_tile(arg->move_unit.sx, arg->move_unit.sy);
    tile_t* end   = mir_map_get_tile(arg->move_unit.ex, arg->move_unit.ey);

//    if( (unit->energy == 0) || (tile->unit->energy == 0) )
//        return;
    bool r1 = unit_can_move(start->unit, end);
    bool r2 = unit_can_move(end->unit, start);

    print_2i(r1, r2);

    if(r1 && r2) unit_swap(start->unit, end);
}


void unit_move_e_random(event_arg_t* arg)
{
    int dx = 0, dy = 0;
    int i = 0;
    do
    {
        dx = rand_index(-1, 1);
        dy = rand_index(-1, 1);
        i++;
        if(!mir_map_is_xy_on_map(arg->move_unit.sx + dx, arg->move_unit.sy + dy)) continue;
        if(!unit_can_move_xy(arg->move_unit.sx, arg->move_unit.sy, arg->move_unit.sx + dx, arg->move_unit.sy + dy)) continue;


        unit_move_xy(arg->move_unit.sx, arg->move_unit.sy, arg->move_unit.sx + dx, arg->move_unit.sy + dy);
        return;

    } while(i < 100); // or: ((ex != 0) || (ey != 0))

    print_i(i);
    error_msg_s(DEFAULT_C, "%s: unreacheble!!", (char*)__func__);
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

void unit_calc_active_tiles(unit_t* unit)
{
    if(unit == NULL) { error_msg_s(DEFAULT_C, "%s: Unit NULL.", __func__); return; }

    unit_search_path_clear();
    if(unit->energy == 0) return;



    switch(unit->type)
    {
    case WARRIOR:
        unit_search_path_8(unit);
    break;

    case ARCHER:
        unit_search_path(unit);
    break;

    default:
        log_msg_s(DEFAULT_C, "Untracked unit.", __func__);
    break;
    }
//    trav_print();


    return;
}

void unit_gen_unique_devision(unit_t* unit)
{
    if(unit->id == 0)
    {
        unit->devision = 1 + rand() % 4;
        return;
    }

    int k = units[unit->id-1]->devision;
    unit->devision = k + 1 + rand() % 4;
}

//void unit_gen_rand_name(unit_t* unit)
//{
//    if(unit->name != NULL) free(unit->name);
//}
//
//void unit_set_name(unit_t* unit, const char* name)
//{
//    if(unit->)
//
//}











