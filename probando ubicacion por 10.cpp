#include <stdio.h>
int main(){
	int num,pot;
	int xd=1,retorno=-1;
	
	printf("ingrese un numero: ");
	scanf("%d",&num);
	
	while(num!=0){
		num=num/10;
		retorno++;
	}
	
	printf("potencia: %d",retorno);
	
	return 0;
}
