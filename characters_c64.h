#ifndef CHARACTERS_C64_H
#define CHARACTERS_C64_H

int char_ascii_offset(char c);
void char_write_binary(uint8_t byte, char *result);
void char_build_matrix(char c, char *results);
void draw_char(WINDOW *window, char c, int x, int y);

#endif
