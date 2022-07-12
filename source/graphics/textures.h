#ifndef TEXTURES_H_INCLUDED
#define TEXTURES_H_INCLUDED


void textures_init();
GLuint texture_load(char* file_name);
GLuint texture_from_bitmap(unsigned char* data, int width, int height, int channels);
void texture_load_char(unsigned char* data, int width, int height, int channels);
void texture_load_font_map(int width, int height, unsigned char* data);

void texture_render_begin(int width, int height);
void texture_render_end();

int  texture_named_render_begin(GLuint* named_texture, int width, int height);
void texture_named_render_end();
void texture_named_save();


GLuint texture_get_tile_map();
GLuint texture_get_font_map();

void texture_save_to_png(GLuint texture);


typedef struct textures_t
{
    GLuint tile_map;
    GLuint font_map;

    GLuint mir_pick_map;

} textures_t;

typedef struct texture_9slices_t
{

} texture_9slices_t;

typedef struct texture_3slices_t
{

} texture_3slices_t;


#endif // TEXTURES_H_INCLUDED
