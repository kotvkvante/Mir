#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

void players_handle_events(void);
void player_handle_events(int id);
void player_process_input();

typedef struct player_t
{
    int id;
    colorf_rgb_t color;
} player_t;




#endif // PLAYER_H_INCLUDED
