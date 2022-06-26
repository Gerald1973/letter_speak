#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <espeak-ng/speak_lib.h>

void buildMenuWindow();
void game_letter_speak();
void game_guess_the_letter();
espeak_AUDIO_OUTPUT output = AUDIO_OUTPUT_SYNCH_PLAYBACK;
char *path = NULL;
void *user_data;
unsigned int *identifier;
int buflength = 500, options = 0;
unsigned int position = 0, position_type = 0, end_position = 0, flags = espeakCHARS_AUTO;

int speak(char *text);

int main(void)
{
  espeak_Initialize(output, buflength, path, options);
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
  do
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
  } while (character != KEY_END);
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
    character = getch();
    clear();
    sprintf(str2, "figlet -c '%c'", character);
    sprintf(str, "%c", character);
    fp = popen(str2, "r");
    while (fgets(screen, sizeof(screen), fp) != NULL)
    {
      printw("%s", screen);
    }
    refresh();
    speak(str);
    flushinp();
  }
  buildMenuWindow();
  return;
}
// espeak-ng -v mb-fr2 -s50 'Appuye sur la lettre %c'"
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
    sprintf(str_for_espeak, "Appuye sur la lettre %c", character);
    speak(str_for_espeak);
    character_from_player = getch();
    if (character_from_player == KEY_HOME)
    {
      continue_game = FALSE;
    }
    else if (character_from_player == character || character_from_player == character + 32)
    {
      speak("Super! Louis, tu as trouvé!");
      retry = FALSE;
    }
    else
    {
      speak("Eh bien Louis, ce n'est pas la bonne lettre, réessaye.");
      retry = TRUE;
    }
    flushinp();
  };
  buildMenuWindow();
  return;
}

int speak(char *text)
{
  espeak_SetParameter(espeakRATE, 120, 0);
  espeak_VOICE voice;
  memset(&voice, 0, sizeof(espeak_VOICE)); // Zero out the voice first
  const char *langNativeString = "fr";     // Set voice by properties
  voice.languages = langNativeString;
  voice.name = "mb-fr2";
  voice.variant = 2;
  voice.gender = 2;
  espeak_SetVoiceByProperties(&voice);
  espeak_Synth(text, buflength, position, position_type, end_position, flags, identifier, user_data);
  return 0;
}