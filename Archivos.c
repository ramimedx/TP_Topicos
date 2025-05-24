#include "Archivos.h"

//FUNCIONES PRINCIPALES

void archivoCorregirIcc(FILE* pf)
{
    char linea[TAM_LINEA];
    sArchivo arch;
    FILE* temp = fopen("temporal.csv", "w+");

    rewind(pf);
    fgets(linea, TAM_LINEA, pf);

    fprintf(temp, "\"periodo\";\"nivel_general_aperturas\";\"indice_icc\";\"clasificador\"\n");

    while(fgets(linea, TAM_LINEA, pf))
    {
        leerLinea(linea, &arch);
        printf("VIEJA:%s", linea);

        modificarLineaIcc(linea, &arch);
        escribirLineaIcc(temp, linea, &arch);

        printf("NUEVA:%s\n", linea);
    }

    fclose(temp);
}

void leerLinea(char* linea, sArchivo* arch)
{
    if(strchr(linea, '/'))
        sscanf(linea, "\"%d/%d/%d\";\"%50[^\"]\";%s",
               &arch->fecha.dia, &arch->fecha.mes, &arch->fecha.anio, arch->nivel, arch->indice);

    else if(strchr(linea, '-'))
        sscanf(linea, "\"%d-%d-%d\";\"%s\";%s",
               &arch->fecha.anio, &arch->fecha.mes, &arch->fecha.dia, arch->nivel, arch->indice);
}

void modificarLineaIcc(char* linea, sArchivo* arch)
{
    comaAPunto(arch->indice);

    if(!(strstr(arch->nivel, "ivel") || strstr(arch->nivel, "ateriales") || strstr(arch->nivel, "obra") || strstr(arch->nivel, "astos")))
        desencriptarICC(arch->nivel);

    if(strchr(arch->nivel, '_') || strstr(arch->nivel, "materiales"))
        normalizarICC(arch->nivel);

    agregarCampo(arch);

    sprintf(linea, "\"%d-%d-%d\";\"%s\";%s;\"%s\"\n",
            arch->fecha.anio, arch->fecha.mes, arch->fecha.dia, arch->nivel, arch->indice, arch->clasificador);
}

void comaAPunto(char* indice)
{
    char* ptr = strchr(indice, ',');

    if(ptr)
        *ptr = '.';
}

void agregarCampo(sArchivo* arch)
{
    if(strstr(arch->nivel, "Nivel general"))
        strcpy(arch->clasificador, "Nivel general");

    else
        strcpy(arch->clasificador, "Capitulos");
}

void escribirLineaIcc(FILE* pf, char* linea, sArchivo* arch)
{
    fprintf(pf, "\"%d-%d-%d\";\"%s\";%s;\"%s\"\n",
        arch->fecha.anio, arch->fecha.mes, arch->fecha.dia, arch->nivel, arch->indice, arch->clasificador);
    fflush(pf);
}

void archivoCorregirItemsObra(FILE* pf)
{
    char linea[TAM_LINEA];
    sArchivo arch;

    rewind(pf);
    fgets(linea, TAM_LINEA, pf);

    while(fgets(linea, TAM_LINEA, pf))
    {
        leerLinea(linea, &arch);
        printf("VIEJA:%s", linea);

        fseek(pf, (-1)*(strlen(linea)+1), SEEK_CUR);

        modificarLineaIO(linea, &arch);
        escribirLineaIO(pf, linea, &arch);

        printf("NUEVA:%s\n", linea);
    }
}

void modificarLineaIO(char* linea, sArchivo* arch)
{
    comaAPunto(arch->indice);
    desencriptarItemsObra(arch->nivel);

    //normalizarItemsObra(arch->nivel);

    sprintf(linea, "\"%d-%d-%d\";\"%s\";%s\n", arch->fecha.anio, arch->fecha.mes, arch->fecha.dia, arch->nivel, arch->indice);
}

void escribirLineaIO(FILE* pf, char* linea, sArchivo* arch)
{
    char aux[TAM_LINEA];

    fgets(aux, TAM_LINEA, pf);

    if(strcmp(linea, aux))
    {
        fseek(pf, (-1)*(strlen(aux)+1), SEEK_CUR);
        fprintf(pf, "\"%d-%d-%d\";\"%s\";%s\n", arch->fecha.anio, arch->fecha.mes, arch->fecha.dia, arch->nivel, arch->indice);
        fflush(pf);
    }
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

void desencriptarItemsObra(char* str)
{
    char tabla[2][11] = {
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

void normalizarItemsObra(char* str)
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
