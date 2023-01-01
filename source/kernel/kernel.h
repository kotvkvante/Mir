#ifndef KERNEL_H_INCLUDED
#define KERNEL_H_INCLUDED

#define FRAME_DELTA 60

#define impl(code, name) \
void kernel_set_state_##name(void) { kernel_set_state(code); }

#define decl(code, name) \
void kernel_set_state_##name(void);

#define kernel_set_state_funcs(f) \
f(STT_MENU, menu) \
f(STT_GAME, game) \
f(STT_SETTINGS, settings) \
f(STT_PREPARE_GAME, prepare_game)

kernel_set_state_funcs(decl)

typedef void (*render_frame_f)(void);
typedef void (*update_state_f)(void);
typedef void (*handle_events_f)(void);

typedef struct point2f_t point2f_t;

void kernel_init();

void kernel_render_frame();
void kernel_update_state(float dt);
void kernel_handle_events();
void handle_hovered();

void kernel_tick();


int kernel_get_fps();
float kernel_get_window_width();
float kernel_get_window_height();
point2f_t kernel_get_window_size();


void kernel_set_window_size(float width, float height);
void kernel_set_window_title(char* title);


int kernel_is_running();


void kernel_stop();
void kernel_close();




#endif // KERNEL_H_INCLUDED
