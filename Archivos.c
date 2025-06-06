#include "Archivos.h"

//FUNCIONES PRINCIPALES

void archivoCorregir(FILE* pf, FILE* temp, const char* band)
{
    char linea[TAM_LINEA];
    sArchivo arch;

    rewind(pf);
    fgets(linea, TAM_LINEA, pf);

    while(fgets(linea, TAM_LINEA, pf))
    {
        if (strchr(linea, '\n') == NULL && !feof(pf))
        {
            printf("⚠️ Línea demasiado larga. Considerá aumentar TAM_LINEA\n");
        }

        leerLinea(linea, &arch);
        printf("VIEJA:%s", linea);

        modificarLinea(linea, &arch, band);
        escribirLinea(temp, linea, &arch);

        printf("NUEVA:%s\n", linea);
    }

}

void leerLinea(char* linea, sArchivo* arch)
{
    if(strchr(linea, '/'))
        sscanf(linea, "\"%d/%d/%d\";\"%50[^\"]\";%s",
               &arch->fecha.dia, &arch->fecha.mes, &arch->fecha.anio, arch->nivel, arch->indice);

    else if(strchr(linea, '-'))
        sscanf(linea, "\"%d-%d-%d\";\"%s\";%s",
               &arch->fecha.anio, &arch->fecha.mes, &arch->fecha.dia, arch->nivel, arch->indice);

    char* nl = strchr(arch->indice, '\n');
    if(nl)
        *nl = '\0';
}

void modificarLinea(char* linea, sArchivo* arch, const char* band)
{
    comaAPunto(arch->indice);

    if(strcmp(band,ARCH_ICC_GENERAL_CAPITULOS) == 0)
    {

    if(!(strstr(arch->nivel, "ivel") || strstr(arch->nivel, "ateriales") || strstr(arch->nivel, "obra") || strstr(arch->nivel, "astos")))
        desencriptarICC(arch->nivel);

    if(strchr(arch->nivel, '_') || strstr(arch->nivel, "materiales"))
        normalizarICC(arch->nivel);
    }

    if(strcmp(band, ARCH_ITEMS_OBRA) == 0)
    {
        desencriptarItemsObra(arch->nivel);
        normalizarIO(arch->nivel);
    }

    agregarCampo(arch, band);

    sprintf(linea, "\"%d-%d-%d\";\"%s\";%s;\"%s\"\n",
            arch->fecha.anio, arch->fecha.mes, arch->fecha.dia, arch->nivel, arch->indice, arch->clasificador);
}

void comaAPunto(char* indice)
{
    char* ptr = strchr(indice, ',');

    if(ptr)
        *ptr = '.';
}

void agregarCampo(sArchivo* arch, const char* band)
{
    if(strcmp(band,ARCH_ICC_GENERAL_CAPITULOS) == 0)
    {
    if(strstr(arch->nivel, "Nivel general"))
        strcpy(arch->clasificador, "Nivel general");

    else
        strcpy(arch->clasificador, "Capitulos");
    }
    if(strcmp(band,ARCH_ITEMS_OBRA) == 0)
    {
        strcpy(arch->clasificador, "Items");
    }
}

void escribirLinea(FILE* pf, char* linea, sArchivo* arch)
{

        fprintf(pf, "\"%d-%d-%d\";\"%s\";%s;\"%s\"\n", arch->fecha.anio, arch->fecha.mes, arch->fecha.dia, arch->nivel, arch->indice, arch->clasificador);
        fflush(pf);
}


//FUNCIONES AUXILIARES

void desencriptarICC(char* str)
{
    char ABC[] = "abcdefghijklmnopqrstuvwxyz";
    char* ptr = ABC;
    int pos = 0;
    int lugar = 0;
    int longABC = strlen(ABC);

    while(*str)
    {
        pos++;

        while(*str && ES_LETRA(*str))
        {
            lugar = 0;

            while(*str != *ptr)
            {
                ptr++;
                lugar++;
            }

            if(pos%2 == 0)
                *str = *(ABC +(2+lugar)%longABC);

            else
                *str = *(ABC +(4+lugar)%longABC);

            ptr = ABC;
            pos++;
            str++;
        }

        while(*str && !ES_LETRA(*str))
            str++;
    }
}


void desencriptarItemsObra(char* str)
{
    char tabla[2][11] =
    {
        {"@8310$7|59"},
        {"abeiostlmn"}
    };
    int i;

    while(*str)
    {
        i = 0;

        if(*str && *str == '_')
            str++;

        while(i < 10 && tabla[0][i] != *str)
            i++;

        if(i < 10)
            *str = tabla[1][i];

        str++;
    }
}

void normalizarICC(char* str)
{
    *str = A_MAYUS(*str);

    while(*str)
    {
        if(*str == '_')
            *str = ' ';

        str++;
    }
}

void normalizarIO(char* str)
{
    int band = 0;
    char* inicio = str;
    char* ult = str + strlen(str) - 1;
    char* i;

    while(*str)
    {
        if(band == 0 && *str == '_')
        {
            for(i = inicio; i < ult; i++)
            {
                *i = *(i + 1 + (str - inicio));
            }

            *i = '\0';
            band = 1;

            *inicio = A_MAYUS(*inicio);
        }

        if(*str == '_')
            *str = ' ';

        str++;
    }
}
