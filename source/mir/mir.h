#ifndef MIR_H_INCLUDED
#define MIR_H_INCLUDED

// size of tile in pixels
#define DEFAULT_MAP_SIZE 10
#define DEFAULT_TILE_SIZE 64

#include <stdbool.h>
#include "../math/point.h"

typedef enum mir_states_t {MIR_START, MIR_PAUSE, MIR_END} mir_states_t;
typedef enum team {TEAM_RED = 0, TEAM_BLUE = 1, TEAM_COUNT} team_enum;

enum mir_events { MOVE_UNIT, BUILD_SOMETHING, CHANGE_LANDSCAPE, CHANGE_FIELD };
//typedef bool (*move_unit)(int sx, int sy, int ex, int ey);
//typedef bool (*build_something)(int x, int y, int building_type);
//typedef bool (*change_landscape)(int x, int y, int landscape_type);
//typedef bool (*change_field)(int x, int y, int field_type);
//
//typedef bool (*move_unit)()



typedef struct team_t team_t;
typedef struct tile_t tile_t;
typedef struct mir_t mir_t;

typedef struct player_t player_t;
typedef struct ai_t ai_t;

typedef struct mir_t
{
    mir_states_t state;


    team_enum turn;
    team_t* teams;
    tile_t* tiles;

    int size;
    float position[16];

    point2i_t selected_tile;
    point2i_t hovered_tile;

    int active_tiles_count;
    int* active_tiles;


} mir_t;


void mir_init();
void mir_refresh();

void mir_turn();
int mir_get_turn();

void mir_start();
int mir_is_started();
void mir_pause();
void mir_end();

void mir_handle_events(void);

tile_t* mir_map_get_tile(int x, int y);
tile_t* mir_map_get_tile_safe(int x, int y);
tile_t* mir_map_get_tile_i(int i);

int mir_map_get_tile_from_screen(int x, int y, int* ox, int* oy);

void mir_map_handle_mouse_button(int btn, int action);


void mir_map_set_size(int size);
int  mir_map_get_size();

int mir_map_get_selected_tile_type();
int mir_map_get_tile_entity(int x, int y, int code);
int mir_map_get_tile_unit(int x, int y);
int mir_map_get_tile_field(int x, int y);
//tile_t* mir_map_get_selected_tile();
tile_t* mir_map_get_selected_tile(int* x, int* y);
tile_t* mir_map_get_hovered_tile(int* x, int* y);
int mir_map_get_distance(int x1, int y1, int x2, int y2);


team_t* mir_map_get_team();

void mir_map_add_active(int x, int y);

void mir_map_gen();
void mir_draw_team();
void mir_draw_map();
void mir_map_draw_active();
void mir_map_draw_pickmap();


void mir_map_deselect();

void mir_map_set_selected_tile_xy(int x, int y);
void mir_map_set_hovered_tile(int x, int y);
void mir_map_set_selected_tile(tile_t* t);


bool mir_map_is_xy_on_map(int x, int y);
//void mir_draw_tile(int x, int y, tile_t* tile);
//void mir_draw_tile_ex(int x, int y, tile_t* tile);
void mir_draw_tile_tm(int x, int y, tile_t* tile);
void mir_draw_tile_tm_s(int x, int y, tile_t* tile);

void mir_draw_selected_tile();

//void mir_map_get_pos(int* x, int* y);
void mir_map_handle_hovered_f(float x, float y);
void mir_map_handle_hovered(int x, int y);
void mir_map_unhover();

//void mir_map_handle_click(float x, float y);


//void mir_draw_tile_loop(int i, int j);
//void mir_draw_tile(int i, int j);

void begin_draw_tiles_ex();
void end_draw_tiles_ex();

void mir_print_map();


#endif // MIR_H_INCLUDED
