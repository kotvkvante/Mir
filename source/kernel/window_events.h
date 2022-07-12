#ifndef WINDOW_EVENTS_H_INCLUDED
#define WINDOW_EVENTS_H_INCLUDED


void handle_events();

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void character_callback(GLFWwindow* window, unsigned int codepoint);

typedef struct mouse_t
{
    float x, y;
} mouse_t;


#endif // WINDOW_EVENTS_H_INCLUDED
