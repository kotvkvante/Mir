#ifndef ERROR_HANDLER_H_INCLUDED
#define ERROR_HANDLER_H_INCLUDED

typedef enum {DEFAULT_C, INIT_C, GRAPHICS_C, GAME_C} __codes;




void _msg(int code);

void error_msg(int code, char* msg);
void error_msg_s(int code, char* msg, char* str);
void log_msg(int code, char* msg);
void log_msg_s(int code, char* msg, char* str);

#define log_msg_va(code, ...) _msg(code); printf(__VA_ARGS__)


#endif // ERROR_HANDLER_H_INCLUDED
