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

void calculate_Matrix(int weight, int height, int max_iter);

#endif /* mandelbrot_h */
