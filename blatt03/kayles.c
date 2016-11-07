#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


void kegelAusgeben(int anzahlKegelBeginn, unsigned long int zahl){

     	for(int k = anzahlKegelBeginn-1;k>=0;k--){
		if((zahl & (int)pow(2,k))!=0){
			printf("/\\ ");
		}else{
			printf("   ");
		}
      	}
	printf("\n");

	int count = 1;
	for(int z = anzahlKegelBeginn-1; z >=0 ;z--){
		if((zahl & (int)pow(2,z)) !=0){
			if(count<10){
				printf("0%d ",count);
			}else{
				printf("%d ",count);
			}	
		}else{
			printf("   ");
		}	
		count++;
	}
	printf("\n");
}

void print_bits(unsigned long int zahl, int anzahlKegelBeginn){
	for(int i=0;i<anzahlKegelBeginn;++i){
		int bit = (zahl >> (anzahlKegelBeginn-i-1)) & 0x1;
		putchar('0'+bit);
	}
}

int yourKegeln(int anzahlKegelBeginn,unsigned long int zahl){
	int yourMove;
	printf("Your move: ");
	/*Faelle die nicht erlaubt sind: 1.keine ganze Zahl 2.Zahl außerhalb anzahl der Kegel 3.your Move = 0 4.Der Kegel wurde schon umgekegelt 5.bei negativer zahl: beide Kegel usind schon umgekegelt gewesen. Bei einem ist es noch ok.  */
	while(scanf("%d",&yourMove)==0 || ((yourMove>anzahlKegelBeginn) && (yourMove<=(-1)*anzahlKegelBeginn)) || yourMove==0 
	|| ((yourMove>0)&&((zahl & (int)pow(2,anzahlKegelBeginn-yourMove))==0)) 
	|| ((yourMove<0) &&((zahl & (int)pow(2,anzahlKegelBeginn-fabs(yourMove)))==0)) || ((yourMove<0) && ((zahl & (int)pow(2,anzahlKegelBeginn-fabs(yourMove)-1))==0))){
		printf("Ungueltige Eingabe!!!\n");
		printf("Your move: ");
		int c;
		while((c=getchar())!= '\n' && c!=EOF){
			continue;
		}
	}

	printf("\n");
	
	return yourMove;
}

int myKegeln(int anzahlKegelBeginn, unsigned long int zahl){
	
	int myMove = rand()%anzahlKegelBeginn + 1;
	int vorzeichen = rand()%2;
	printf("vorzeichen: %d\n",vorzeichen);
	while((vorzeichen==0 && !(((zahl & (int)pow(2,anzahlKegelBeginn-myMove))==0) && ((zahl & (int)pow(2,anzahlKegelBeginn-myMove-1))==0))) 
	|| (vorzeichen==1 && ((zahl & (int)pow(2,anzahlKegelBeginn-myMove))==0))){
		myMove = rand()%anzahlKegelBeginn + 1;
	}

	if(vorzeichen==0){
		myMove = (-1)*myMove;
	}

	return myMove;
}

unsigned long int kegeln(int anzahlKegelBeginn, unsigned long int zahl, int move){
	
	unsigned long int tempVar = pow(2, anzahlKegelBeginn-fabs(move));
	tempVar = ~tempVar;
	zahl = zahl & tempVar;
	return zahl;
}

int main(){

	int yourMove = 0;
	int myMove = 0;

   	printf("***Dudeney's Game of Kayles***\n");
   	srand(time(NULL));
   	int anzahlKegel = rand() % 32 + 1;
   	int anzahlKegelBeginn = anzahlKegel;
   	unsigned long int zahl = 0;
   	for(int i = 0; i<=anzahlKegelBeginn;i++){
     		 zahl = zahl + pow(2,i);
   	}	

   	for(;;){
      		print_bits(zahl,anzahlKegelBeginn);
		printf("\n");
		kegelAusgeben(anzahlKegelBeginn,zahl);
		yourMove = yourKegeln(anzahlKegelBeginn,zahl);
		zahl = kegeln(anzahlKegelBeginn, zahl, yourMove);

		if(yourMove>0){
			anzahlKegel = anzahlKegel - 1;
		}else{
			yourMove--;
			zahl = kegeln(anzahlKegelBeginn, zahl, yourMove);
			anzahlKegel = anzahlKegel - 2;
		}
		
		if(zahl==0){
			printf("You win!\n");
			break;
		}

		myMove = myKegeln(anzahlKegelBeginn,zahl);

		printf("My move: %d\n",myMove);


		zahl = kegeln(anzahlKegelBeginn, zahl, myMove);

		if(myMove>0){
			anzahlKegel--;
		}else{
			myMove--;
			zahl = kegeln(anzahlKegelBeginn, zahl, myMove);
			anzahlKegel = anzahlKegel - 2;
		}
		
		if(zahl==0){
			printf("You lose!\n");
			break;
		}
   	}


}
