#ifndef TEAM_H_INCLUDED
#define TEAM_H_INCLUDED

typedef struct unit_t unit_t;

typedef struct team_t
{
    unit_t** units;
    int is_navigation_reseached;
    int is_rock_climbing_reseached;
} team_t;


void init_teams();
void team_research_navigation(int team);

unit_t* team_rand_unit(int team);

#endif // TEAM_H_INCLUDED
