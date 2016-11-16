#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

double read_double();

int main(){
	printf("Please type in a number!\n");
	
	int i=0;
	
	while(1){
		
		double res = read_double();
		printf("Number %d: %f\n",i++,res);
	}
	
	return 0;
}

double read_double(){
	typedef enum {S0, S1, S2, S3, S4, S5, S6, S7} State;

	State state = S0;
	int sign = 1;
	int sign_exp = 1;
	int potenz = 0;
	int ch;
	long value = 0;
	long value_dec = 0;
	int i_dec = 0;
	bool finished = false;
	
	while(!finished && (ch = getchar())!=EOF){
		switch(state){
			case S0:
				if(ch == '+' || ch == '-'){
					if(ch == '-'){
						sign = -1;
					}
					state = S1;
				}else if(isdigit(ch)){
					value = ch - '0';
					state = S2;
				}else if(ch == '.'){
					state = S3;
				}else{
					finished = true;
				}
				break;

			case S1:
				if(ch == '.'){
					state = S3;
				}else if(isdigit(ch)){
					value = value * 10 + ch - '0';
					state = S2;
				}else{
					finished = true;
				}
				break;

			case S2:
				if(isdigit(ch)){
					value = value * 10 + ch - '0';
				}else if(ch == '.'){
					state = S4;
				}
				else if(ch == 'E' || ch == 'e'){
					state = S5;
				}else{
					finished = true;
				}
				break;

			case S3:
				if(isdigit(ch)){
					value_dec = value_dec * 10 + ch - '0';
					i_dec++;
					state = S4;
				}else{
					finished = true;
				}
				break;

			case S4:
				if(isdigit(ch)){
					value_dec = value_dec * 10 + ch - '0';
					i_dec++;
				}else if(ch == 'E' || ch == 'e'){
					state = S5;
				}else{
					finished = true;
				} 
				break;	

			case S5:
				if(isdigit(ch)){
					potenz = potenz*10 + ch - '0';
					state = S7;
				}else if(ch == '+' || ch == '-'){
					if(ch == '-'){
						sign_exp = -1;
					}
					state = S6;
				}else{
					finished = true;
				}
				break;

			case S6:
				if(isdigit(ch)){
					potenz = potenz*10 + ch - '0';
					state = S7;
				}else{
					finished = true;
				}
				break;

			case S7:
				if(isdigit(ch)){
					potenz = potenz * 10 + ch - '0';
				}
				else{
					finished = true;
				}
				break;
		}
	}
	return  sign * (value+value_dec*pow(10,(-1)*i_dec))* pow(10,sign_exp*potenz);
}
