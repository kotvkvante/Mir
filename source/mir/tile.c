#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

#include "tile.h"

#include "../utils/utils.h"

#include "../math/point.h"
#include "../math/matrix.h"
#include "../math/mrand.h"

#include "../kernel/error_handler.h"

#define MIR_RENDER
#include "../graphics/camera.h"
#include "../graphics/graphics.h"
#include "../graphics/textures.h"
#include "../graphics/texture_map.h"

#include "mir.h"
#include "unit.h"
#include "unit_search_path.h"

#define NONE 0
#define GET(a) texture_map_tiles[a][0]

static char* str_field_info[] =
{
    [NO_FIELD] = "No field",
    [PLAINS] = "Plains",
    [SEA] =    "Sea",
};

static char* str_landscape_info[] =
{
    [NO_LANDSCAPE]    = "No landscape",
    [FOREST]          = "Forest",
    [MOUNTAIN]        = "Mountain",
    [MOUNTAIN_FOREST] = "Rock forest",
};

static char* str_building_info[] =
{
    [NO_BUILDING] = "No building",
    [TOWER] = "Tower",
};

static char* str_unit_info[] =
{
    [NO_UNIT] = "No unit",
    [WARRIOR] = "Warrior",
    [ARCHER]  = "Archer",
    [SHIP]    = "Ship",
    [CANNON]  = "Cannon",
};

static wchar_t* wstr_field_info[] =
{
    [NO_FIELD] = L"No field",
    [PLAINS] = L"Plains",
    [SEA] = L"Water",
};

static wchar_t* wstr_landscape_info[] =
{
    [NO_LANDSCAPE] = L"No landscape",
    [FOREST]    = L"Forest",
    [MOUNTAIN] = L"Mountain",
    [MOUNTAIN_FOREST] = L"Rock forest",
};

static wchar_t* wstr_building_info[] =
{
    [NO_BUILDING] = L"No building",
    [TOWER] = L"Tower",
};

static wchar_t* wstr_unit_info[] =
{
    [NO_UNIT] = L"No unit",
    [WARRIOR] = L"Warrior",
    [ARCHER]  = L"Archer",
    [SHIP]    = L"Ship",
    [CANNON]  = L"Cannon",
};

extern glp_texture_map_t glp_texture_map;
extern glp_texture_map_ex_t glp_texture_map_ex;
extern gl_program_t gl_program;
extern glp_point_t glp_point;

extern GLuint vao;

static float _world_matrix[16];
static float _map_matrix[16];
static float _tile_matrix[16];


int field_textures[FIELDS_COUNT] =
{
    [NONE] = -1,
    [PLAINS] = GET(TM_GRASS),
    [SEA]  = GET(TM_WATER),
//    [OCEAN] = GET(TM_WATER),
//    [SNOW] = GET(TM_WATER),
};
int landscape_textures[LANDSCAPE_COUNT] =
{
    [NONE] = GET(TM_SELECTED),
    [FOREST] = GET(TM_TREE),
    [MOUNTAIN] = GET(TM_MOUNTAIN),
    [MOUNTAIN_FOREST] = GET(TM_MOUNTAIN_FOREST),
};
int building_textures[BUILDING_COUNT] =
{
    [TOWER] = GET(TM_RED_TOWER),
    [VILLAGE] = GET(TM_SELECTED),
};

//int unit_textures[UNIT_COUNT] =
//{
//    [WARRIOR] = GET(TM_RED_WARRIOR),
//    [ARCHER]  = GET(TM_RED_ARCHER),
//    [SHIP]    = GET(TM_BLUE_SHIP),
//};


char* field_info_str[] =
{
    [NONE]   = "No field",
    [PLAINS] = "Plains",
    [SEA]    = "Sea",
//    [SNOW]   = "Snow",
//    [OCEAN]  = "Ocean",
};

char* unit_info_str[] =
{
    [NONE]    = "No unit",
    [WARRIOR] = "Warrior",
    [ARCHER]  = "Archer",
};

// TODO (kotvkvante#1#): fix ...
//
// duplicated from texture_map

char* landscape_info_str[] =
{
    [NONE] = "No landscape",
    [FOREST] = "Forest",
    [MOUNTAIN] = "Mountain",
};

