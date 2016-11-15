#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

double kahan(int decimal[30], int number){
	double c = 0.0;
	double sum = number;
	for(int x=0; x < 30; x++){
		double y = decimal[x] * pow(10,(-1)*x-1) - c;
	//	printf("y: %f\n", y);
	//	printf("c: %f\n", c);
		double temp = sum + y;
		c = (temp - sum) - y;
		sum = temp;
		printf("sum %f\n", sum);
	}
	return sum;
}

int main() {
	typedef enum {S0, S1, S2, S3, S4, S5, S6, S7} State;

	State state = S0;
	int sign = 1;  
	int sign2 = 1;
	int ch;
	int number = 0;
	int number2 = 0;
	bool finished = false;
	int decimal[30];
	int i = 0; //Laufvariable fuer decimal
	int potenz = 0;

	for(int z=0; z<30; z++){
		decimal[z]=0;
	}

	printf("Please type in a number!\n");
	while(!finished && (ch = getchar())!=EOF){
		switch(state){
			case S0:
				if(ch == '+' || ch == '-'){
					if(ch == '-'){
						sign = sign * (-1);
						state = S1;
					}
				}
				else if(isdigit(ch)){
					number = ch - '0';
					printf("number1: %d\n", number);
					state = S2;
				}
				else if(ch == '.'){
					state = S3;
				}
			break;

			case S1:
				if(ch == '.'){
					state = S3;
				}
				else if(isdigit(ch)){
					number = number * 10 + ch - '0';
					printf("number2: %d\n", number);
					state = S2;
				}
			break;

			case S2:
				if(isdigit(ch)){
					number = number * 10 + ch - '0';
					printf("number3: %d\n", number);
				}
				else if(ch == '.'){
					state = S4;
				}
				else if(ch == 'E' || ch == 'e'){
					state = S5;
				}
				else{
					finished = true;
				}
			break;

			case S3:
				if(isdigit(ch)){
					decimal[i] = ch - '0'; 
					i++;
					state = S4;
				}
			break;

			case S4:
				if(isdigit(ch)){
					decimal[i] = ch - '0';
					i++;
				}
				else if(ch == 'E' || ch == 'e'){
					state = S5;
				}
				else{
					finished = true;
				} 
			break;	

			case S5:
				if(isdigit(ch)){
					potenz = potenz*10 + ch - '0';
					state = S7;
				}
				else if(ch == '+' || ch == '-'){
					if(ch == '-'){
						sign2 = sign2 * (-1);
					}
					state = S6;
				}
			break;

			case S6:
				if(isdigit(ch)){
					potenz = potenz*10 + ch - '0';
					state = S7;
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
	double value = 0.0;
	printf("potenz: %d\n", potenz);
	printf("decimal: ");
	for(int y=0; y<10;y++){
		printf("%d ", decimal[y]);
	}
	printf("\n");

	if(decimal[0]!=0){
		value = kahan(decimal, number);
	}
	else{
		value = number;
	}
	value = sign * value * pow(10,sign2 * potenz);
	printf("Value = %f\n", value);

	return 0;
}

