#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>


#define MIR_RENDER

#include "mir.h"
#include "mir_queue.h"


#include "../kernel/error_handler.h"
#include "../graphics/graphics.h"
#include "../kernel/kernel.h"
#include "../kernel/kernel_t.h"
#include "../math/matrix.h"
#include "../graphics/textures.h"
#include "../graphics/camera.h"
#include "../utils/utils.h"
#include "../graphics/texture_map.h"

#include "tile.h"
#include "unit.h"
#include "team.h"

static float _world_matrix[16];
static float _map_matrix[16];
static float _tile_matrix[16];


extern gl_program_t gl_program;
extern glp_base_t glp_base;
extern glp_texture_map_t glp_texture_map;
extern glp_texture_map_s_t glp_texture_map_s;

extern textures_t textures;
extern GLuint vbo;
extern GLuint vao;
extern GLuint vao_base;
extern GLuint FramebufferName;

extern GLint size_vert;
extern kernel_t kernel;

extern char* unit_info_str[];

extern team_t teams;

point3uc_t team_colors[TEAM_COUNT] =
{
    [TEAM_RED]  = (point3uc_t){161, 29, 45},
    [TEAM_BLUE] = (point3uc_t){26, 95, 180},
};



mir_t mir;

void mir_init()
{
    mqueue_init();

    mir.turn = TEAM_RED;
    mir.teams = &teams;
    mir.state = MIR_END;
    mir.active_tiles_count = 0;
    mir.active_tiles = malloc(sizeof(int) * 9);

    tiles_init();

    mir_map_set_size(DEFAULT_MAP_SIZE);
    mir.tiles = malloc(sizeof(tile_t) * DEFAULT_MAP_SIZE * DEFAULT_MAP_SIZE);
    mir.selected_tile = (point2i_t)
    {
        3, 5
    };

    mir_map_gen();
}

void mir_start() { mir.state = MIR_START; }
void mir_pause() { mir.state = MIR_PAUSE; }
void mir_end()   { mir.state = MIR_END;   }

int mir_is_started() { return (mir.state == MIR_START); }


//int mir_map_get_selected_tile_type()
//{
//    return mir_map_get_selected_tile()
//}


void mir_map_handle_mouse_button(int btn, int action)
{
    tile_t* selected_tile = mir_map_get_selected_tile(NULL, NULL);
    tile_t* hovered_tile  = mir_map_get_hovered_tile(NULL, NULL);
    event_f f = NULL;

    if( hovered_tile && !selected_tile)
    {
        goto SELECT;
    }

    if( selected_tile == hovered_tile )
    {
//        log_msg(GAME_C, "You already select this tile!");
        goto DESELECT;
    }

    if(selected_tile)
    {
        if(selected_tile->entities[UNIT] == NO_UNIT)
        {
            goto SELECT;
        }

        if(hovered_tile->entities[UNIT] == NO_UNIT)
        {
            f = unit_move_e;
            goto ENQ;
        }

        if(hovered_tile->unit->team == mir_get_turn())
        {
//            here;
            f = unit_swap_e;
            goto ENQ;
        }

        if(hovered_tile->unit->team != mir_get_turn())
        {
            f = unit_attack_e;
            goto ENQ;
        }

//        if(hovered_tile->entities[UNIT] != NO_UNIT)
//        {
//            if(hovered_tile->unit->team == mir_get_turn())
//            {
//                goto SELECT;
//            }
//        }
        if(selected_tile->unit->team != mir_get_turn())
        {
            goto SELECT;
        }


        if(unit_can_move(selected_tile->unit, hovered_tile))
        {
//                mir_event_t e = {
//                    .type = MOVE_UNIT,
//                    .move_unit = {mir.selected_tile.x, mir.selected_tile.y, mir.hovered_tile.x, mir.hovered_tile.y}
//                };
//                mqueue_event_enqueue(e);


//                unit_move(selected_tile->unit, tile2);
        }
        goto DESELECT;
    }




ENQ:
    {
        event_t e = {
        .f = f,
        .arg.move_unit = {
            .sx = mir.selected_tile.x,
            .sy = mir.selected_tile.y,
            .ex = mir.hovered_tile.x,
            .ey = mir.hovered_tile.y
        },
        };
        mqueue_fevent_enqueue(e);
    }

DESELECT:
    mir_map_deselect();
    return;

SELECT:
//    here;
    mir_map_set_selected_tile(hovered_tile);
    return;
}



