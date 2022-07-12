#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../math/point.h"
#include "../graphics/graphics.h"
#include "../utils/utils.h"
#include "../graphics/font.h"
#include "../mir/mir.h"
#include "../kernel/kernel.h"

#include "button.h"
//#include "label.h"

#define MAX_BUTTONS 10

static button_t** _buttons;

static int _hovered_button;
static int _pressed_button;
static int _count = 0;

typedef enum {
    VISIBLE  = 1 << 0,
    ACTIVE   = 1 << 1,
    HOVERED  = 1 << 2,
    PRESSED  = 1 << 3,
    TEXTURED = 1 << 4,
    PRESSED_AND_HOVERED = PRESSED | HOVERED,
} button_flags;

button_t button_start_game;
button_t button_leave;
button_t button_info;

void hello_world(void) { printf("Hello world!\n"); }


void buttons_init()
{
    _pressed_button = -1;
    _hovered_button = -1;
    _buttons = malloc(sizeof(button_t*) * MAX_BUTTONS);



    button_init(&button_start_game,
                L"Mir: Начать игру!\n\nStart Game",
                (point2i_t){50, 500}, (point3uc_t){60, 120, 180}, (point3uc_t){20, 40, 80});
    button_bind_event(&button_start_game, hello_world);

    button_init(&button_leave,
                L"Mir: Leave",
                (point2i_t){50, 400}, (point3uc_t){60, 120, 180}, (point3uc_t){20, 40, 80});
    button_bind_event(&button_leave, kernel_stop);

    button_init(&button_info,
                L"Mir: Info",
                (point2i_t){50, 300}, (point3uc_t){240, 120, 240}, (point3uc_t){20, 40, 80});
    button_bind_event(&button_info, mir_print_map);
}

void buttons_draw()
{
    for(int i = 0; i < _count; i++)
        button_draw(_buttons[i]);
//    button_draw(&button_start_game);
//    button_draw(&button_leave);
}




void buttons_draw_pickmap()
{
    for(int i = 0; i < _count; i++)
        button_draw_pickmap(_buttons[i]);
//    button_draw_pickmap(&button_leave);
//    button_draw_pickmap(&button_start_game);

}
void button_draw(button_t* button)
{
    if(! (button->flags & VISIBLE) )
        return;

    point2i_t tmp = button->position;
    tmp.y += -button->text.rect.y + font_get_height();

    if ( (button->flags & PRESSED) && (button->flags & HOVERED) ) draw_rectangle_xy_wh_rgb(tmp, button->text.rect, (point3uc_t){220, 24, 146});
    else if(button->flags & HOVERED) draw_rectangle_xy_wh_rgb(tmp, button->text.rect, (point3uc_t){100, 100, 180});
    else draw_rectangle_2p2i(button->text.rect , tmp);


    draw_wtext_xyrgb(&button->text,
                     button->position.x, button->position.y,
                     button->text_color.x / 255, button->text_color.y / 255, button->text_color.z / 255);

//    draw_button(button);
}

void button_draw_pickmap(button_t* btn)
{
    if(! (btn->flags & VISIBLE) )
        return;

    draw_button_pickmap(btn);
}



void buttons_unhover()
{
    if(_hovered_button != -1)
        button_rem_flag(_buttons[_hovered_button], HOVERED);
    _hovered_button = -1;
}

// id = 0, 1, ...
void buttons_handle_hovered(int id)
{
    if(id != _hovered_button) buttons_unhover();
//    if(_pressed_button != id) return; // some button logic...
    if(_hovered_button == id) return;
    if(_hovered_button != -1) button_rem_flag(_buttons[_hovered_button], HOVERED);

    button_t* btn = _buttons[id];
    button_set_flag(btn, HOVERED);

    _hovered_button = id;
}


void buttons_handle_mouse_button(int mouse_btn, int action)
{
    if(mouse_btn == GLFW_MOUSE_BUTTON_LEFT)
    {
        if(action == GLFW_PRESS)
        {
            buttons_handle_mouse_press();
        }
        else
        {
            buttons_handle_mouse_release();
        }
    }

}



void buttons_handle_mouse_press()
{
    if(_hovered_button != -1)
    {
        _pressed_button = _hovered_button;
        button_set_flag(_buttons[_pressed_button], PRESSED);
    }
}


void buttons_handle_mouse_release()
{
    if(_pressed_button == -1) return;

    if(_pressed_button == _hovered_button)
    {
        _buttons[_pressed_button ]->event();
    }

    button_rem_flag(_buttons[_pressed_button], PRESSED);
    _pressed_button = -1;
}


void buttons_add_button()
{

}

void button_init(button_t* button, wchar_t* str, point2i_t pos, point3uc_t bc, point3uc_t tc)
{

    button->id               = _count; _buttons[_count] = button; _count++;
    button->background_color = bc;
    button->text_color       = tc;
    button->position         = pos;


    button_enable(button);
    button_show(button);
//    button->flags |= TEXTURED;

    wtext_init(&button->text, str);
}

void button_bind_event(button_t* button, button_event_t event)
{
    button->event = event;
}

void button_enable(button_t* button)
{
    button->flags |= ACTIVE;
}

void button_disable(button_t* button)
{
    button->flags &= ~ACTIVE;
}

void button_show(button_t* button)
{
    button->flags |= VISIBLE;
}

void button_hide(button_t* button)
{
    button->flags &= ~VISIBLE;
}

void button_set_flag(button_t* btn, int f)
{
    btn->flags |= f;
}

void button_rem_flag(button_t* btn, int f)
{
    btn->flags &= ~f;
}
