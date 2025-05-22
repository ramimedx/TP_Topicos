#include "Archivos.h"

int main()
{

    FILE* pf1 = fopen("indices_icc_general_capitulos.csv", "r+");

    if(pf1 == NULL)
    {
        puts("----ERROR durante la apertura de los archivos----");
        return -1;
    }

    //archivoCorregirIcc(pf1);

    char ejemplo[] = "5_19$7@|@c109_$@917@r1@_y_c097r@19c39d10";
    desencriptarItemsObra(ejemplo);
    puts("items obra desencriptado: ");
    puts(ejemplo);

    normalizarItemsObra(ejemplo);
    puts("items obra normalizado: ");
    puts(ejemplo);

    fclose(pf1);

    return 0;
}