void mir_handle_events(void)
{
    event_t e;
    while(mqueue_fevent_dequeue(&e))
    {
        e.f(&e.arg);
    }

    mqueue_reinit();

//    while(mqueue_event_dequeue(&event))
//    {
////        print_i(event.type);
//        switch (event.type)
//        {
//        case MOVE_UNIT:
//            {
//                tile_t* selected_tile = mir_map_get_tile(event.move_unit.sx, event.move_unit.sy);
//                tile_t* tile2 = mir_map_get_tile(event.move_unit.ex, event.move_unit.ey);
//
//                if(tile2 == NULL || selected_tile == NULL) {error_msg(DEFAULT_C, "Null."); return; }
//
//                if(unit_can_move(selected_tile->unit, tile2)) unit_move(selected_tile->unit, tile2);
//            } break;
//        case BUILD_SOMETHING:
//
//            break;
//        default:
//            log_msg(DEFAULT_C, "Untracked event.");
//            break;
//        }
//    }
}

void mir_refresh()
{
    units_refresh();

}

void mir_turn()
{
    if(mir.turn == TEAM_COUNT - 1)
    {
        mir_refresh();

        mir.turn = TEAM_RED;
        return;
    };
    mir.turn++;
}

int mir_get_turn()
{
    return mir.turn;
}

void mir_map_deselect()
{
    mir.selected_tile.x = -1;
    mir.active_tiles_count = 0;
}

void mir_map_set_selected_tile_xy(int x, int y)
{
    mir.selected_tile.x = x;
    mir.selected_tile.y = y;
}

void mir_map_set_hovered_tile(int x, int y)
{
    mir.hovered_tile.x = x;
    mir.hovered_tile.y = y;
}

void mir_map_set_selected_tile(tile_t* t)
{
    mir.selected_tile.x = t->x;
    mir.selected_tile.y = t->y;
//    log_msg_va(GAME_C, "Select new tile: [%d][%d]\n", mir.selected_tile.x, mir.selected_tile.y);
//    return;
}

// (x1 - x2) ** 2 + (y1 - y2) ** 2
int mir_map_get_distance(int x1, int y1, int x2, int y2)
{
    return math_sqr(x1 - x2) + math_sqr(y1 - y2);
}

int mir_map_get_tile_from_screen(int x, int y, int* ox, int* oy)
{
    unsigned char pixel[4];
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pixel);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    *ox = pixel[0];
    *oy = pixel[1];

    return pixel[2];
}



tile_t* mir_map_get_selected_tile(int* x, int* y)
{
    if(mir.selected_tile.x == -1) return NULL;

    if(x != NULL) *x = mir.selected_tile.x;
    if(y != NULL) *y = mir.selected_tile.y;

    return &mir.tiles[mir.selected_tile.x * mir.size + mir.selected_tile.y];
}

tile_t* mir_map_get_hovered_tile(int* x, int *y)
{
    if(mir.hovered_tile.x == -1) return NULL;

    if(x != NULL) *x = mir.hovered_tile.x;
    if(y != NULL) *y = mir.hovered_tile.y;

    return &mir.tiles[mir.hovered_tile.x * mir.size + mir.hovered_tile.y];

}

//int mir_map_get_selected_tile_type()
//{
//    return ->field_type;
//}

int mir_map_get_selected_tile_entity(int entity_code)
{
    return mir.tiles[mir.selected_tile.x * mir.size + mir.selected_tile.y].entities[entity_code];
}

int mir_map_get_selected_tile_unit()
{
    return mir_map_get_selected_tile_entity(UNIT);
}


tile_t* mir_map_get_tile(int x, int y)
{
    return &mir.tiles[x * DEFAULT_MAP_SIZE + y];
}

