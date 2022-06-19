/* triangle.c */

#include <curses.h>
#include <stdlib.h>

int main(void)
{
  initscr();                 /* Start curses mode 		  */
  start_color();
  printw("Hello World !!!"); /* Print Hello World		  */
  printf("")
  refresh();                 /* Print it on to the real screen */
  getch();                   /* Wait for user input */
  endwin();                  /* End curses mode		  */

  return 0;
}