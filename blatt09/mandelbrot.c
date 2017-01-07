//
//  mandelbrot.c
//  
//
//  Created by Charlotte Strobel on 29.12.16.
//
//

#include "mandelbrot.h"

int xNull;
int yNull;


int mandelbrot(int x0, int y0, int max_iter){
    double xi = 0.0;
    double yi = 0.0;
    int iteration = 0;
    while (xi*xi + yi*yi < 2*2  &&  iteration < max_iter) {
        int xtemp = xi*xi - yi*yi + x0;
        yi = 2*xi*yi + y0;
        xi = xtemp;
        iteration = iteration + 1;
    }
    
    return iteration;
}

void calculate_Matrix(int weight, int height, int max_iter){
    printf("angekommen\n");
    //festlegen wo die 0 in der Matrix liegt
    xNull = weight/2 - 1;//-1 da bei matrix das zahelen bei 0 losgeht
    yNull = weight/2 - 1;
    int result[weight][height];
    
    //berechnung matrix result
    int x0 = -xNull;
    int y0 = yNull-height+1;
    
    for(int j=0;j<=weight;j++){
        for(int i=0; i<=height; i++){
            printf("spalte:%d, reihe:%d, x0:%d, y0:%d \n", j, i, x0, y0);
            result[j][i] = mandelbrot(x0, y0, max_iter);
            y0++;
        }
        x0++;
    }
}