tile_t* mir_map_get_tile_safe(int x, int y)
{
    if(!mir_map_is_xy_on_map(x, y)) return NULL;
    return &mir.tiles[x * DEFAULT_MAP_SIZE + y];
}

// i = mir.size * x + y
tile_t* mir_map_get_tile_i(int i)
{
    return &mir.tiles[i];
}

int mir_map_get_tile_entity(int x, int y, int code)
{
    return mir.tiles[x * mir.size + y].entities[code];
}

int mir_map_get_tile_unit(int x, int y)
{
    return mir_map_get_tile_entity(x, y, UNIT);
}

int mir_map_get_tile_field(int x, int y)
{
    return mir_map_get_tile_entity(x, y, FIELD);
}

void mir_map_set_size(int size)
{
    mir.size = size;
    camera_set_map_projection(size);
}


int mir_map_get_size()
{
    return mir.size;
}


team_t* mir_map_get_team()
{
    return &mir.teams[mir.turn];
}



void mir_map_add_active(int x, int y)
{
    int k = 0;
    int s = x * mir.size + y;

    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            if(!mir_map_is_xy_on_map(x+i, y+j)) continue;
            int t = s + i * mir.size + j;

            if(mir.tiles[t].entities[FIELD] == PLAINS && mir.tiles[t].entities[UNIT] == 0)
            {
                mir.active_tiles[k] = t;
                k++;
            }
        }
    }
    mir.active_tiles_count = k;
}

void mir_map_gen()
{
    for(int i = 0; i < mir.size; i++)
    {
        for(int j = 0; j < mir.size; j++)
        {
            tile_t* t = &mir.tiles[i * mir.size + j];
            t->unit = NULL;
            t->x = i;
            t->y = j;

            tile_reset_entities(t);
            tile_rand_field(t);
//            t->ent |= FIELD_F;

            if(rand() % 4) continue;
            if(tile_get_entity(t, FIELD) == PLAINS)
            {
//                tile_rand_entity_or_nothing(t, UNIT);
//                if(t->entities[UNIT] )
//                    if(t->entities[UNIT] == SHIP) t->entities[UNIT] = 0;
                tile_rand_entity_or_nothing(t, LANDSCAPE);
            }
        }
    }
    units_init();
}


void mir_print_map()
{
    for(int i = mir.size - 1; i >= 0; i--)
    {
        for(int j = 0; j < mir.size ; j++)
        {
            printf("%2.1d", mir.tiles[j * mir.size + i].entities[UNIT]);
        }
        printf("\n");
    }
}

//static void _begin_draw_tiles()
//{
//    glEnable(GL_BLEND);
//    glUseProgram(gl_program.id);
//    glBindTexture(GL_TEXTURE_2D, textures.tile_map);
//}
//
//static void _end_draw_tiles()
//{
//    glBindTexture(GL_TEXTURE_2D, 0);
//    glDisable(GL_BLEND);
//}

void mir_draw_team()
{
    draw_rectangle_xy_wh_rgb((point2i_t)
    {
        ((int)kernel_get_window_width()) - 30, ((int)kernel_get_window_height()) - 30
    },
    (point2i_t)
    {
        20, 20
    },
    team_colors[mir.turn]);
}


void begin_draw_tiles_ex()
{
    glEnable(GL_BLEND);
    glUseProgram(glp_base.id);
    glBindTexture(GL_TEXTURE_2D, textures.tile_map);
}

void end_draw_tiles_ex()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
}

#define p glp_texture_map
void begin_draw_tiles_tm()
{
    glEnable(GL_BLEND);
    glUseProgram(glp_texture_map.id);
    glBindTexture(GL_TEXTURE_2D, textures.tile_map);
    glBindVertexArray(vao);


    glUniformMatrix4fv(p.projection, 1, GL_FALSE, camera_get_projection());
    glUniformMatrix4fv(p.view, 1, GL_FALSE, camera_get_view());


    matrix_identity(_world_matrix);
    matrix_identity(_map_matrix);
    int tmp = mir_map_get_size() >> 1;
    matrix_translate(_map_matrix, -tmp, -tmp, 0.0f);
    matrix_scale(_map_matrix, 64.0f, 64.0f, 0.0f);
}
#undef p

