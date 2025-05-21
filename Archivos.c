#include "Archivos.h"

//FUNCIONES PRINCIPALES

void archivoNormalizarIcc(FILE* pf)
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

        modificarLinea(linea, &arch);
        escribirLinea(pf, linea, &arch);

        printf("NUEVA:%s\n", linea);
    }
}

void leerLinea(char* linea, sArchivo* arch)
{
    if(strchr(linea, '/'))
        sscanf(linea, "\"%d/%d/%d\";\"%31[^\"]\";%s", &arch->fecha.dia, &arch->fecha.mes, &arch->fecha.anio, arch->nivel, arch->indice);

    else if(strchr(linea, '-'))
        sscanf(linea, "\"%d-%d-%d\";\"%31[^\"]\";%s", &arch->fecha.anio, &arch->fecha.mes, &arch->fecha.dia, arch->nivel, arch->indice);
}

void modificarLinea(char* linea, sArchivo* arch)
{
    comaAPuntoICC(arch->indice);

    if(!(strstr(arch->nivel, "ivel") || strstr(arch->nivel, "ateriales") || strstr(arch->nivel, "obra") || strstr(arch->nivel, "astos")))
        desencriptarICC(arch->nivel);

    if(strchr(arch->nivel, '_') || strstr(arch->nivel, "materiales"))
        normalizarICC(arch->nivel);

    sprintf(linea, "\"%d-%d-%d\";\"%s\";%s\n", arch->fecha.anio, arch->fecha.mes, arch->fecha.dia, arch->nivel, arch->indice);
}

void comaAPuntoICC(char* indice)
{
    char* ptr = strchr(indice, ',');

    if(ptr)
        *ptr = '.';
}

void escribirLinea(FILE* pf, char* linea, sArchivo* arch)
{
    char aux[TAM_LINEA];

    fgets(aux, TAM_LINEA, pf);

    if(strcmp(linea, aux))
    {
        fseek(pf, (-1)*(strlen(linea)+1), SEEK_CUR);
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

