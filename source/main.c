#include <stdio.h>
#include <stdlib.h>

#include <locale.h>

#include "kernel/kernel.h"

int main(int argc, char** argv)
{
    kernel_init();

	while(kernel_is_running())
	{

		kernel_handle_events();
		kernel_update_state(1.0f);
		kernel_render_frame();

//		double end   = glfwGetTime();

	}

  	kernel_close();
	return 0;
}
