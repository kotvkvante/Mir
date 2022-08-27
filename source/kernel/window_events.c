#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <stdbool.h>

#include "../graphics/graphics.h"
#include "error_handler.h"
#include "window_events.h"
#include "../math/point.h"
#include "../graphics/camera.h"
#include "../mir/mir.h"
#include "../mir/tile.h"
#include "../mir/unit.h"

#include "../utils/utils.h"


#include "kernel_t.h"
#include "kernel.h"
#include "../graphics/font.h"
#include "../gui/label.h"
#include "../gui/button.h"


#define KEY_PRESS(glfw_key) (key == glfw_key) && (action == GLFW_PRESS)

point2f_t _map_origin = {0};
point2f_t _map_pos    = {0};
mouse_t mouse;

extern kernel_t kernel;
extern text_t hello;
extern wtext_t russia;
extern label_t label_selected_tile;
extern mir_t mir;
extern unit_t test_unit;

void handle_events()
{
    glfwPollEvents();
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    int p0, p1;
    switch(gfx_get_pickmap_index((int)mouse.x, (int)mouse.y, &p0, &p1))
    {
    case 0:
        printf("Nothing =(\n");
        break;
    case MAP_INDEX:
        if(!mir_is_started()) break;
        if(action == GLFW_RELEASE) break;
        mir_map_handle_mouse_button(button, action);
        break;
    case GUI_INDEX:
        buttons_handle_mouse_button(button, action);
        break;
    }

    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) buttons_handle_mouse_release();

//

//    {
//        int p0, p1;
//        if(gfx_get_pickmap_index((int)mouse.x, (int)mouse.y, &p0, &p1) == MAP_INDEX)
//        {
//            tile_t* t;
//            int x, y;
//            if(t = mir_map_get_selected_tile(&x, &y))
//            {
//                if((mir_map_get_distance(x, y, p0, p1) <= 2) && (mir_map_get_selected_tile_unit()))
//                {
//                    int u = mir_map_get_tile_unit(p0, p1);
//                    if ((u == WARRIOR) || (u == ARCHER)) {mir_map_deselect(); return;}
//
//
//                    if(mir_map_get_tile_field(p0, p1) != SEA)
//                    {
//                        tile_move_unit(t, mir_map_get_tile(p0, p1));
//                        mir_map_deselect();
//                        return;
//                    }
//                }
//            }
//
//            if(mir_map_get_tile_unit(p0, p1))
//            {
//                mir_map_add_active(p0, p1);
//            }
//            else
//            {
//                mir.active_tiles_count = 0;
//            }
//
//            mir_map_set_selected_tile(p0, p1);
//        }
//    }

}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    mouse.x = (float)xpos;
    mouse.y = kernel.window_size.y - (float)ypos;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
//    if (key == GLFW_KEY_I && action == GLFW_PRESS)
//    {
//        mir_print_map();
//        point2f_t scr = kernel_get_window_size();
//        int size = mir_map_get_size();
//        _map_origin = (point2f_t){scr.x / 2 - (size >> 1) * 64, scr.y / 2 - (size >> 1) * 64};
//    }

//    point2f_t vec = (point2f_t){ (key == GLFW_KEY_RIGHT) - (key == GLFW_KEY_LEFT),
//                                 (key == GLFW_KEY_UP)    - (key == GLFW_KEY_DOWN) };

//    if(action == GLFW_PRESS)
//    {
//        camera_adjust_xy(vec.x, vec.y);
//    }

    if(KEY_PRESS(GLFW_KEY_1)) team_research_navigation(0);
    if(KEY_PRESS(GLFW_KEY_ENTER)) mir_start();
    if(key == GLFW_KEY_R && action == GLFW_PRESS) camera_reset_view();

//    if(key == GLFW_KEY_ENTER && action == GLFW_PRESS)
//    {
//        text_push_new_line(&hello);
//    }

//    if(key == GLFW_KEY_BACKSPACE && (action == GLFW_PRESS || action == GLFW_REPEAT ))
//    {
//        text_pop_char(&hello);
//    }

//    if(KEY_PRESS(GLFW_KEY_R))
//    {
//        draw_screen_to_texture();
//    }
//    if(KEY_PRESS(GLFW_KEY_T))
//    {
//        draw_tile_to_texture();
//    }

    if(KEY_PRESS(GLFW_KEY_I)) mir_print_map();
//    if(KEY_PRESS(GLFW_KEY_I)) unit_print_info(&test_unit);
//    if(KEY_PRESS(GLFW_KEY_M)) draw_map_to_texture();
//    if(KEY_PRESS(GLFW_KEY_N)) draw_gui_to_texture();
//    if(KEY_PRESS(GLFW_KEY_B)) render_frame_to_texture();
    if(KEY_PRESS(GLFW_KEY_C)) system("clear");
    if(KEY_PRESS(GLFW_KEY_G)) mir_map_gen();

    if(mir_is_started())
    {
        if(KEY_PRESS(GLFW_KEY_W)) mir_turn();
        if(KEY_PRESS(GLFW_KEY_D)) mir_map_deselect();
    }
    if(KEY_PRESS(GLFW_KEY_P)) mir_map_draw_pickmap();
    if(KEY_PRESS(GLFW_KEY_F2)) graphics_reload_program();
    if(KEY_PRESS(GLFW_KEY_ESCAPE)) kernel_stop();



}

const char* get_character_string(int codepoint)
{
    static char result[6 + 1];

    int length = wctomb(result, codepoint);
    if (length == -1)
        length = 0;

    result[length] = '\0';
    return result;
}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
//    text_push_char(&hello, (char)codepoint);
//    wtext_push_char(&russia, codepoint);


//    printf("%c\n", (char)codepoint);

//    putc(codepoint, stdout);


//    printf("Character: 0x%08x (%s)\n", codepoint,  get_character_string(codepoint));
//    char d[6 + 1];
//    wctomb(d, codepoint);
//    printf("Character: 0x%08x (%s)\n", codepoint, d);

//    wchar_t c = (wchar_t)codepoint;

//    putwchar(codepoint);
//    putwchar((wchar_t)codepoint);
//    wprintf(L"%s\n", codepoint);


//    printf("%d %d\n", sizeof(codepoint), codepoint);


//    printf("%s\n", b);

//    printf("%d %d\n", sizeof(wchar_t), c);
//    print_i(MB_CUR_MAX);
    // Note: wctomb is deprecated; consider using wctomb_s


//    wchar_t star = (wchar_t)codepoint;
//    wprintf(L"%lc\n", star);

//    fflush(stdout);

//    wchar_t c = (wchar_t)codepoint;
//    char b = wctob(c);
//    if(b == EOF)
//    {
////        log_msg(DEFAULT_C, "Failed to convert int to wchar byte!");
//        char d[8];
//        int n = wctomb(d, c);
//        printf("%d: %.*s\n",codepoint,n, d);
//        return;
//    } else printf("%d: %c\n", codepoint ,b);



//    font_get_char(codepoint);
}

