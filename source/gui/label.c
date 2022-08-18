#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>
#include <stdarg.h>

#include "../math/point.h"
#include "../graphics/graphics.h"
#include "../mir/mir.h"
#include "../mir/tile.h"
#include "../kernel/kernel.h"
#include "../graphics/font.h"
#include "../graphics/texture_map.h"
#include "label.h"

#include "../graphics/font.h"
#include "../utils/utils.h"
#define MAX_LABELS 10
static label_t** _labels;

static int _count = 0;

label_t test;
label_t label_selected_tile;
label_t label_fps = {.id = -1};
label_t label_main;

void labels_init()
{
    _labels = malloc(sizeof(label_t*) * MAX_LABELS);

    label_init(&label_main, L">>> Mir <<<", (point2i_t){50, kernel_get_window_height() - 50}, (point3uc_t){32, 64, 128}, (point3uc_t){128, 64, 32});
//    label_init(&test, L"||TEST|@#$%^&*~/*-.5/,\nAsSS\n\nMy profile: Мой профиль", (point2i_t){50, 100}, (point3uc_t){32, 64, 128}, (point3uc_t){128, 64, 32});
    label_init(&label_selected_tile, L"Selected tile: %s", (point2i_t){50, 150}, (point3uc_t){32, 64, 128}, (point3uc_t){128, 64, 32});
    label_init(&label_fps, L"FPS: %s", (point2i_t){50, 600}, (point3uc_t){16, 255, 128}, (point3uc_t){255, 127, 0});
}


void label_init(label_t* label, wchar_t* str, point2i_t pos, point3uc_t bc, point3uc_t tc)
{
    label->id               = _count; _labels[_count] = label; _count++;
    label->background_color = bc;
    label->text_color       = tc;
    label->position         = pos;

    wtext_init(&(label->text), str);
}

//#define label_set_text_va(label, format, ...) wtext_set_text_va(&label->text, format, __VA_ARGS__);
void label_set_text_va(label_t* label, const wchar_t* format, ...)
{
    va_list args;
    va_start(args, format);
    wtext_set_text_va(&label->text, format, args);
    va_end(args);
}

void label_set_text_s(label_t* label, const wchar_t* format, const wchar_t* str)
{
    wtext_set_text_ex(&label->text, format, str);
}

void label_set_text_d(label_t* label, const wchar_t* format, int d)
{
    wtext_set_text_ex_d(&label->text, format, d);
}

void labels_draw()
{
    for(int i = 0; i < _count; i++)
    {
        draw_label(_labels[i]);
    }
}

void labels_update()
{
    tile_t* t = mir_map_get_selected_tile(NULL, NULL);
    if(t)
    {
        wchar_t buff[256] = {0};
        tile_get_info_wstr(t, buff, 256);
        wtext_set_text(&label_selected_tile.text, buff);
    }
//    else
//        label_set_text_s(&label_selected_tile, L"tile: %ls", L"No tile selected.");

    if(label_fps.id != -1) label_set_text_d(&label_fps, L"FPS: %d", kernel_get_fps() );
}

void labels_handle_hovered()
{

}
