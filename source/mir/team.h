#ifndef TEAM_H_INCLUDED
#define TEAM_H_INCLUDED

typedef void (*process_input_t)(int);
typedef struct unit_t unit_t;



typedef struct team_t
{
    unit_t** units;
    int units_count;

    process_input_t process_input;
    int is_navigation_reseached;
    int is_rock_climbing_reseached;
} team_t;


void init_teams();
void team_research_navigation(int team);

unit_t* team_rand_unit(int team);
//unit_t* team_next_unit(int team);
void team_unit_iterate(int team);
unit_t* team_next_unit();

#endif // TEAM_H_INCLUDED