#define arr_size(arr) const int arr ## _size = sizeof(arr) / sizeof(arr[0])
arr_size(field_textures);
arr_size(landscape_textures);
arr_size(building_textures);
//arr_size(unit_textures);

//enum tile_color_types {RED, BLUE};

static int _sizes[] =
{
    [FIELD]     = field_textures_size,
    [LANDSCAPE] = landscape_textures_size,
    [BUILDING]  = building_textures_size,
//    [UNIT]      = unit_textures_size,
};


tile_t test_tile;

void tiles_init()
{
//    test_tile.color = BLUE;
    test_tile.entities[FIELD] = PLAINS;
    test_tile.entities[LANDSCAPE] = MOUNTAIN;
    test_tile.entities[BUILDING] = TOWER;
    test_tile.entities[UNIT] = WARRIOR;
}



void tiles_test_draw()
{
//    static i = 0;
//    tile_draw(&test_tile, (3) % 8, 0);

}



void tile_rand_entities(tile_t* tile)
{
// TODO (kotvkvante#1#): unsafe int 4 bytes ...
//entities array of 4 chars
    int* a = (int*)&tile->entities;
    *a = 0;
    tile->entities[FIELD] = rand_index(1, field_textures_size - 1);

    int i = rand() % 3;
    i++;

    tile->entities[i] = rand_index_zero(_sizes[i] - 1);

//    tile->entities[LANDSCAPE] = rand_index_zero(lanscape_textures_size - 1);
//    tile->entities[BUILDING] = rand_index_zero(building_textures_size - 1);
//    tile->entities[UNIT] = rand_index_zero(unit_textures_size - 1);
}

void tile_rand_entity(tile_t* tile, int entity)
{
    tile->entities[entity] = rand_index(1, _sizes[entity] - 1);
}

void tile_rand_entity_or_nothing(tile_t* tile, int entity)
{
    tile->entities[entity] = rand_index_zero(_sizes[entity] - 1);
}

void tile_rand_field(tile_t* tile)
{
    tile_rand_entity(tile, FIELD);
}

void tile_rand_unit(tile_t* tile)
{
    tile_rand_entity(tile, UNIT);
}

void tile_set_entity(tile_t* tile, char entity_type, char entity_code)
{
    tile->entities[(int)entity_type] = entity_code;
}

char tile_get_entity(tile_t* tile, char entity_type)
{
    return tile->entities[(int)entity_type];
}

int tile_is(tile_t* t, int entity_type, int entity_code)
{
    return t->entities[entity_type] == entity_code;
}

int tile_field_is(tile_t* t, char entity_code)
{
    return tile_is(t, FIELD, entity_code);
}

void tile_reset_entities(tile_t* t)
{
    t->entities[FIELD] = NO_FIELD;
    t->entities[LANDSCAPE] = 0;
    t->entities[BUILDING] = 0;
    t->entities[UNIT] = 0;
}

void tile_draw_begin()
{
    glEnable(GL_BLEND);
    glUseProgram(glp_texture_map.id);
    glBindTexture(GL_TEXTURE_2D, texture_get_tile_map());
    glBindVertexArray(vao);

    glUniformMatrix4fv(glp_texture_map.projection, 1, GL_FALSE, camera_get_projection());
    glUniformMatrix4fv(glp_texture_map.view, 1, GL_FALSE, camera_get_view());

    matrix_identity(_world_matrix);
    matrix_identity(_map_matrix);

    int tmp = mir_map_get_size() >> 1;
    matrix_translate(_map_matrix, -tmp, -tmp, 0.0f);
    matrix_scale(_map_matrix, 64.0f, 64.0f, 0.0f);
}

void tile_draw_end()
{
    glDisable(GL_BLEND);
}

void tile_draw_prepare(int x, int y)
{
    matrix_identity(_tile_matrix);
    matrix_translate(_tile_matrix, x, y, 0.0f);
    matrix_multiply(_world_matrix, _map_matrix, _tile_matrix);
    glUniformMatrix4fv(glp_texture_map.model, 1, GL_FALSE, _world_matrix);
}


