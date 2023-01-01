#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "error_handler.h"
#include "../graphics/graphics.h"
#include "kernel.h"
#include "state.h"

state_t states[STATES_COUNT];

void stt_init_states()
{
    states[STT_MENU].render_frame = render_frame_menu;
    states[STT_SETTINGS].render_frame = render_frame;
    states[STT_GAME].render_frame = render_frame_game;
    states[STT_PREPARE_GAME].render_frame = render_frame_prepare_game;
    states[STT_RESUME_GAME].render_frame  = render_frame_game;
}

bool stt_is_valid_state(int state)
{
    return ((STT_MENU <= state) && (state < STATES_COUNT));
}

render_frame_f stt_get_render_frame(int state)
{
    if (!stt_is_valid_state(state)) error_msg(DEFAULT_C, "invalid state");

    return states[state].render_frame;
}
