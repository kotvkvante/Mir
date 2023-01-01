#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

typedef int ivalue_t;

enum GUI_TYPE { BUTTON_T, LABEL_T};

void gui_init();
void gui_update();

void gui_handle_hovered(int t, int id);

void gui_draw_pickmap();

void increase_value();
void decrease_value();

#endif // GUI_H_INCLUDED
