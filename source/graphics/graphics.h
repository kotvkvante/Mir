#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#define NULL_INDEX 0
#define MAP_INDEX 1
#define GUI_INDEX 2

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../math/point.h"

//#ifdef
typedef struct text_t text_t;
typedef struct wtext_t wtext_t;
typedef struct label_t label_t;
typedef struct button_t button_t;

typedef struct tile_t tile_t;

void graphics_init();

void opengl_init();

void render_frame();
void render_frame_to_texture();

void graphics_check_error();
void graphics_reload_program();


unsigned char gfx_get_pickmap_index(int x, int y, int* p0, int* p1);
void graphics_get_pickmap_index(int x, int y);
void gfx_handle_gui_hovered();
void gfx_handle_map_hovered();


void draw_square();

//void label_draw(label_t* label);
void draw_label(label_t* label);
void draw_button(button_t* button);
void draw_button_pickmap(button_t* button);


void draw_rectangle_l(float x, float y, float w, float h);
void draw_rectangle_2p2i(point2i_t wh, point2i_t xy);
void draw_rectangle_xy_wh_rgb(point2i_t xy, point2i_t wh, point3uc_t rgb);
void draw_wtext_xy_rgb(wtext_t* text_s, float xpos, float ypos, float r, float g, float b);

void draw_point(float x, float y);

#ifdef MIR_RENDER
void draw_texture(GLuint texture);
void draw_texture_part(GLuint texture);
void draw_rectangle(float x, float y, float w, float h);
void draw_rectangle_t(float x, float y, float w, float h, GLuint texutre);

void draw_text_xy(text_t* text, GLint font_map, float xpos, float ypos);
void draw_text_xyrgb(text_t* text_s, GLint font_map, float xpos, float ypos, float r, float g, float b);
void draw_wtext_xyrgb_t(wtext_t* text_s, GLint font_map, float xpos, float ypos, float r, float g, float b);


void draw_texture_char(GLuint texture, unsigned char ch);
void draw_texture_text(char* text, int length, GLint texture);
void draw_texture_text_xy(char* text, int length, GLint texture, float x_start, float y_start);
void draw_tile(GLuint tile_map, int x0, int y0);
void draw_tile_ex_xy(int x, int y);
void draw_tile_ex(tile_t* tile);
void _draw_tile_ex(tile_t* tile);
void _draw_active_ex(int texture);


void draw_tile_to_texture();
void draw_map_to_texture();
void draw_gui_to_texture();


void draw_screen_to_texture();



GLuint LoadFile(char* FileName, const GLchar** t);
GLuint LoadShader(char *FileName, GLuint type);
int LoadProgram(GLuint* ID, char* frag, char* vert);


typedef struct
{
    GLuint id;
    GLint vertex_position;
    GLint texture_coordinates;
    GLint projection;
    GLint view;
    GLint model;
} gl_program_t;

typedef struct
{
    GLuint id;
    GLint vertex_position;
    GLint texture_coordinates;
    GLint projection;
    GLint view;
    GLint model;
} gl_program_tm_t;

typedef struct
{
    GLuint id;
    GLint vertex_position;
    GLint texture_coordinates;
    GLint texture_position;

    GLint solid_color;
    GLint projection;
    GLint view;
    GLint model;
} glp_base_t;

typedef struct glp_texture_map_t
{
    GLuint id;
    GLint vertex_position;  // attribute
    GLint texture_position; // uniform
                            //      [texture map position, map position x, map position y]
                            //      if (texture map == -1) => solid color

    GLint projection;       // uniform
    GLint view;             // uniform
    GLint model;            // uniform
} glp_texture_map_t;

typedef struct glp_texture_map_s_t
{
    GLuint id;
    GLint vertex_position;  // attribute
    GLint texture_position; // uniform
                            //      [texture map position, map position x, map position y]
                            //      if (texture map == -1) => solid color

    GLint projection;       // uniform
    GLint view;             // uniform
    GLint model;            // uniform
} glp_texture_map_s_t;


typedef struct glp_texture_map_ex_t
{
    GLuint id;
    GLint vertex_position;  // attr
    GLint field; // unif
    GLint unit;
        GLint unit_healt;
    GLint landscape;
    GLint building;

    GLint projection;       // unif
    GLint view;             // unif
    GLint model;            // unif
} glp_texture_map_ex_t;

typedef struct
{
    GLuint id;
    GLint vertex_position;
    GLint texture_coordinates;
    GLint texture_position;

    GLint is_solid_color;
    GLint color;
    GLint projection;
    GLint view;
    GLint model;
} glp_text_t;

typedef struct glp_9slices_t
{
    GLuint id;
    GLint vertex_position;
    GLint vertex_color;
//    GLint texture_position;

//    GLint is_solid_color;
//    GLint color;
    GLint projection;
    GLint view;
    GLint model;
} glp_9slices_t;

typedef struct glp_3slices_t
{
    GLuint id;
    GLint vertex_position;
    GLint vertex_color;
//    GLint texture_position;

//    GLint is_solid_color;
//    GLint color;
    GLint u_rect_width;
    GLint projection;
    GLint view;
    GLint model;
} glp_3slices_t;

typedef struct glp_point_t
{
    GLuint id;
    GLint vertex_position;

    GLint projection;
    GLint view;
    GLint model;
    GLint color;
} glp_point_t;


#endif // MIR_RENDER


#endif // GRAPHICS_H_INCLUDED
