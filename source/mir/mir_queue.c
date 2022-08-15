#include <stdlib.h>
#include <stdio.h>

#include "../kernel/error_handler.h"
#include "mir_queue.h"

static mir_queue_t queue;

void mqueue_init()
{
    queue.front = 0;
    queue.rear = 0;
    queue.events = malloc(sizeof(mir_event_t) * MQUEUE_MAX_SIZE);
    queue._events = malloc(sizeof(event_t) * MQUEUE_MAX_SIZE);
}

void mqueue_reinit()
{
    queue.front = 0;
    queue.rear  = 0;
}

//bool mqueue_event_enqueue(int event)
//{
//    if(queue.front == MQUEUE_MAX_SIZE) {error_msg(DEFAULT_C, "Mqueue max size reached!"); return false;}
//
//    queue.events[queue.front] = event;
//    queue.front++;
//    return true;
//}
//
//bool mqueue_event_dequeue(int* event)
//{
//    if(queue.rear == queue.front) { return false; }
//
//    *event = queue.events[queue.rear];
//    queue.rear++;
//
bool mqueue_event_enqueue(mir_event_t event)
{
    if(queue.front == MQUEUE_MAX_SIZE) {error_msg(DEFAULT_C, "Mqueue max size reached!"); return false;}

    queue.events[queue.front] = event;
    queue.front++;
    return true;
}

bool mqueue_event_dequeue(mir_event_t* event)
{
    if(queue.rear == queue.front) { return false; }

    *event = queue.events[queue.rear];
    queue.rear++;
}

bool mqueue_fevent_enqueue(event_t event)
{
    if(queue.front == MQUEUE_MAX_SIZE - 1) {error_msg(DEFAULT_C, "Mqueue max size reached!"); return false;}

    queue._events[queue.front] = event;
    queue.front++;
    return true;
}

bool mqueue_fevent_dequeue(event_t* event)
{
    if(queue.rear == queue.front) { return false; }

    *event = queue._events[queue.rear];
    queue.rear++;
    return true;
}




