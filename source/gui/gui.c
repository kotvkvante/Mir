#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

#include "../kernel/error_handler.h"
#include "../utils/utils.h"
#include "../math/point.h"
#include "../graphics/font.h"
#include "../kernel/kernel.h"
#include "gui.h"
#include "label.h"
#include "button.h"

extern label_t lbl_map_size;
ivalue_t val_map_size = 0;

wchar_t* _map_size_str[] = {L"small", L"medium", L"large"};

void gui_init()
{
    buttons_init();
    labels_init();
//    init_buttons();
}

void gui_draw_pickmap()
{
    buttons_draw_pickmap();
    //
}

void gui_update()
{
    buttons_unhover();


    labels_update();
    handle_hovered();
}

void gui_handle_hovered(int t, int id)
{
    switch(t)
    {
        case BUTTON_T:
            buttons_handle_hovered(id);
        break;

        case LABEL_T:
            labels_handle_hovered(id);
        break;

        default:
            log_msg_s(DEFAULT_C, "%s: Untracked case.", __func__);
        break;
    }
}

void decrease_value()
{
    if(val_map_size == 0) return;
    val_map_size--;

    wtext_set_text(&lbl_map_size.text, _map_size_str[val_map_size]);
}

void increase_value()
{
    if(val_map_size == 2) return;
    val_map_size++;

    wtext_set_text(&lbl_map_size.text, _map_size_str[val_map_size]);
}
