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
int buflength = 50000, options = 0;
unsigned int position = 0, position_type = 0, end_position = 0, flags = espeakCHARS_AUTO;
time_t t;

char words[][20] = {"Arbre",
                    "Bébé",
                    "Couleur",
                    "Dinosaure",
                    "Eléphant",
                    "Fleur",
                    "Giraffe",
                    "Hippopotame",
                    "Iguane",
                    "Jumelles",
                    "Kangourou",
                    "Lampe",
                    "Montre",
                    "Nuage",
                    "Ordinateur",
                    "Papa",
                    "Quatre",
                    "Raisin",
                    "Serpent",
                    "Tomate",
                    "Usine",
                    "Vache",
                    "Wagon",
                    "Xylophone",
                    "Yacht",
                    "Zèbre"};

int speak(char *text, int rate);

int main(void)
{
  srand((unsigned) time(&t));
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
  waddstr(my_window, "1   | Prononce la lettre\n");
  waddstr(my_window, "2   | Devine la lettre\n");
  waddstr(my_window, "END | Quitte\n");
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
    speak(str, 80);
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
  int letterNumber = 0;
  bool continue_game = TRUE;
  bool retry = FALSE;
  char str_for_letter[80];
  char str_for_word[80];
  while (continue_game)
  {
    clear();
    refresh();
    if (!retry)
    {
      letterNumber = rand() % 26;
      character = 'A' + letterNumber;
    }
    sprintf(str_for_letter,"Appuie sur la lettre %c", character);
    sprintf(str_for_word," comme %s", words[letterNumber]);
    speak(str_for_letter, 80);
    speak(str_for_word, 80);
    character_from_player = getch();
    if (character_from_player == KEY_HOME)
    {
      continue_game = FALSE;
    }
    else if (character_from_player == character || character_from_player == character + 32)
    {
      speak("Super! Louis, tu as trouvé!", 120);
      retry = FALSE;
    }
    else
    {
      speak("Eh bien Louis, ce n'est pas la bonne lettre, réessaye.", 140);
      retry = TRUE;
    }
    flushinp();
  };
  buildMenuWindow();
  return;
}

int speak(char *text, int rate)
{
  espeak_SetParameter(espeakRATE, rate, 0);
  espeak_SetParameter(espeakVOICETYPE,1,0);
  espeak_SetParameter(espeakVOLUME,95,0);
  espeak_VOICE voice;
  memset(&voice, 0, sizeof(espeak_VOICE)); // Zero out the voice first
  voice.identifier = "mb-fr4";
  espeak_SetVoiceByProperties(&voice);
  espeak_Synth(text, buflength, position, position_type, end_position, flags, identifier, user_data);
  return 0;
}