/* triangle.c */

#include <curses.h>
#include <stdlib.h>

int main(void)
{
  FILE *fp;
  char str[80];
  char str2[80];
  char screen[2000];
  int character = 0;

  initscr(); /* Start curses mode 		  */
  noecho();
  keypad(stdscr, TRUE);
  start_color();
  raw();

  bool hasColors = has_colors();
  bool canChangeColor = can_change_color();
  printw("The terminal has color        : %d \n", hasColors);
  printw("The user can modify the color : %d \n", canChangeColor);
  refresh(); /* Print it on to the real screen */
  while (true)
  {
    character = getch(); /* Wait for user input */
    clear();
    sprintf(str, "espeak -v fr-br '%c'", character);
    sprintf(str2, "figlet -c '%c'", character);
    fp = popen(str2, "r");
    while (fgets(screen, sizeof(screen), fp) != NULL)
    {
      printw("%s", screen);
    }
    system(str);
    refresh();
  }
  endwin(); /* End curses mode		  */
  return 0;
}