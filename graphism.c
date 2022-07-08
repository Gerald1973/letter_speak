#include "graphism.h"
#include <ncurses.h>

void graph_plot(WINDOW *window, int x0, int y0)
{
    init_pair(1, COLOR_YELLOW, COLOR_RED);
    wattrset(window, COLOR_PAIR(1));
    wmove(window, y0, x0);
    waddch(window, ' ');
    wattrset(window, A_NORMAL);
}

void graph_line(WINDOW *window, int x0, int y0, int x1, int y1)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;
    for (;;)
    {
        graph_plot(window, x0, y0);
        if (x0 == x1 && y0 == y1)
            break;
        e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void drawCircle(WINDOW *window, int xc, int yc, int x, int y)
{
    graph_plot(window, xc + x, yc + y);
    graph_plot(window, xc - x, yc + y);
    graph_plot(window, xc + x, yc - y);
    graph_plot(window, xc - x, yc - y);
    graph_plot(window, xc + y, yc + x);
    graph_plot(window, xc - y, yc + x);
    graph_plot(window, xc + y, yc - x);
    graph_plot(window, xc - y, yc - x);
}

void graph_circle(WINDOW *window, int xc, int yc, int r)
{
    int x = 0, y = r;
    int d = 3 - 2 * r;
    drawCircle(window, xc, yc, x, y);
    while (y >= x)
    {
        x++;
        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
        {
            d = d + 4 * x + 6;
        }
        drawCircle(window, xc, yc, x, y);
    }
}
