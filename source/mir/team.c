#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../kernel/error_handler.h"
#include "../utils/utils.h"
#include "../math/point.h"

#include "mir.h"
#include "team.h"
#include "unit.h"



team_t teams[TEAM_COUNT];

extern unit_t* _units_red[10];
extern unit_t* _units_blue[10];


void init_teams()
{
    log_msg(DEFAULT_C, "Init teams.");
    for(int i = 0; i < TEAM_COUNT; i++)
    {
        teams[i].is_navigation_reseached    = false;
        teams[i].is_rock_climbing_reseached = false;
    }

    teams[TEAM_BLUE].is_navigation_reseached   = true;
    teams[TEAM_RED].is_rock_climbing_reseached = true;

    teams[TEAM_BLUE].units = _units_blue;
    teams[TEAM_RED].units = _units_red;

//    print_i(teams[TEAM_BLUE].units[0]->x);
//    print_i(teams[TEAM_BLUE].units[0]->y);
}

unit_t* team_rand_unit(int team)
{
//    print_i(teams[team].units[0]->x);
//    print_i(teams[team].units[0]->y);
    return teams[team].units[0];
}