void tile_draw_begin_ex()
{
    glEnable(GL_BLEND);
    glUseProgram(glp_texture_map_ex.id);
    glBindTexture(GL_TEXTURE_2D, texture_get_tile_map());
    glBindVertexArray(vao);

    glUniformMatrix4fv(glp_texture_map_ex.projection, 1, GL_FALSE, camera_get_projection());
    glUniformMatrix4fv(glp_texture_map_ex.view, 1, GL_FALSE, camera_get_view());

    matrix_identity(_world_matrix);
    matrix_identity(_map_matrix);

    int tmp = mir_map_get_size() >> 1;
    matrix_translate(_map_matrix, -tmp, -tmp, 0.0f);
    matrix_scale(_map_matrix, 64.0f, 64.0f, 0.0f);
}

void tile_draw_end_ex()
{
//    glDisable(GL_BLEND);
}

void tile_draw_prepare_ex(int x, int y)
{
    matrix_identity(_tile_matrix);
    matrix_translate(_tile_matrix, x, y, 0.0f);
    matrix_multiply(_world_matrix, _map_matrix, _tile_matrix);
    glUniformMatrix4fv(glp_texture_map_ex.model, 1, GL_FALSE, _world_matrix);
}



void tile_frame_draw_begin()
{
    glEnable(GL_BLEND);
    glUseProgram(glp_texture_map.id);
    glBindTexture(GL_TEXTURE_2D, texture_get_tile_map());
    glBindVertexArray(vao);

    glUniformMatrix4fv(glp_texture_map.projection, 1, GL_FALSE, camera_get_projection());
    glUniformMatrix4fv(glp_texture_map.view, 1, GL_FALSE, camera_get_view());

    matrix_identity(_world_matrix);
    matrix_identity(_map_matrix);

    int tmp = mir_map_get_size() >> 1;
    matrix_translate(_map_matrix, tmp + 1, 0, 0.0f);
    matrix_scale(_map_matrix, 64.0f, 64.0f, 0.0f);
}

void tile_frame_draw_end()
{
    glDisable(GL_BLEND);
}

