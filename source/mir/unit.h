#ifndef UNIT_H_INCLUDED
#define UNIT_H_INCLUDED

#include <stdbool.h>


#include "mir_queue.h"


typedef struct unit_t unit_t;
typedef struct tile_t tile_t;

// deprecated
//typedef void (*unit_attack_t)(unit_t* unit, int x, int y);
//typedef void (*unit_move_t)(unit_t* unit, int x, int y);
//typedef void (*unit_draw_t)(unit_t* unit);

typedef struct unit_t
{
    int x, y;
    tile_t* tile;
    int type;
    int team;
    int texture;
    int health;
    int state;

    int energy;

// deprected
//    unit_attack_t attack;
//    unit_move_t move;
//    unit_draw_t draw;
    int devision;
    int id;
} unit_t;


void units_init();
void units_refresh();

void units_draw();

void unit_gen_rand_name(unit_t* unit);
void unit_set_name(unit_t* unit, const char* name);


int unit_get_texture();
void unit_print_info(unit_t* unit);

bool unit_can_move(unit_t* unit, tile_t* tile_dest);
bool unit_can_move_xy(int sx, int sy, int ex, int ey);

bool unit_warrior_can_visit_xy(int x, int y);
bool unit_archer_can_visit_xy(int x, int y);

bool unit_warrior_can_visit_tile(tile_t* tile_dest);
bool unit_archer_can_visit_tile(tile_t* tile_dest);


void unit_move(unit_t* unit, tile_t* tile_dest);
void unit_move_xy(int sx, int sy, int ex, int ey);

void unit_event_move(event_arg_t* arg);

void unit_move_e(event_arg_t* arg);
void unit_attack_e(event_arg_t* arg);
void unit_swap_e(event_arg_t* arg);


void unit_move_e_random(event_arg_t* arg);

void unit_calc_active_tiles(unit_t* unit);

void unit_gen_unique_devision(unit_t* unit);



#endif // UNIT_H_INCLUDED
