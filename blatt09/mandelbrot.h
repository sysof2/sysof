//
//  mandelbrot.h
//  
//
//  Created by Charlotte Strobel on 29.12.16.
//
//

#ifndef mandelbrot_h
#define mandelbrot_h

#include <stdio.h>
#include <stdlib.h>

/* calculates if point is in madelbrot set
 *returns number of needed iterations
 */
int mandelbrot();

void calculate_Matrix(double x, double y, double d, int w, int h, int max_iter, int iterations[h][w]);

#endif /* mandelbrot_h */