void _tile_draw(int texture)
{
    glUniform2i(glp_texture_map.texture_position, texture, 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void tile_draw(tile_t* tile)
{
    if(tile == NULL) { error_msg_s(DEFAULT_C, "%s: Null tile", __func__ ); }

    if(tile->entities[FIELD]) _tile_draw(field_textures[(int)tile->entities[FIELD]]);
    if(tile->entities[LANDSCAPE]) _tile_draw(landscape_textures[(int)tile->entities[LANDSCAPE]]);
    if(tile->entities[BUILDING]) _tile_draw(building_textures[(int)tile->entities[BUILDING]]);

    if(tile->entities[UNIT])
    {
        if(tile->unit->energy > 0 && tile->entities[FIELD] != SEA && tile->unit->team == mir_get_turn())
        {
            _tile_draw(GET_TEXTURE(TM_ACTIVE_UNIT));
        }
        _tile_draw(unit_get_texture(tile->unit));

        if(tile->unit->health < MIR_UNIT_MAX_HEALTH)
        {

            _tile_draw(GET_TEXTURE(TM_HEALTH_POINT) +  3 - ((tile->unit->health) / 2) );
        }
    }

//    if(tile->entities[UNIT] && (tile->entities[FIELD] == SEA))
//    {
//        _tile_draw();
//    } else if(tile->entities[UNIT]) _tile_draw(unit_textures[(int)tile->entities[UNIT]]);
}

void tile_draw_selected(int x, int y)
{
    matrix_identity(_tile_matrix);
    matrix_translate(_tile_matrix, x, y, 0.0f);
    matrix_multiply(_world_matrix, _map_matrix, _tile_matrix);

    glUniformMatrix4fv(glp_texture_map.model, 1, GL_FALSE, _world_matrix);

    glUniform2i(glp_texture_map.texture_position, GET(TM_SELECTED), 0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void tile_draw_active()
{
    _tile_draw(29);
}

void tile_draw_border(int x, int y, float r, float g, float b)
{
    glUseProgram(glp_point.id);

    matrix_identity(_world_matrix);
    matrix_identity(_map_matrix);

    int tmp = mir_map_get_size() >> 1;

    matrix_identity(_tile_matrix);
#define _BORDER 4
    matrix_scale(_tile_matrix, _BORDER * 2 + 64.0f, _BORDER * 2 + 64.0f, 0.0f);

    matrix_translate(_map_matrix, - (tmp << 6), -(tmp << 6), 0.0f);
    matrix_translate(_map_matrix, (x << 6) - _BORDER, (y << 6) - _BORDER, 0.0f);

    matrix_multiply(_world_matrix, _map_matrix, _tile_matrix);

    glUniform3f(glp_point.color, r, g, b);
    glUniformMatrix4fv(glp_point.model, 1, GL_FALSE, _world_matrix);
    glUniformMatrix4fv(glp_point.projection, 1, GL_FALSE, camera_get_projection());
    glUniformMatrix4fv(glp_point.view, 1, GL_FALSE, camera_get_view());

    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
#undef _BORDER

}

void tile_draw_selected_l(int x, int y)
{
    tile_draw_border(x, y, 0.9f, 0.6f, 0.5f);
}


void tile_draw_hovered_l(int x, int y)
{
    tile_draw_border(x, y, 0.9f, 0.7f, 0.6f);
}

// Only if you want draw one tile!
void tile_draw_single(tile_t* tile, int x, int y)
{
    if(tile == NULL) error_msg(0, "Tile null!\n");

    tile_draw_begin();
    tile_draw_prepare(x, y);

    if(tile->entities[FIELD]) _tile_draw(field_textures[(int)tile->entities[FIELD]]);
    if(tile->entities[LANDSCAPE]) _tile_draw(landscape_textures[(int)tile->entities[LANDSCAPE]]);;
    if(tile->entities[BUILDING]) _tile_draw(building_textures[(int)tile->entities[BUILDING]]);;
//    if(tile->entities[UNIT]) _tile_draw(unit_textures[(int)tile->entities[UNIT]]);;

    tile_draw_end();
}

void tile_draw_info(tile_t* tile)
{
    if(tile == NULL) error_msg(DEFAULT_C, "Draw tile info NULL.\n");

    tile_frame_draw_begin();

    tile_draw_prepare(0, 0);

    if(tile->entities[FIELD]) _tile_draw(field_textures[(int)tile->entities[FIELD]]);
    if(tile->entities[LANDSCAPE]) _tile_draw(landscape_textures[(int)tile->entities[LANDSCAPE]]);;
    if(tile->entities[BUILDING]) _tile_draw(building_textures[(int)tile->entities[BUILDING]]);;
//    if(tile->entities[UNIT]) _tile_draw(unit_textures[(int)tile->entities[UNIT]]);;

    tile_frame_draw_end();
}

void tile_move_entity(tile_t* tile_from, char entity_type, tile_t* tile_dest)
{
    if(tile_from == NULL) error_msg(DEFAULT_C ,"Tile from NULL");
    if(tile_dest == NULL) error_msg(DEFAULT_C ,"Tile dest NULL");

    if(tile_from == tile_dest) return;
    tile_dest->entities[(int)entity_type] = tile_from->entities[(int)entity_type];
    tile_from->entities[(int)entity_type] = 0;
}

void tile_move_unit(tile_t* tile_from, tile_t* tile_dest)
{
    tile_move_entity(tile_from, UNIT, tile_dest);
}

void tile_get_info_str(tile_t* tile, char* str, int size)
{
    snprintf(str, size, PATTERN_TILE_INFO_STR(tile));
}

void tile_get_info_wstr(tile_t* tile, wchar_t* str, int size)
{
    int x = -1, y = -1, e = -1, team = -1, dev = -1, health = -1;
    if(tile->entities[UNIT] != NO_UNIT)
    {
        x = tile->unit->x;
        y = tile->unit->y;
        e = tile->unit->energy;
        team = tile->unit->team;
        dev = tile->unit->devision;
        health = tile->unit->health;
    }
    int res = swprintf(str, size, PATTERN_TILE_INFO_WSTR(tile));
    if(res < 0) log_msg_s(DEFAULT_C, "%s: res < 0", __func__);
}


void tile_print_info(tile_t* t)
{
    printf("Tile: %s %s\t[unit] %p\n", field_info_str[(int)t->entities[FIELD]], unit_info_str[(int)t->entities[UNIT]], t->unit);
//    t->entities[LANDSCAPE];
//    t->entities[BUILDING];
//    t->entities[UNIT];
}
