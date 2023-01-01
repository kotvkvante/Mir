#ifndef KERNEL_T_H_INCLUDED
#define KERNEL_T_H_INCLUDED

typedef struct
{
	GLFWwindow* window;
	point2f_t window_size;
	const char* window_title;

	int is_running;
    int state;
    int hovered;
    int selected;
	// render state

    double timer_start, timer_end;
    double delta;

    render_frame_f render_frame;
} kernel_t;


#endif // KERNEL_T_H_INCLUDED
