#ifndef WINDOW_H
#define WINDOW_H

#include <ncursesw/panel.h>

void InitWindow();

WINDOW *CreateNewWindow(uint16_t lines, uint16_t columns,
                        uint16_t start_x, uint16_t start_y);

PANEL *CreateNewPanel(WINDOW *window);

#endif // WINDOW_H
