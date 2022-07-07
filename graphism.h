#ifndef GRAPHISM_H
#define GRAPHISM_H

#include <stdio.h>
#include <curses.h>
#include <stdlib.h>

void graph_plot(WINDOW *window, int x0, int y0);

void graph_line(WINDOW *window, int x0, int y0, int x1, int y1);

void graph_circle(WINDOW *window, int x0, int y0, int radius);

#endif