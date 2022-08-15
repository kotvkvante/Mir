#ifndef MIR_QUEUE_H_INCLUDED
#define MIR_QUEUE_H_INCLUDED

#include <stdbool.h>

#include "mir.h"

#define MQUEUE_MAX_SIZE 64

typedef struct mir_event_t mir_event_t;
typedef union event_arg_t event_arg_t;

typedef void (*event_f)(event_arg_t*);

typedef struct
{
    int front;
    int rear;
    int* events;
} queue_t;


typedef union event_arg_t
{
        struct {
            int sx, sy, ex, ey;
        } move_unit;

        struct {
            int x, y, t;
        } build_smf;

        struct {
            int x, y, t;
        } change_landscape;

        struct {
            int x, y, t;
        } change_field;
} event_arg_t;

typedef struct event_t
{
    event_arg_t arg;
    event_f f;
} event_t;


typedef struct mir_event_t
{
    int type;

    union {
        struct {
            int sx, sy, ex, ey;
        } move_unit;

        struct {
            int x, y, t;
        } build_smf;

        struct {
            int x, y, t;
        } change_landscape;

        struct {
            int x, y, t;
        } change_field;
    };
} mir_event_t;


typedef struct
{
    int front;
    int rear;
    mir_event_t* events;
    event_t* _events;
} mir_queue_t;

//bool mqueue_event_enqueue(mir_t* mir, int event);
//bool mqueue_event_dequeue(mir_t* mir, int* event);

void mqueue_init();
void mqueue_reinit();


bool mqueue_event_enqueue(mir_event_t event);
bool mqueue_event_dequeue(mir_event_t* event);

bool mqueue_fevent_enqueue(event_t event);
bool mqueue_fevent_dequeue(event_t *event);

#endif // MIR_QUEUE_H_INCLUDED
