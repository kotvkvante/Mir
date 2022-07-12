#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED


void camera_init();
float* camera_get_projection();
float* camera_get_view();
float* camera_get_identity();
float* camera_get_projection64();
float* camera_get_map_projection();

void camera_set_projection_wh(float* matrix, float width, float height);
void camera_set_view_xy(float x, float y);
void camera_set_map_projection(int size);


void camera_adjust_xy(float x, float y);
void camera_reset_view();


#endif // CAMERA_H_INCLUDED
