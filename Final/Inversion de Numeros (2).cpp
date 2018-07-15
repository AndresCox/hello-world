#include <stdio.h>
#include <string.h>
#include <math.h>

void metodoPresentacion(float Num)
{
	int residuo = 0;

	printf("\nMetodo Presentacion: ");

	while (Num != 0)
	{
		residuo = (int)Num % 10;
		printf("%d", residuo);
		Num = (int)Num / 10;
	}

	printf(" <--Presentacion inversa de la escritura");
}

void metodoPresentacionCadena(char *Cad)
{
	int j = strlen(Cad);

	printf("\nMetodo Presentacion Cadena:  ");

	while (j >= 0)
	{
		printf("%c", Cad[j]);
		j--;
	}

	printf(" <--Presentacion inversa de la escritura");
}

float metodo_1(float Num)
{
	int Residuo;
	float NumeroInvertido = 0;

	while (Num != 0)
	{
		Residuo = (int)Num % 10;
		Num = (int)Num / 10;
		NumeroInvertido = NumeroInvertido * 10 + Residuo;
	}

	return NumeroInvertido;
}

float ubicacion(float numero)
{
	int retorno = -1;

	while (numero != 0)
	{
		numero = (int)numero / 10;
		retorno++;
	}

	return retorno;
}

float metodo_2(float num)
{
	int pot = ubicacion(num);

	if (num < 10)
	{
		return num;
	}
	else
	{
		return (int)num % 10 * pow(10, pot) + metodo_2((int)num / 10);
	}
}

void metodo_Sp(float Num, float *metodoSp)
{
	int Residuo;
	float NumeroInvertido = 0;

	while (Num != 0)
	{
		Residuo = (int)Num % 10;
		Num = (int)Num / 10;
		*metodoSp = *metodoSp * 10 + Residuo;
	}
}

char metodo_3(char NumCad[], char metodo3[])
{
	int floatitud = strlen(NumCad) - 1, i, e;

	for (i = floatitud, e = 0; 0 <= i; i--, e++)
	{
		metodo3[e] = NumCad[i];
	}

	metodo3[e] = '\0';
}

void metodo_4(char *NumCad)
{
	strrev(NumCad);
}

int main()
{
	float num;
	char numCad[15], metodo3[15], metodo4[15];
	float metodo1, metodo2, metodoSp = 0;

	printf("Digite un numero(entero): ");
	scanf("%f", &num);

	printf("Digite un numero(cadena): ");
	fflush(stdin);
	gets(numCad);

	metodoPresentacionCadena(numCad); //Presenta en pantalla
	metodoPresentacion(num);		  //Presenta en pantalla

	metodo1 = metodo_1(num); //uso de while
	metodo2 = metodo_2(num); //uso de recursividad

	metodo_Sp(num, &metodoSp); //uso de while por referencia

	metodo_3(numCad, metodo3); //uso de ciclo for y arrays
	metodo_4(numCad);

	printf("\nMetodo 1:            %.0f <--Usando while", metodo1);
	printf("\nMetodo 2:            %.0f <--Usando recursividad", metodo2);
	printf("\nMetodo Sp:           %.0f <--Usando while y punteros", metodoSp);
	printf("\n");
	printf("\nMetodo 3:            %s <--Inversion de cada caracter del array", metodo3);
	printf("\nMetodo 4:            %s <--Usando la funcion strrev()", numCad);
	
	fflush(stdin);
	getchar();
	
	return 0;
}
