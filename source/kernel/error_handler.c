#include <stdlib.h>
#include <stdio.h>

#include "error_handler.h"


char* __msg_codes[] =
{
	[DEFAULT_C]  = "default",
	[INIT_C]     = "init",
	[GRAPHICS_C] = "graphics",
	[GAME_C]     = "game",
};


void _msg(int code)
{
	printf("[%s]", __msg_codes[code]);
}

void log_msg(int code, char* msg)
{
	printf("[%s] %s\n", __msg_codes[code], msg);
}

void log_msg_s(int code, char* msg, char* str)
{
    printf("[%s] ", __msg_codes[code]);
    printf(msg, str);
    printf("\n");
}


void error_msg(int code, char* msg)
{
	printf("[%s] %s\n", __msg_codes[code], msg);
	exit(code);
}

void error_msg_s(int code, char* msg, char* str)
{
    printf("[%s] ", __msg_codes[code]);
    printf(msg, str);
    printf("\n");
	exit(code);
}
