#include <stdio.h>

void Ingreso(int *cad, int tamanio);
void Presentar(int *cad, int, int tamanio);
void InversionCadena(int *Cad, int tamanio);

int main()
{
	/* code */
	int tamanio;

	printf("\n Ingrese el tamanio: ");
	scanf("%d", &tamanio);

	int cadena[tamanio];

	Ingreso(cadena, tamanio);
	Presentar(cadena, 0, tamanio);
	InversionCadena(cadena, tamanio);
	printf("\n\n   Inversion\n\n");
	Presentar(cadena, 1, tamanio);

	fflush(stdin);
	getchar();

	return 0;
}

void Ingreso(int *cad, int tamanio)
{
	printf(" Ingrese la cadena:  \n\n");
	for (int i = 0; i < tamanio; i++)
	{
		printf("   Cadena[%d] = ", i);
		scanf("%d", &cad[i]);
	}
}

void Presentar(int *cad, int tipo, int tamanio)
{
	if (tipo == 0)
	{
		printf("\n La cadena es:           ", cad);
		for (int i = 0; i < tamanio; i++)
		{
			printf("%d ", cad[i]);
		}
	}
	else
	{
		printf(" La cadena invertida es: ");
		for (int i = 0; i < tamanio; i++)
		{
			printf("%d ", cad[i]);
		}
	}
}

void InversionCadena(int *Cad, int tamanio)
{
	int tam, i;
	int temp, *str1, *str2;

	for (tam = 0, str2 = Cad; tam < tamanio; tam++, str2++);

	str2--;

	for (i = 0, str1 = Cad; i < (tamanio / 2); i++, str1++, str2--)
	{
		temp = *str1;
		*str1 = *str2;
		*str2 = temp;
	}
}
