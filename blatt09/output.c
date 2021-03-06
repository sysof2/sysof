#include <math.h>
#include "output.h"

 
void output_image(int row, int col, int iterations[row][col], int max_iter){
	
	int cur_iter;
	
	GdkPixbuf* pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8,col,row);
	int n_channels = gdk_pixbuf_get_n_channels(pixbuf);
	int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
	guchar* pixels = gdk_pixbuf_get_pixels(pixbuf);

	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++){
			//printf("%d\n",iterations[i][j]);
			
			guchar* pp = pixels + i * rowstride + j * n_channels;
			
			cur_iter = iterations[i][j];
			get_colour(pp,cur_iter,max_iter);
		}
	}
	gdk_pixbuf_save (pixbuf, "image.jpg", "jpeg",NULL, "quality", "100", NULL);
}


void get_colour(unsigned char *colour, int cur_iter, int max_iter){
	
	if(cur_iter>=max_iter){
		colour[0] = 255;
		colour[1] = 255;
		colour[2] = 255;
	/*
	}else if(cur_iter<max_iter/2){
		colour[0] = 255;
		colour[1] = 0;
		colour[2] = 0;	
	}else if(cur_iter>max_iter/2){
		colour[0] = 0;
		colour[1] = 255;
		colour[2] = 0;
	}else if(cur_iter<=0){
		colour[0] = 0;
		colour[1] = 0;
		colour[2] = 0;
	*/
	
	}else{
		double value = 50*cur_iter;
		double frequency = 5/(double)max_iter;
		
		double r = sin(frequency * value + 0) * (127) + 128;
   		double g = sin(frequency * value + 1) * (127) + 128;
  		double b = sin(frequency * value + 3) * (127) + 128;
	
		colour[0] = r;
		colour[1] = g;
		colour[2] = b;
	}
	
}

//just for some testing, not needed anymore
void blending(int h, int w, int iterations[h][w]){
	for(int i=0;i<h;i++){
		for(int j=0;j<w;j++){
			iterations[i][j] = 4*i+j;
		}
	}
}
