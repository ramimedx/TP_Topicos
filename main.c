#include "Archivos.h"

int main()
{

    FILE* pf1 = fopen(ARCH_ICC_GENERAL_CAPITULOS, "r+");
    FILE* pf2 = fopen(ARCH_ITEMS_OBRA, "r+");
    FILE* pf3 = fopen("temporal.csv","w+");
    FILE* pf4 = fopen("tabla_unificada.csv","w+");


    if(!pf1 || !pf2 || !pf3 || !pf4)
    {
        puts("----ERROR durante la apertura de los archivos----");
        return -1;
    }
    puts("Archivo ICC: \n");
    archivoCorregir(pf1, pf3, ARCH_ICC_GENERAL_CAPITULOS);
    puts("Archivo IO: \n");
    archivoCorregir(pf2, pf3, ARCH_ITEMS_OBRA);

    pf3 = ordenarArchivo(pf3);

    pf3 = AgregarVariacionMensual(pf3);

    pf3 = AgregarVariacionInteranual(pf3);

    crearArchivoUnificado(pf3, pf4);

    fclose(pf1);
    fclose(pf2);
    fclose(pf3);
    fclose(pf4);

    return 0;
}
