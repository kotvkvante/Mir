#ifndef AI_H_INCLUDED
#define AI_H_INCLUDED

#include "../math/point.h"

typedef struct ai_t
{
    int id;
    int team;
    colorf_rgb_t color;
} ai_t;

void bots_init();
void bots_process_input();


void bot_init(ai_t* bot);
void bot_handle_events();

void bot_process_input(int id);
#endif // AI_H_INCLUDED
