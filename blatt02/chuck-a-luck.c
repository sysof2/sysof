#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	
	printf("**** Chuck-a-luck ****\n");
	
	int credit = 1000;
	int exit_status = 0;
	int w1 = 0,w2 = 0,w3 = 0;
	int money_set = 0;
	int money_received = 0;
	int number = 0;
	
	//display rules
	printf("Sie haben 1000 Geldeinheiten.\
	\nIn jeder Runde koennen Sie einen Teil davon auf eine der Zahlen 1 bis 6 setzen.\
	\nDanach werden 3 Wuerfel geworfen. Falls Ihr Wert dabei ist, erhalten Sie Ihren Einsatz zurueck\
	\nund zusaetzlich Ihren Einsatz fuer jeden Wuerfel, der die von Ihnen gesetzte Zahl zeigt.\n\n");
	
	while(exit_status == 0) {
		
		printf("Sie haben %d Geldeinheiten\n",credit);
		
		printf("Ihr Einsatz: ");
		while(scanf("%d",&money_set)==0||money_set<0||money_set>credit) {
			printf("Ungueltige Eingabe!!!\n");
			printf("Ihr Einsatz: ");
			int c;
			while((c=getchar()) != '\n' && c!=EOF)
			continue;
		}
		credit-=money_set;
		
		if(money_set==0){
			exit_status=1;
		}else{
			
			printf("Sie haben %d Geldeinheiten\n",credit);
			
			printf("Ihre Zahl: ");
			while(scanf("%d",&number)==0||number<1||number>6) {
				printf("Ungueltige Eingabe!!!\n");
				printf("Ihre Zahl: ");
				int c;
				while((c=getchar()) != '\n' && c!=EOF)
				continue;
			}
			
			srand(time(NULL));
			w1 = rand()%6+1; // Zahl zwischen 1 und 6
			w2 = rand()%6+1; // Zahl zwischen 1 und 6
			w3 = rand()%6+1; // Zahl zwischen 1 und 6
			
			printf("Die Wuerfel sind gefallen: %d %d %d\n",w1,w2,w3);
			
			money_received = 0;
			if(number==w1) money_received = money_set*2;
			if(number==w2) money_received += money_set*2;
			if(number==w3) money_received += money_set*2;
			
			credit+=money_received;
			
			if(money_received != 0){
				printf("Glueckwunsch, Sie erhalten %d Geldeinheiten!!\n",money_received);
			}else{
				printf("Pech gehabt!\n");
			}
			
			if(credit==0){
				exit_status=2;
			}
		}
	}
	
	if(exit_status==1){
		printf("Glueckwunsch, Sie verlassen das Casino mit %d Geldeinheiten\n",credit);
	}else{
		printf("Sie haben kein Geld mehr. Bitte verlassen sie das Casino!!\n");
	}
	return 0;
}