#ifndef LABEL_H_INCLUDED
#define LABEL_H_INCLUDED

typedef struct label_t
{
    int id;
    wtext_t text;
    point2i_t position;
    point3uc_t background_color;
    point3uc_t text_color;

    int flags;
} label_t;

void label_init();
void label_set_text(label_t* label, const wchar_t* format, const wchar_t* str);
void label_set_text_s(label_t* label, const wchar_t* format, const wchar_t* str);
void label_set_text_d(label_t* label, const wchar_t* format, int d);
//#define label_set_text_va(label, format, ...) wtext_set_text_va(&label->text, format, __VA_ARGS__);


void labels_init();
void labels_update();
void labels_handle_hovered();
void labels_draw();

#endif // LABEL_H_INCLUDED
