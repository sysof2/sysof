#ifndef output_h
#define output_h

#include <stdio.h>
#include <stdlib.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

void output_image(int row, int col, int iterations[row][col], int max_iter);
void get_colour(unsigned char *colour, int cur_iter, int max_iter);
void blending(int h, int w, int iterations[h][w]);

#endif /* output_h */ 