#define p glp_texture_map_s
void begin_draw_tiles_tm_pick()
{
    glEnable(GL_BLEND);
    glUseProgram(p.id);
    glBindTexture(GL_TEXTURE_2D, textures.tile_map);
    glBindVertexArray(vao);

    glUniformMatrix4fv(p.projection, 1, GL_FALSE, camera_get_projection());
//    glUniformMatrix4fv(p.projection, 1, GL_FALSE, camera_get_map_projection());

//    float tmp[16];
//    matrix_identity(tmp);
////    matrix_translate(tmp, 256 / 2, 256 / 2, 0);
//    glUniformMatrix4fv(p.view, 1, GL_FALSE, camera_get_view());
//
//    matrix_identity(_world_matrix);
//    matrix_identity(_map_matrix);
//    matrix_scale(_map_matrix, 64.0f, 64.0f, 0.0f);

    glUniformMatrix4fv(p.view, 1, GL_FALSE, camera_get_view());


    matrix_identity(_world_matrix);
    matrix_identity(_map_matrix);
    int tmp = mir_map_get_size() >> 1;
    matrix_translate(_map_matrix, -tmp, -tmp, 0.0f);
    matrix_scale(_map_matrix, 64.0f, 64.0f, 0.0f);

}
#undef p

void end_draw_tiles_tm()
{
    glDisable(GL_BLEND);
}

void end_draw_tiles_tm_pick()
{
    glDisable(GL_BLEND);
}

void mir_draw_map_tm()
{
    begin_draw_tiles_tm();
    for(int i = 0; i < mir.size; i++)
    {
        for(int j = 0; j < mir.size; j++)
        {
            mir_draw_tile_tm(i, j, &mir.tiles[i * mir.size + j]);
        }
    }
    mir_draw_selected_tile();

    end_draw_tiles_tm();
}

void mir_map_draw_pickmap()
{
    begin_draw_tiles_tm_pick();

    for(int i = 0; i < mir.size; i++)
    {
        for(int j = 0; j < mir.size; j++)
        {
            mir_draw_tile_tm_s(i, j, &mir.tiles[i * DEFAULT_MAP_SIZE + j]);
        }
    }
//    mir_draw_selected_tile();

    end_draw_tiles_tm_pick();
}


void depr_mir_draw_map()
{
    begin_draw_tiles_tm();
    for(int i = 0; i < mir.size; i++)
    {
        for(int j = 0; j < mir.size; j++)
        {
            mir_draw_tile_tm(i, j, &mir.tiles[i * DEFAULT_MAP_SIZE + j]);
        }
    }

    mir_draw_selected_tile();
    end_draw_tiles_tm();
}

void mir_map_draw_active()
{
    tile_draw_begin();
    for(int i = 0; i < mir.active_tiles_count; i++)
    {
        tile_draw_prepare(mir.active_tiles[i] / mir.size, mir.active_tiles[i] % mir.size);
        tile_draw_active();
    }
    tile_draw_end();
}

void mir_draw_map()
{
    tile_draw_begin();
    for(int i = 0; i < mir.size; i++)
    {
        for(int j = 0; j < mir.size; j++)
        {
            tile_draw_prepare(i, j);
            tile_draw(&mir.tiles[i * mir.size + j]);
        }
    }

    tile_t* t = mir_map_get_selected_tile(NULL, NULL);
    if(t) tile_draw_info(t);

    if(mir.selected_tile.x != -1)
    {
        tile_draw_info(mir_map_get_selected_tile(NULL, NULL));
        tile_draw_selected_l(mir.selected_tile.x, mir.selected_tile.y);
        tile_draw_begin();
        tile_draw_prepare(mir.selected_tile.x, mir.selected_tile.y);
        tile_draw(&mir.tiles[mir.selected_tile.x * mir.size + mir.selected_tile.y]);
    }

    tile_draw_hovered_l(mir.hovered_tile.x, mir.hovered_tile.y);
    tile_draw_begin();
    tile_draw_prepare(mir.hovered_tile.x, mir.hovered_tile.y);
    tile_draw(&mir.tiles[mir.hovered_tile.x * mir.size + mir.hovered_tile.y]);

    tile_draw_end();
}

