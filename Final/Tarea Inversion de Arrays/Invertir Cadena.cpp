#include <stdio.h>
#define MAX 30

void Ingreso(char *cad);
void Presentar(char *cad, int);
void InversionCadena(char *Cad);

int main()
{
    /* code */
    char cadena[MAX + 1];

    Ingreso(cadena);
    Presentar(cadena, 0);
    InversionCadena(cadena);
    printf("\n\n   Inversion\n\n");
    Presentar(cadena, 1);

    return 0;
}

void Ingreso(char *cad)
{
    printf("\n Ingrese una cadena:  ");
    gets(cad);
}

void Presentar(char *cad, int tipo)
{
    if (tipo == 0)
    {
        printf(" La cadena es:           %s\n", cad);
    }
    else
    {
        printf(" La cadena invertida es: %s\n", cad);
    }
}

void InversionCadena(char *Cad)
{
    int tam, i, j;
    char temp, *str1, *str2;

    str1 = Cad;
    str2 = Cad;

    for (tam = 0; *str2 != '\0'; str2++, tam++)
        ;

    str2--;
    tam--;

    for (i = 0; i < (tam / 2); i++, str1++, str2--)
    {
        temp = *str1;
        *str1 = *str2;
        *str2 = temp;
    }
}
