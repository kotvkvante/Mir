#ifndef LABEL_H_INCLUDED
#define LABEL_H_INCLUDED

typedef struct label_t
{
    int id;
    wtext_t text;
    point2i_t position;
    color_uc_rgb_t background_color;
    color_uc_rgb_t text_color;

    int flags;
} label_t;

void label_init();
void label_set_text(label_t* label, const wchar_t* format, const wchar_t* str);
void label_set_text_s(label_t* label, const wchar_t* format, const wchar_t* str);
void label_set_text_d(label_t* label, const wchar_t* format, int d);
//#define label_set_text_va(label, format, ...) wtext_set_text_va(&label->text, format, __VA_ARGS__);
void label_set_text_color_uc_rgb(label_t* label, color_uc_rgb_t color);
void label_set_bg_color_uc_rgb(label_t* label, color_uc_rgb_t color);


void labels_init();
void labels_update();
void labels_handle_hovered();
void labels_draw();
void labels_draw_static();
void labels_game_prepare_draw();


#endif // LABEL_H_INCLUDED
