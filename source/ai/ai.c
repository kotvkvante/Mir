#include <stdlib.h>
#include <stdio.h>


#include "../utils/utils.h"
#include "../kernel/error_handler.h"
#include "../mir/mir.h"
#include "../mir/mir_queue.h"
#include "../mir/unit.h"
#include "../mir/team.h"

#include "ai.h"


ai_t* bots;
static int bots_count;
static int _id = 0;

void bots_init()
{
    log_msg(DEFAULT_C, "Bots init.");
    bots_count = 1;
    bots = malloc(sizeof(ai_t) * bots_count);
    bot_init(&bots[0]);
//    bot_init(&bots[1]);

    bots[0].team = TEAM_BLUE;
//    bots[1].team = TEAM_BLUE;

}

void bot_init(ai_t* bot)
{
    bot->id = _id; _id++;
    bot->color = random_cfrgb();
}

void bots_process_input()
{
    for(int i = 0; i < bots_count; i++)
    {
//        here;
        bot_process_input(i);

    }

    event_t e = {
        .f = mir_turn_e,
        .arg = {0},
    };

    mqueue_fevent_enqueue(e);
}

void bot_process_input(int id)
{
    unit_t* u = team_rand_unit(bots[id].team);
    if(u == NULL) { error_msg(DEFAULT_C, "Null unit."); return;}
    int x = u->x, y = u->y;

//    print_2i(x, y);

    event_t e = {
        .f = unit_move_e_random,
        .arg.move_unit = {.sx = x, .sy = y},
    };

//    here;
    mqueue_fevent_enqueue(e);
}

void bot_handle_events()
{
    return;
}



