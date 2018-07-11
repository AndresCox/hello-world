#include <stdio.h>
#include <string.h>
#include <math.h>

void metodoPresentacion(long Num)
{
	int residuo = 0;
	printf("\nMetodo Presentacion: ");
	while (Num != 0)
	{
		residuo = Num % 10;
		printf("%d", residuo);
		Num = Num / 10;
	}
	printf(" <--Presentacion inversa de la escritura");
}

void metodoPresentacionCadena(char *Cad)
{
	int longitud, j;
	for (j = 0; j == '\0'; j++)
		;
	printf("\nMetodo Presentacion Cadena: ");
	for (int i = j; i == 0; i--)
	{
		printf("%c", Cad[i]);
	}
	printf(" <--Presentacion inversa de la escritura(no funciona xd)");
	//fail xd
}

long metodo_1(long Num)
{
	int Residuo;
	long NumeroInvertido = 0;

	while (Num != 0)
	{
		Residuo = Num % 10;
		Num = Num / 10;
		NumeroInvertido = NumeroInvertido * 10 + Residuo;
	}

	return NumeroInvertido;
}

long ubicacion(long numero)
{
	int retorno = -1;

	while (numero != 0)
	{
		numero = numero / 10;
		retorno++;
	}

	return retorno;
}

long metodo_2(long num)
{
	long numeroInvertido = 0;

	int pot = ubicacion(num);

	if (num < 10)
	{
		return num;
	}
	else
	{
		return num % 10 * pow(10, pot) + metodo_2(num / 10);
	}
}

void metodo_Sp(long Num, long *metodoSp)
{
	int Residuo;
	long NumeroInvertido = 0;

	while (Num != 0)
	{
		Residuo = Num % 10;
		Num = Num / 10;
		*metodoSp = *metodoSp * 10 + Residuo;
	}
}
/*
char metodo_3(char NumCad[],char metodo3[]){
	char aux=' ';
	for(int i = 0,j=strlen(NumCad)-1; i < j ; i++,j--){
		aux=NumCad[i];
		NumCad[i]=NumCad[j];
		NumCad[j]=aux;
	}
	strcpy(metodo3,NumCad);
}

char *metodo_4(char *NumCad){
	return strrev(NumCad);
}
*/
int main()
{
	long num;
	//char *numCad,*metodo3,*metodo4;
	long metodo1, metodo2, metodoSp = 0;

	printf("Digite un numero(entero): ");
	scanf("%ld", &num);

	//printf("Digite un numero(cadena): ");
	//scanf("%s",&numCad);

	//metodoPresentacionCadena(numCad);
	metodoPresentacion(num);

	metodo1 = metodo_1(num); //uso de while
	metodo2 = metodo_2(num); //uso de recursividad

	metodo_Sp(num, &metodoSp); //uso de while por referencia

	//metodo_3(numCad,metodo3);//uso de ciclo for y arrays
	//strcpy(metodo_4,strrev(numCad));//uso de strrev()

	printf("\nMetodo 1:            %ld <--Usando while", metodo1);
	printf("\nMetodo 2:            %ld <--Usando recursividad", metodo2);
	printf("\nMetodo Sp:           %ld <--Usando while y punteros", metodoSp);

	//printf("\nMetodo 3: %s",metodo3);
	//printf("\nMetodo 4: %s",metodo4);

	return 0;
}
