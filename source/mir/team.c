#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../kernel/error_handler.h"
#include "../utils/utils.h"

#include "../math/point.h"
#include "../math/mrand.h"

#include "mir.h"
#include "team.h"
#include "../ai/ai.h"
#include "../player/player.h"
#include "unit.h"

team_t teams[TEAM_COUNT];

extern unit_t* _units_red[10]; extern int red_id;
extern unit_t* _units_blue[10]; extern int blue_id;

static int unit_number;
static int units_count;
static int current_team;

void teams_init()
{
    log_msg(DEFAULT_C, "Init teams.");
    for(int i = 0; i < TEAM_COUNT; i++)
    {
        teams[i].is_navigation_reseached    = false;
        teams[i].is_rock_climbing_reseached = false;
    }

    teams[TEAM_BLUE].process_input = bot_process_input;
    teams[TEAM_RED ].process_input = player_process_input;

    teams[TEAM_BLUE].is_navigation_reseached   = true;
    teams[TEAM_RED].is_rock_climbing_reseached = true;

    teams[TEAM_BLUE].units = _units_blue;
    teams[TEAM_RED].units = _units_red;

    teams[TEAM_BLUE].units_count = blue_id;
    teams[TEAM_RED ].units_count = red_id;

//    print_i(teams[TEAM_BLUE].units[0]->x);
//    print_i(teams[TEAM_BLUE].units[0]->y);
}

void team_research_navigation(int team)
{
    teams[team].is_navigation_reseached = true;
}

void team_unit_iterate(int team)
{
    current_team = team;
    units_count = teams[team].units_count;
    unit_number = 0;
}

unit_t* team_next_unit()
{
    unit_t* u;
    while(unit_number != units_count)
    {
        u = teams[current_team].units[unit_number];
        unit_number++;
//        print_i(unit_number);
        return u;
    }

    return NULL;
}

unit_t* team_rand_unit(int team)
{
//    print_i(teams[team].units[0]->x);
//    print_i(teams[team].units[0]->y);
    if(team == TEAM_BLUE)
    {
        return teams[team].units[rand_index_zero(blue_id - 1 )];
    }
    else if(team == TEAM_RED)
    {
        return teams[team].units[rand_index_zero(red_id - 1)];
    }
}
