#include "inc/manager_keyboard.h"

#include <ncursesw/ncurses.h>

uint8_t kbhit()
{
  int ch = getch();
  if (ch != ERR)
  {
    ungetch(ch);
    return 1;
  }
  else
  {
    return 0;
  }
}
