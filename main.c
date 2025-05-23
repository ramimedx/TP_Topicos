#include "Archivos.h"

int main()
{

    FILE* pf1 = fopen("indices_icc_general_capitulos.csv", "r+t");
    FILE* pf2 = fopen("Indices_items_obra.csv", "r+t");

    if(pf1 == NULL || pf2 == NULL)
    {
        puts("----ERROR durante la apertura de los archivos----");
        return -1;
    }

    archivoCorregirIcc(pf1);
    archivoCorregirItemsObra(pf2);

    fclose(pf1);
    fclose(pf2);

    return 0;
}
