#include <stdlib.h>
#include <stdio.h>

#include "../utils/utils.h"
#include "../math/point.h"
#include "../graphics/font.h"
#include "../kernel/kernel.h"
#include "gui.h"
#include "label.h"
#include "button.h"


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
    }
}
