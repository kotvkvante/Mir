#include <stdlib.h>
#include <stdio.h>

#include "../math/point.h"
#include "camera.h"
#include "graphics.h"
#include "../math/matrix.h"
#include "../kernel/kernel.h"
#include "../utils/utils.h"

typedef struct
{
    float projection[16];
    float identity[16];
    float view[16];
    float map_projection[16];

    float scale;

    point2f_t position;
    point2f_t default_position;

} camera_t;

camera_t camera;
void camera_update_projection(float k);



void camera_init()
{
    point2f_t scr = kernel_get_window_size();

    camera.position = camera.default_position = scr;
    camera.scale = 1.0f;

    matrix_identity(camera.identity);
    matrix_identity(camera.projection);
//    matrix_identity(camera.view);
    matrix_identity(camera.map_projection);


    camera_set_projection_wh(camera.projection, scr.x, scr.y);
    // camera_set_projection_wh(camera.map_projection, DEFAULT_MAP_SIZE*64.0f, DEFAULT_MAP_SIZE*64.0f);
    // dont forget update map_projection

    camera_set_view_xy(scr.x / 2 + 200, scr.y / 2);
}

void camera_reset_view()
{
    camera_set_view_xy(camera.default_position.x / 2, camera.default_position.y / 2);

}


void camera_set_map_projection(int size)
{
    camera_set_projection_wh(camera.map_projection, size*64.0f, size*64.0f);

}

void camera_set_view_xy(float x, float y)
{
    matrix_identity(camera.view);
    matrix_translate(camera.view, x, y, 0.0f);
}

void camera_adjust_xy(float x, float y)
{
    if((x == 0) && (y == 0))
    {
        return;
    }
    print_f(x);
    matrix_translate(camera.view, x, y, 0.0f);
}


void camera_update_projection(float k)
{
    matrix_ortho(camera.projection,
                -1 * camera.scale, 1 * camera.scale,
                -k * camera.scale, k * camera.scale,
                -25.0f, 25.0f
    );
}

void camera_set_projection_wh(float* matrix, float width, float height)
{
//    printf("%f %f\n", width, height);
    matrix_ortho(matrix, 0, height, 0, width, 1.0f, -1.0f);
}


float* camera_get_projection()
{
    return camera.projection;
}

float* camera_get_view()
{
    return camera.view;
}

float* camera_get_identity()
{
    return camera.identity;
}

float* camera_get_map_projection()
{
    return camera.map_projection;
}


