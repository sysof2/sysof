#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void kegelAusgeben(int anzahlKegelBeginn, unsigned long int zahl){

    for(int k=0; k<anzahlKegelBeginn;k++){
		if(zahl & (1<<k)){
			printf("/\\ ");
		}else{
			printf("   ");
		}
    }
	printf("\n");

	int count = 1;
	for(int k=0; k<anzahlKegelBeginn;k++){
		if(zahl & (1<<k)){
			printf("%02d ",count);	
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
	
	//Faelle die nicht erlaubt sind: 1.keine ganze Zahl 2.Zahl außerhalb anzahl der Kegel 3.your Move = 0 4.Der Kegel wurde schon umgekegelt 5.bei negativer zahl: beide Kegel usind schon umgekegelt gewesen. Bei einem ist es noch ok.
	while(scanf("%d",&yourMove)==0||yourMove>anzahlKegelBeginn||yourMove<=-anzahlKegelBeginn||yourMove==0
	||((yourMove>0)&&!(zahl&(1<<(yourMove-1))))||((yourMove<0)&&!(zahl&((1<<(-yourMove-1))|(1<<-yourMove))))){
		printf("Ungueltige Eingabe!!!\n");
		printf("Your move: ");
		int c;
		while((c=getchar())!= '\n' && c!=EOF);
	}

	printf("\n");
	
	return yourMove;
}

int myKegeln(int anzahlKegelBeginn, unsigned long int zahl){
	
	int myMove = rand()%anzahlKegelBeginn + 1;
	int vorzeichen = rand()%2;
	//printf("vorzeichen: %d\n",vorzeichen);
	
	if(vorzeichen==0){
		myMove = -myMove;
	}
	
	while(myMove>anzahlKegelBeginn||myMove<=-anzahlKegelBeginn||myMove==0
	||((myMove>0)&&!(zahl&(1<<(myMove-1))))||(((myMove<0)&&!(zahl&((1<<(-myMove-1))|(1<<-myMove)))))){
		myMove = rand()%anzahlKegelBeginn + 1;
	}

	return myMove;
}

unsigned long int kegeln(unsigned long int zahl, int move){
	zahl &= ~(1<<(move-1));
	return zahl;
}

int main(){
	setbuf(stdout, NULL); //option for np++ console output
	int yourMove = 0;
	int myMove = 0;

   	printf("***Dudeney's Game of Kayles***\n");
   	srand(time(NULL));
   	int anzahlKegel = rand() % 32 + 1;
   	int anzahlKegelBeginn = anzahlKegel;
   	unsigned long int zahl = 0;
   	for(int i = 0; i<=anzahlKegelBeginn;i++){
     		 zahl |= (1<<i);
   	}	
	
   	for(;;){
      	//print_bits(zahl,anzahlKegelBeginn);
		//printf("\n");
		kegelAusgeben(anzahlKegelBeginn,zahl);
		yourMove = yourKegeln(anzahlKegelBeginn,zahl);
		
		if(yourMove>0){
			anzahlKegel--;
			zahl = kegeln(zahl, yourMove);
		}else{
			zahl = kegeln(zahl,-yourMove);
			yourMove--;
			zahl = kegeln(zahl, -yourMove);
			anzahlKegel -= 2;
		}
		
		if(anzahlKegel==0){
			printf("You win!\n");
			break;
		}

		myMove = myKegeln(anzahlKegelBeginn,zahl);

		printf("My move: %d\n",myMove);

		if(myMove>0){
			anzahlKegel--;
			zahl = kegeln(zahl, myMove);
		}else{
			zahl = kegeln(zahl,-myMove);
			myMove--;
			zahl = kegeln(zahl,-myMove);
			anzahlKegel -= 2;
		}
		
		if(anzahlKegel==0){
			printf("You lose!\n");
			break;
		}
		
   	}


}
