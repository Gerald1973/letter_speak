#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <espeak-ng/speak_lib.h>
#include <ctype.h>
#include "graphism.h"
#include "characters_c64.h"

void buildMenuWindow();
void writeLetterInTheCenter(WINDOW *window, char character);
WINDOW *game_letter_speak();
void game_guess_the_letter();
void buildFunctionBar(WINDOW *window, char *text);
int displayPictureForLetter(WINDOW **window, WINDOW **sub, char character, int begin_y, int begin_x, int height, int width);

espeak_AUDIO_OUTPUT output = AUDIO_OUTPUT_SYNCH_PLAYBACK;
char *path = NULL;
void *user_data;
unsigned int *identifier;
int buflength = 50000, options = 0;
unsigned int position = 0, position_type = 0, end_position = 0, flags = espeakCHARS_AUTO;
WINDOW *menuWindow = NULL;

time_t t;

char successSentences[][256] = {
    "Bravo Louis !",
    "Génial!",
    "Tu es super intelligent!",
    "Continue, tu vas tout savoir !",
    "Sensationnel, tu connais tout !",
    "Super! Louis, tu as trouvé!"};

char words[][20] = {"Arbre",
                    "Bébé",
                    "Cadeau",
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
  setlinebuf(stderr);
  srand((unsigned)time(&t));
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
  int width = 40;
  int height = 10;
  int y = 0;
  int x = 0;
  getmaxyx(stdscr, y, x);
  if (menuWindow == NULL)
  {
    menuWindow = newwin(height, width, (y - height) / 2, (x - width) / 2);
    keypad(menuWindow, TRUE);
    box(menuWindow, 0, 0);
    wmove(menuWindow, 1, 1);
    wprintw(menuWindow, " 1    Je prononce la lettre");
    wmove(menuWindow, 2, 1);
    wprintw(menuWindow, " 2    Devine la lettre");
    wmove(menuWindow, 3, 1);
    wprintw(menuWindow, " END  Quitte");
  }
  wrefresh(menuWindow);
  do
  {
    character = wgetch(menuWindow);
    flushinp();
    switch (character)
    {
    case '1':
      game_letter_speak();
      touchwin(menuWindow);
      break;
    case '2':
      game_guess_the_letter();
      touchwin(menuWindow);
      break;
    default:
      break;
    }
  } while (character != KEY_END);
  return;
}

WINDOW *game_letter_speak()
{
  int character = 0;
  int upperCharacter = 0;
  int x = 0;
  int y = 0;
  getmaxyx(stdscr, y, x);
  char str[80];
  char screen[2000];
  WINDOW *drawingWindow = NULL;
  WINDOW *subDrawingWindow = NULL;
  WINDOW *me = newwin(y, x / 2, 0, 0);
  keypad(me, TRUE);
  while (character != KEY_HOME)
  {
    wclear(me);
    if (character != 0)
    {
      upperCharacter = toupper(character);
      displayPictureForLetter(&drawingWindow, &subDrawingWindow, character, y, x - (x / 2), 0, x / 2);
      writeLetterInTheCenter(me, character);
      wmove(me, 0, 0);
    }
    buildFunctionBar(me, "Je prononce la lettre | Menu (home)");
    wrefresh(me);
    if (character != 0)
    {
      sprintf(str, "%c", character);
      speak(str, 80);
      if (upperCharacter >= 'A' && upperCharacter <= 'Z')
      {
        sprintf(str, "comme %s", words[upperCharacter - 'A']);
        speak(str, 80);
      }
    }
    character = wgetch(me);
    flushinp();
  }
  wclear(subDrawingWindow);
  wclear(drawingWindow);
  wclear(me);
  wrefresh(subDrawingWindow);
  wrefresh(drawingWindow);
  wrefresh(me);
  delwin(subDrawingWindow);
  delwin(drawingWindow);
  return me;
}

void game_guess_the_letter()
{
  int character = 0;
  int character_from_player = 0;
  int letterNumber = 0;
  bool continue_game = TRUE;
  bool retry = FALSE;
  char str_for_letter[80];
  char str_for_word[80];
  char str_for_error[160];
  int x = 0;
  int y = 0;
  WINDOW *drawingPicture = NULL;
  WINDOW *subDrawingPicture = NULL;
  WINDOW *me = NULL;
  getmaxyx(stdscr, y, x);
  me = newwin(2, x, y - 2, 0);
  keypad(me, TRUE);
  while (continue_game)
  {
    if (!retry)
    {
      letterNumber = rand() % 26;
      character = 'A' + letterNumber;
    }
    buildFunctionBar(me, "Devine la lettre | Menu (home)");
    displayPictureForLetter(&drawingPicture, &subDrawingPicture, character, y - 2, x, 0, 0);
    wrefresh(me);
    sprintf(str_for_letter, "Appuie sur la lettre %c", character);
    sprintf(str_for_word, " comme %s", words[letterNumber]);
    speak(str_for_letter, 80);
    speak(str_for_word, 80);
    character_from_player = wgetch(me);
    if (character_from_player == KEY_HOME)
    {
      continue_game = FALSE;
    }
    else if (character_from_player == character || character_from_player == character + 32)
    {
      speak(successSentences[rand() % 6], 120);
      retry = FALSE;
    }
    else
    {
      sprintf(str_for_error, "Eh bien Louis, ce n'est pas la bonne lettre, tu as appuyé sur lettre %c, réessaye.", character_from_player);
      speak(str_for_error, 140);
      retry = TRUE;
    }
    flushinp();
  };
  wclear(me);
  wclear(subDrawingPicture);
  wclear(drawingPicture);
  wrefresh(me);
  wrefresh(subDrawingPicture);
  wrefresh(drawingPicture);
  delwin(subDrawingPicture);
  delwin(drawingPicture);
  delwin(me);
  return;
}

