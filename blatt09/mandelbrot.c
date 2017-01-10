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


int mandelbrot(double x0, double y0, int max_iter){
    double xi = 0.0;
    double yi = 0.0;
    int iteration = 0;
    while (xi*xi + yi*yi < 2*2  &&  iteration < max_iter) {
        double xtemp = xi*xi - yi*yi + x0;
        yi = 2*xi*yi + y0;
        xi = xtemp;
        iteration = iteration + 1;
    }
    
    return iteration;
}

void calculate_Matrix(double x, double y, double d, int w, int h, int max_iter, int iterations[h][w]){
    
	//calculate vertikal diameter
	double pageratio = h/w;
	double vertdia = pageratio * d;
    
    //start values of x0,y0, top left corner of the picture
    double x0 = x-d/2;
    double y0 = y+vertdia/2;
	
	double step = d/w;

    for(int i=0;i<h;i++){
        for(int j=0; j<w; j++){
            //printf("spalte:%d, reihe:%d, x0:%d, y0:%d \n", j, i, x0, y0);
            iterations[i][j] = mandelbrot(x0, y0, max_iter);
            x0 += step;
			//printf("x0:%3.1fy0:%3.1f; ",x0,y0);
			//printf("%3.1f; ",x0);
			//printf("%3.1f; ",y0);
			
		}
        //printf("\n");
		x0 = x-d/2;
        y0 -= step;
    }
    
    /*
    for(int i=0;i<h;i++){
		for(int j=0; j<w; j++){
			printf("%d ",iterations[i][j]);
		}
		printf("\n");
	}
	*/
	//printf("testing\n");
	//int test = mandelbrot(-1,0.8,1000);
	//printf("%d\n",test);
}
