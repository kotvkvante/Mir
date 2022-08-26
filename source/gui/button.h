#ifndef BUTTON_INCLUDED
#define BUTTON_INCLUDED

#include <stdint.h>

typedef struct point2i_t point2i_t;
typedef struct point3uc_t point3uc_t;
typedef void (*button_event_t)(void);

typedef struct button_t
{
    char id;
    wtext_t text;
    point2i_t position;
    point3uc_t background_color;
    point3uc_t text_color;
    int8_t flags;

    button_event_t event;
} button_t;


void buttons_init();
void buttons_draw();
void buttons_draw_pickmap();
void buttons_handle_hovered(int id);
void buttons_unhover();

void buttons_handle_mouse_button(int mouse_btn, int action);
void buttons_handle_mouse_release();
void buttons_handle_mouse_press();


void button_init(button_t* button, wchar_t* str, point2i_t pos, point3uc_t bc, point3uc_t tc);
void button_draw(button_t* button);
void button_draw_pickmap(button_t* btn);

void button_bind_event(button_t* button, button_event_t event);
void button_enable(button_t* button);
void button_disable(button_t* button);
void button_show(button_t* button);
void button_hide(button_t* button);

void button_rem_flag(button_t* btn, int f);
void button_set_flag(button_t* btn, int f);
//void button_();

#endif // BUTTON_INCLUDED