int speak(char *text, int rate)
{
  espeak_SetParameter(espeakRATE, rate, 0);
  espeak_SetParameter(espeakVOICETYPE, 1, 0);
  espeak_SetParameter(espeakVOLUME, 100, 0);
  espeak_SetParameter(espeakPITCH, 60, 0);
  espeak_VOICE voice;
  memset(&voice, 0, sizeof(espeak_VOICE)); // Zero out the voice first
  voice.identifier = "mb-fr4";
  espeak_SetVoiceByProperties(&voice);
  espeak_Synth(text, buflength, position, position_type, end_position, flags, identifier, user_data);
  return 0;
}

void writeLetterInTheCenter(WINDOW *window, char character)
{
  int x = 0;
  int y = 0;
  getmaxyx(window, y, x);
  draw_char(window, character, (x - 8) / 2, (y - 8) / 2);
}

void buildFunctionBar(WINDOW *window, char *text)
{
  int x = 0;
  int y = 0;
  int gap = 2;
  char functionBar[2048];
  getmaxyx(window, y, x);
  for (int i = 0; i < x; i++)
  {
    functionBar[i] = ' ';
  }
  for (int i = gap; i < (strlen(text) + gap); i++)
  {
    functionBar[i] = text[i - gap];
  }
  functionBar[x] = 0;
  wattron(window, A_REVERSE);
  wmove(window, y - 2, 0);
  waddstr(window, functionBar);
  wattrset(window, A_NORMAL);
}

int displayPictureForLetter(WINDOW **me, WINDOW **sub, char character, int height, int width, int begin_y, int begin_x)
{
  int draw_width = 40;
  int draw_height = 25;
  int tmp = 0;
  char drawing[1025];
  char fileName[256];
  int maxX = 0;
  int maxY = 0;
  if (*me == NULL)
  {
    *me = newwin(height, width, begin_y, begin_x);
    getmaxyx(*me, maxY, maxX);
    *sub = derwin(*me, draw_height, draw_width, (maxY - draw_height) / 2, (maxX - draw_width) / 2);
  }
  wclear(*me);
  wclear(*sub);
  int lowerCase = tolower(character);
  switch (lowerCase)
  {
  case 'a':
    strncpy(fileName, "data/arbre.txt", sizeof(fileName));
    break;
  case 'b':
    strncpy(fileName, "data/bebe.txt", sizeof(fileName));
    break;
  case 'c':
    strncpy(fileName, "data/cadeau.txt", sizeof(fileName));
    break;
  case 'd':
    strncpy(fileName, "data/dinosaure.txt", sizeof(fileName));
    break;
  case 'e':
    strncpy(fileName, "data/elephant.txt", sizeof(fileName));
    break;
  case 'f':
    strncpy(fileName, "data/fleur.txt", sizeof(fileName));
    break;
  case 'g':
    strncpy(fileName, "data/girafe.txt", sizeof(fileName));
    break;
  case 'h':
    strncpy(fileName, "data/hippopotame.txt", sizeof(fileName));
    break;
  case 'i':
    strncpy(fileName, "data/iguane.txt", sizeof(fileName));
    break;
  case 'j':
    strncpy(fileName, "data/jumelles.txt", sizeof(fileName));
    break;
  case 'k':
    strncpy(fileName, "data/kangourou.txt", sizeof(fileName));
    break;
  case 'l':
    strncpy(fileName, "data/lampe.txt", sizeof(fileName));
    break;
  case 'm':
    strncpy(fileName, "data/montre.txt", sizeof(fileName));
    break;
  case 'n':
    strncpy(fileName, "data/nuage.txt", sizeof(fileName));
    break;
  case 'o':
    strncpy(fileName, "data/ordinateur.txt", sizeof(fileName));
    break;
  case 'p':
    strncpy(fileName, "data/papa.txt", sizeof(fileName));
    break;
  case 'q':
    strncpy(fileName, "data/quatre.txt", sizeof(fileName));
    break;
  case 'r':
    strncpy(fileName, "data/raisin.txt", sizeof(fileName));
    break;
  case 's':
    strncpy(fileName, "data/serpent.txt", sizeof(fileName));
    break;
  case 't':
    strncpy(fileName, "data/tomate.txt", sizeof(fileName));
    break;
  case 'u':
    strncpy(fileName, "data/usine.txt", sizeof(fileName));
    break;
  case 'v':
    strncpy(fileName, "data/vache.txt", sizeof(fileName));
    break;
  case 'w':
    strncpy(fileName, "data/wagon.txt", sizeof(fileName));
    break;
  case 'x':
    strncpy(fileName, "data/xylophone.txt", sizeof(fileName));
    break;
  case 'y':
    strncpy(fileName, "data/yacht.txt", sizeof(fileName));
    break;
  case 'z':
    strncpy(fileName, "data/zebre.txt", sizeof(fileName));
    break;
  default:
    strncpy(fileName, "data/other.txt", sizeof(fileName));
  }

  FILE *fptr = fopen(fileName, "r");
  if (fptr != NULL)
  {
    for (int i = 0; i < 1025; i++)
    {
      tmp = fgetc(fptr);
      if (tmp != '\n')
      {
        waddch(*sub, tmp);
      }
    }
    fclose(fptr);
  }
  wrefresh(*me);
  return 0;
}