#define p glp_texture_map
void mir_draw_selected_tile()
{
    matrix_identity(_tile_matrix);
    matrix_translate(_tile_matrix,
                     mir.selected_tile.x,
                     mir.selected_tile.y,
                     0.0f);
    matrix_multiply(_world_matrix, _map_matrix, _tile_matrix);
    glUniformMatrix4fv(p.model, 1, GL_FALSE, _world_matrix);

//    if(glfwGetKey(kernel.window, GLFW_KEY_I) )
//        glUniform3i(p.texture_position, x, y, -1);
//    else

    glUniform2i(p.texture_position, 56, 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
#undef p

//void mir_draw_tile(int i, int j)
//{
//    begin_draw_tiles_ex();
//    mir_draw_tile_loop(i, j);
//    end_draw_tiles_ex();
//}

//
//void mir_draw_tile_loop(int i, int j)
//{
//    #define p glp_base
//    tile_t* t = &mir.tiles[ i * mir.size + j ];
//
//    int x0 = t->field.x;
//    int y0 = t->field.y;
//
//    float tmp[16];
//    matrix_identity(tmp);
//    matrix_scale(tmp, 2.0f, 2.0f, 0.0f);
//    matrix_translate(tmp, -1.0f, -1.0f, 0.0f);
//
//    glBindVertexArray(vao_base);
//
//    glUniformMatrix4fv(glp_base.model, 1, GL_FALSE, tmp);
//    glUniformMatrix4fv(glp_base.projection, 1, GL_FALSE, camera_get_identity());
//    glUniformMatrix4fv(glp_base.view, 1, GL_FALSE, camera_get_identity());
//    glUniform2f(glp_base.texture_position, (float)x0 / 8.0f, (float)y0 / 8.0f);
//    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
//
////    if(tile->building.x > 0)
////    {
////        glUniform2f(glp_base.texture_position, (float)tile->building.x / 8.0f, (float)tile->building.y / 8.0f);
////        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
////    }
//    if(t->landscape.x > 0)
//    {
//        glUniform2f(glp_base.texture_position, (float)t->landscape.x / 8.0f, (float)t->landscape.y / 8.0f);
//        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
//    }
////    glUniform2f(glp_base.texture_position, 0.0f, 0.0f);
//
//#undef p
//}
//



//void mir_draw_tile_xy(int x, int y, tile_t* tile)
//{
//    int x0 = tile->field.x; int x1 = tile->field.x + 1;
//    int y0 = tile->field.y; int y1 = tile->field.y + 1;
//
//    float tmp[16];
//    matrix_identity(tmp);
//    matrix_scale(tmp, 64.0f, 64.0f, 0.0f);
//    matrix_translate(tmp, x * 64.0f, y * 64.0f, 0.0f);
//
//
//    glUniformMatrix4fv(gl_program.model, 1, GL_FALSE, tmp);
//    glUniformMatrix4fv(gl_program.projection, 1, GL_FALSE, camera_get_projection());
//    glUniformMatrix4fv(gl_program.view, 1, GL_FALSE, camera_get_view());
//
//    float t[] =
//    {
//        (float)x0 / 8.0f, (float)y1 / 8.0f,
//        (float)x1 / 8.0f, (float)y1 / 8.0f,
//        (float)x1 / 8.0f, (float)y0 / 8.0f,
//        (float)x0 / 8.0f, (float)y0 / 8.0f,
//    };
//    glBindBuffer(GL_ARRAY_BUFFER, vbo);
//    glBufferSubData(GL_ARRAY_BUFFER, size_vert, sizeof(t), t);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//    glBindVertexArray(vao);
//    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
//}
//
//void mir_draw_tile_ex(int x, int y, tile_t* tile)
//{
//#define p glp_base
//    int x0 = tile->field.x;
//    int y0 = tile->field.y;
//
//    float tmp[16];
//    matrix_identity(tmp);
//    matrix_scale(tmp, 64.0f, 64.0f, 0.0f);
//    matrix_translate(tmp, (x - mir.size / 2) * 64.0f, (y - mir.size / 2) * 64.0f, 0.0f);
//
//    glBindVertexArray(vao_base);
//
//    glUniformMatrix4fv(glp_base.model, 1, GL_FALSE, tmp);
//    glUniformMatrix4fv(glp_base.projection, 1, GL_FALSE, camera_get_projection());
//    glUniformMatrix4fv(glp_base.view, 1, GL_FALSE, camera_get_view());
//    glUniform2f(glp_base.texture_position, (float)x0 / 8.0f, (float)y0 / 8.0f);
//    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
//    if(tile->building.x > 0)
//    {
//        glUniform2f(glp_base.texture_position, (float)tile->building.x / 8.0f, (float)tile->building.y / 8.0f);
//        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
//    }
//    if(tile->landscape.x > 0)
//    {
//        glUniform2f(glp_base.texture_position, (float)tile->landscape.x / 8.0f, (float)tile->landscape.y / 8.0f);
//        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
//    }
////    glUniform2f(glp_base.texture_position, 0.0f, 0.0f);
//
//}
//#undef p

#define p glp_texture_map
void mir_draw_tile_tm(int x, int y, tile_t* tile)
{
    matrix_identity(_tile_matrix);
    matrix_translate(_tile_matrix, x, y, 0.0f);
    matrix_multiply(_world_matrix, _map_matrix, _tile_matrix);
    glUniformMatrix4fv(p.model, 1, GL_FALSE, _world_matrix);

//    if(glfwGetKey(kernel.window, GLFW_KEY_I) )
//    glUniform3i(p.texture_position, -1, x, y);
//    else
    glUniform2i(p.texture_position, tile->field_id, 0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    if(tile->landscape_id != -1 )
    {
        glUniform3i(p.texture_position, tile->landscape_id, 0, 0);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
    if(tile->unit_id != -1)
    {
        glUniform3i(p.texture_position, tile->unit_id, 0, 0);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
}
#undef p


#define p glp_texture_map_s
void mir_draw_tile_tm_s(int x, int y, tile_t* tile)
{
    matrix_identity(_tile_matrix);
    matrix_translate(_tile_matrix, x, y, 0.0f);
    matrix_multiply(_world_matrix, _map_matrix, _tile_matrix);

    glUniformMatrix4fv(p.model, 1, GL_FALSE, _world_matrix);

//    if(glfwGetKey(kernel.window, GLFW_KEY_I) )
    glUniform2i(p.texture_position, x, y);
//    else
//    glUniform2i(p.texture_position, tile->field_id, 0, 0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
//    if(tile->landscape_id != -1 )
//    {
//        glUniform3i(p.texture_position, tile->landscape_id, 0, 0);
//        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
//    }
//    if(tile->unit_id != -1)
//    {
//        glUniform3i(p.texture_position, tile->unit_id, 0, 0);
//        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
//    }
}
#undef p

bool mir_map_is_xy_on_map(int x, int y)
{
    return ( (0 <= x) && (x < mir.size) ) && ( (0 <= y) && (y < mir.size) );
}

void mir_map_handle_hovered(int x, int y)
{
//    int x_pos = roundf(x * 8.0f);
//    int y_pos = roundf(y * 8.0f);
//    print_2i(x, y);

    if(mir_map_is_xy_on_map(x, y))
    {
        mir_map_set_hovered_tile(x, y);
//        tile_print_info(mir_map_get_hovered_tile(NULL, NULL));
    }
}


void mir_map_handle_hovered_f(float x, float y)
{
    int x_pos = roundf(x * 8.0f);
    int y_pos = roundf(y * 8.0f);

    if(mir_map_is_xy_on_map(x_pos, y_pos))
    {
//        here;
        mir_map_set_selected_tile_xy(x_pos, y_pos);
    }
}
