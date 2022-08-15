#include <stdlib.h>
#include <stdio.h>

#include "../math/point.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "player.h"

static int _count = 1;

void players_handle_events(void)
{
    for(int i = 0; i < _count; i++)
    {
//        player_handle_events(i);
    }
}

//void player_handle_events(int id)
//{
//    glfwPollEvents();
//}

void player_process_input()
{
    glfwPollEvents();
}
