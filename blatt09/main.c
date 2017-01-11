#include <stdio.h>
#include <stdlib.h> //fuer atoi/atof
#include <ctype.h> //fuer isdigit()
#include "mandelbrot.h"
#include "output.h"
#include <string.h> //fuer strch

int main(int argc, char* argv[]){
    
    //Standardwerte
    double x = -0.7;
    double y = 0.0;
    double d = 3.07;
    int w = 256;
    int h = 192;
    int i = 1000;
    
    int finished = 0;
    /*printf("Kommandoname: %s\n", argv[0]);
    for(int k=1; k<argc;k++){
        printf("Argument %d: %s\n",k,argv[k]);
    }*/
    char* temp;
    
    //Ausgabe falls nur -h vorhanden
    if(argc == 2 && argv[1][1] == 'h' && argv[1][0]== '-' && argv[1][2]=='\0'){
        printf("\n");
        printf("******************* Usage ******************\n");
        printf("\n");
        printf("-x: Re(c),Bildmittelpunkt x-Achse, Standardwert -0.7\n");
        printf("-y: Im(c),Bildmittelpunkt y-Achse, Standardwert 0\n");
        printf("-d: Horizontaler Durchmesser des abzubildenden Bereichs, Standardwert 3.07\n");
        printf("-w: Breite in Pixeln, Standardwert 256\n");
        printf("-h: Hoehe in Pixeln, Standardwert 192\n");
        printf("-i: Maximale Anzahl der Iterationen, Standardwert 1000\n");
        printf("\n");
        printf("********************++**********************\n");
        printf("\n");
    }else if(argc !=2){
        for(int j=1; j<argc; j++){
            //printf("%c\n",argv[j][1]);
            
            //Test ob nicht erlaubte eingaben vorhanden sind
            /*Test 1:es duerfen nur bekannte buchstaben eingegeben werden und negative zahlen sind erlaubt*/
            /*Test 2:keine buchstaben ohne - davor erlaubt*/
            /*Test 3:wert bei d,w,h,i darf nur >0 sein*/
            /*Test 4: bei w, h,i sind nur ganze zahlen erlaubt*/
            if(j+1<argc && ((argv[j][0] == '-' && !((argv[j][1] == 'x') || (argv[j][1] == 'y') || (argv[j][1] == 'd') || (argv[j][1] == 'w') ||(argv[j][1] == 'h') || (argv[j][1] == 'i') || isdigit(argv[j][1])!=0)) || (argv[j][0]!='-' && isdigit(argv[j][0])==0) || ((argv[j][1] == 'd' || argv[j][1] == 'w' || argv[j][1] == 'h' || argv[j][1] == 'i') && argv[j+1][0] == '-' && isdigit(argv[j+1][1])!=0) || ((argv[j][1]=='w'|| argv[j][1]=='h' || argv[j][1]=='i') && ((isdigit(argv[j+1][0])!=0 && strchr(&argv[j+1][0], '.')!=NULL) ||(argv[j+1][0]== '-' && strchr(&argv[j+1][1], '.')!=NULL))))){
                finished = 1;
                printf("Ungueltige Eingabe\n");
                break;
            }
            
            //test ob andere Werte als die Standardwerte verwendet werden sollen
            if(argv[j][0] == '-' /*&& argv[j][2]=='\0'*/ && j+1<argc && isdigit(argv[j][1])==0 && (isdigit(argv[j+1][0])!=0 || (argv[j+1][0]=='-' && isdigit(argv[j+1][1])!=0))){
                //printf("es wird ein wert geaendert!\n");
                switch(argv[j][1]){
                    case 'x':
                        if(argv[j+1][0]== '-'){
                            temp = &argv[j+1][1];
                            x = -1 * atof(temp);
                        }else{
                            temp = &argv[j+1][0];
                            x = atof(temp);
                        }
                        break;
                    case 'y':
                        if(argv[j+1][0]== '-'){
                            temp = &argv[j+1][1];
                            y = -1 * atof(temp);
                        }else{
                            temp = &argv[j+1][0];
                            y = atof(temp);
                        }
                        break;
                    case 'd':
                        temp = &argv[j+1][0];
                        d = atof(temp);
                        break;
                    case 'w':
                        temp = &argv[j+1][0];
                        w = atoi(temp);
                        break;
                    case 'h':
                        temp = &argv[j+1][0];
                        h = atoi(temp);
                        break;
                    case 'i':
                        temp = &argv[j+1][0];
                        i = atoi(temp);
                        break;
                    default:
                        printf("ERROR2\n");
                }
            }
        }
        /*printf("\n");
         printf("x = %f\n", x);
         printf("y = %f\n", y);
         printf("d = %f\n", d);
         printf("w = %d\n", w);
         printf("h = %d\n", h);
         printf("i = %d\n", i);
         printf("\n");*/
        
        if(!finished){
            int iterations[h][w];
        
            printf("calculate\n");
            calculate_Matrix(x,y,d,w,h,i,iterations);
        
            printf("blending\n");
            //blending(h,w,iterations);
        
            printf("print\n");
            output_image(h,w,iterations,i);
        }
    }
    
    
    
    return 0;
}
