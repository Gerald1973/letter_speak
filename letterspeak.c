#include <curses.h>
#include <stdlib.h>

void buildMenuWindow();
void game_letter_speak();
void game_guess_the_letter();

int main(void)
{
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  start_color();
  bool hasColors = has_colors();
  bool canChangeColor = can_change_color();
  int character = 0;
  printw("The terminal has color        : %d \n", hasColors);
  printw("The user can modify the color : %d \n", canChangeColor);
  buildMenuWindow();
  endwin();
  return 0;
}

void buildMenuWindow()
{
  int character = 0;
  WINDOW *my_window = newwin(10, 40, 7, 20);
  keypad(my_window, TRUE);
  waddstr(my_window, "1   | Letter speak\n");
  waddstr(my_window, "2   | Guess the letter \n");
  waddstr(my_window, "END | Quit\n");
  while (character != KEY_END)
  {
    wrefresh(my_window);
    character = wgetch(my_window);
    switch (character)
    {
    case '1':
      game_letter_speak();
      break;
    case '2':
      game_guess_the_letter();
      break;
    default:
      break;
    }
  }
  return;
}

void game_letter_speak()
{
  FILE *fp;
  int character = 0;
  char str[80];
  char str2[80];
  char screen[2000];
  while (character != KEY_HOME)
  {
    clear();
    character = getch(); /* Wait for user input */
    sprintf(str2, "figlet -c '%c'", character);
    sprintf(str, "espeak-ng -v mb-fr2 -s50 '%c'", character);
    fp = popen(str2, "r");
    while (fgets(screen, sizeof(screen), fp) != NULL)
    {
      printw("%s", screen);
    }
    refresh();
    system(str);
    flushinp();
  }
  buildMenuWindow();
  return;
}

void game_guess_the_letter()
{
  int character = 0;
  int character_from_player = 0;
  bool continue_game = TRUE;
  bool retry = FALSE;
  char str_for_espeak[80];
  char str_for_figlet[2000];
  while (continue_game)
  {
    clear();
    refresh();
    if (!retry)
    {
      character = 'A' + (rand() % 26);
    }
    sprintf(str_for_espeak, "espeak-ng -v mb-fr2 -s50 'Appuye sur la lettre %c'", character);
    system(str_for_espeak);
    character_from_player = getch();
    if (character_from_player == KEY_HOME)
    {
      continue_game = FALSE;
    }
    else if (character_from_player == character || character_from_player == character + 32)
    {
      system("espeak-ng -v mb-fr2 -s100 'Super! Louis, tu as trouvé!'");
      retry = FALSE;
    }
    else
    {
      system("espeak-ng -v mb-fr2 -s50 \"Eh bien Louis, ce n'est pas la bonne lettre, réessaye.\"");
      retry = TRUE;
    }
    flushinp();
  };
  buildMenuWindow();
  return;
}