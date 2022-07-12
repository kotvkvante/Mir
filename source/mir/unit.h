#ifndef UNIT_H_INCLUDED
#define UNIT_H_INCLUDED

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
    int heath;
    int state;

// deprected
//    unit_attack_t attack;
//    unit_move_t move;
//    unit_draw_t draw;
} unit_t;


void units_init();

void units_draw();

int unit_get_texture();
void unit_print_info(unit_t* unit);
int unit_can_move(unit_t* unit, tile_t* tile_dest);
void unit_move(unit_t* unit, tile_t* tile_dest);
#endif // UNIT_H_INCLUDED
