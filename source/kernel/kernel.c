#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>

#include "../math/point.h"
#include "../graphics/graphics.h"
#include "../player/player.h"


#include "error_handler.h"
#include "../math/point.h"

#include "../graphics/font.h"
#include "../graphics/camera.h"
#include "../graphics/textures.h"
#include "../mir/mir.h"
#include "../gui/gui.h"
#include "../mir/unit.h"
#include "../mir/team.h"
#include "../ai/ai.h"


#include "window_events.h"

#include "../utils/utils.h"


#include "kernel.h"
#include "kernel_t.h"

kernel_t kernel;

extern player_t* players;
extern ai_t* bots;
extern mouse_t mouse;
extern point2f_t _map_pos;


void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}


void kernel_init()
{
    opengl_init();
    graphics_init();
    camera_init();
    textures_init();
    font_init_atlas();
    texts_init();
    mir_init();
    gui_init();

//    units_init(); // duplicate init
    init_teams();
    bots_init();

	kernel.is_running = true;

	render_frame_to_texture();
}


void kernel_render_frame()
{
    glViewport(0, 0, kernel.window_size.x, kernel.window_size.y);

    render_frame();

    glfwSwapBuffers(kernel.window);
}

void handle_hovered()
{
    int p0, p1;
    switch (gfx_get_pickmap_index((int)mouse.x, (int)mouse.y, &p0, &p1))
    {
        case NULL_INDEX:
            mir_map_unhover();
        break;

        case MAP_INDEX:
            if(!mir_is_started()) break;
            mir_map_handle_hovered(p0, p1);
        break;

        case GUI_INDEX:
            gui_handle_hovered(p0, p1);
            mir_map_unhover();
        break;

        default:
            log_msg_s(DEFAULT_C, "%s: Untracked case.", __func__);
        break;
    }

    return;


}


void kernel_update_state(float dt)
{
    kernel_tick();
    gui_update();

    if(mir_is_started())
    {
        mir_process_events();
    }
}


void kernel_handle_events()
{
    kernel.is_running = !glfwWindowShouldClose(kernel.window);
	handle_events();

    if(mir_is_started())
    {
        mir_handle_input();
//        switch(mir_get_turn())
//        {
//            case TEAM_RED:
//                player_process_input();
//
//                break;
//            case TEAM_BLUE:
//                bots_process_input();
//
//                break;
//            default:
//                error_msg(DEFAULT_C, "Untraced team.");
//
//                break;
//        }

    }
}

void kernel_set_window_size(float width, float height)
{
    kernel.window_size.x = width;
    kernel.window_size.y = height;
}

void kernel_set_window_title(char* title)
{
    kernel.window_title = title;
}


point2f_t kernel_get_window_size()
{
    return kernel.window_size;
}

float kernel_get_window_height()
{
    return kernel.window_size.y;
}

float kernel_get_window_width()
{
    return kernel.window_size.x;
}

static int frames = 0;
void kernel_tick()
{
    kernel.timer_end = glfwGetTime();
    frames++;

    if(frames == FRAME_DELTA)
    {
        kernel.delta = kernel.timer_end - kernel.timer_start;
        frames = 0;

        kernel.timer_start = glfwGetTime();
    }
}

int kernel_get_fps()
{

    return (int)(FRAME_DELTA / kernel.delta);
}


int kernel_is_running()
{
	return kernel.is_running;
}

void kernel_stop()
{
    kernel.is_running = false;
    printf("Bye!\n");
}

void kernel_close()
{
	glfwDestroyWindow(kernel.window);
	glfwTerminate();
}


