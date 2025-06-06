#include "Archivos.h"

int main()
{

    FILE* pf1 = fopen(ARCH_ICC_GENERAL_CAPITULOS, "r+");
    FILE* pf2 = fopen(ARCH_ITEMS_OBRA, "r+");
    FILE* pf3 = fopen("temporal.csv","w+");


    if(!pf1 || !pf2 || !pf3)
    {
        puts("----ERROR durante la apertura de los archivos----");
        return -1;
    }
    fprintf(pf3, "\"periodo\";\"nivel_general_aperturas\";\"indice_icc\";\"clasificador\"\n");
    puts("Archivo ICC: \n");
    archivoCorregir(pf1, pf3, ARCH_ICC_GENERAL_CAPITULOS);
    puts("Archivo IO: \n");
    archivoCorregir(pf2, pf3, ARCH_ITEMS_OBRA);

    fclose(pf1);
    fclose(pf2);
    fclose(pf3);

    return 0;
}
