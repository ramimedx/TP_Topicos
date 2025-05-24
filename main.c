#include "Archivos.h"

int main()
{

    FILE* pf1 = fopen("indices_icc_general_capitulos.csv", "r+");
    FILE* pf2 = fopen("Indices_items_obra.csv", "r+");


    if(!pf1 || !pf2)
    {
        puts("----ERROR durante la apertura de los archivos----");
        return -1;
    }

    //archivoCorregirIcc(pf1);
    archivoCorregirItemsObra(pf2);

    fclose(pf1);
    fclose(pf2);

    return 0;
}
