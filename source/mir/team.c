#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../math/point.h"

#include "mir.h"
#include "team.h"



team_t team[TEAM_COUNT];

void init_teams()
{
    for(int i = 0; i < TEAM_COUNT; i++)
    {
        team[i].is_navigation_reseached    = false;
        team[i].is_rock_climbing_reseached = false;
    }

    team[TEAM_BLUE].is_navigation_reseached   = true;
    team[TEAM_RED].is_rock_climbing_reseached = true;
}
