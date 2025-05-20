#include "Archivos.h"

int main()
{

    FILE* pf1 = fopen("indices_icc_general_capitulos.csv", "r+");

    if(pf1 == NULL)
    {
        puts("----ERROR durante la apertura de los archivos----");
        return -1;
    }

    archivoNormalizarIcc(pf1);

    fclose(pf1);

    return 0;
}
