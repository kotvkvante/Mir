#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

typedef enum { STT_MENU, STT_SETTINGS, STT_PREPARE_GAME, STT_RESUME_GAME, STT_GAME, STATES_COUNT } states_t;

render_frame_f stt_get_render_frame(int state);

void stt_init_states();

typedef struct state_t
{
    render_frame_f render_frame;
} state_t;

#endif // STATE_H_INCLUDED
