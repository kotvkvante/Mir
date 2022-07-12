#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

void font_init_atlas();
void font_draw_char();

void font_get_char();
int font_get_height();

#define FONT_Y_OFFSET 4
#define ATLAS_TEXTURE_SIZE 1024.0f
#define NUM_GLYPHS 2048

typedef struct
{
    int height;
    int y_offset;

} font_t;

typedef struct {
	int x0, y0, x1, y1;	// coords of glyph in the texture atlas
	int x_off, y_off;   // left & top bearing when rendering
	int advance;        // x advance when rendering
} glyph_info_t;

typedef struct text_t
{
    char* text;
    int length;
    point2i_t rect;
    int background;
    int fl_h; // first line height in pixels
} text_t;

typedef struct wtext_t
{
    wchar_t*  text;
    int     length;
    point2i_t rect;
    int background;
//    int fl_h; // first line height in pixels
} wtext_t;

void texts_init();
void text_init(text_t* text, char* str);
void wtext_init(wtext_t* text, wchar_t* str);
void wtext_set_text_ex(wtext_t* text, const wchar_t* format, const wchar_t* str);
void wtext_set_text_ex_d(wtext_t* text, const wchar_t* format, int d);
void wtext_set_text_va(wtext_t* text, const wchar_t *format, ...);
//void wtext_push_char(wtext_t* text, unsigned char c);
void wtext_push_char(wtext_t* text, unsigned int c);
void wtext_set_text(wtext_t* text, const wchar_t* str);


void text_push_char(text_t* text, unsigned char c);
void text_push_new_line(text_t* text);
void text_pop_char(text_t* text);

#endif // FONT_H_INCLUDED
