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
    comaAPunto(arch);

    if(!(strstr(arch->nivel, "nivel") || strstr(arch->nivel, "mat") || strstr(arch->nivel, "mano") || strstr(arch->nivel, "gastos")))
        desencriptar(arch->nivel);

    sprintf(linea, "\"%d-%d-%d\";\"%s\";%s\n", arch->fecha.anio, arch->fecha.mes, arch->fecha.dia, arch->nivel, arch->indice);
}

void comaAPunto(sArchivo* arch)
{
    char* ptr = strchr(arch->indice, ',');

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

void desencriptar(char* str)
{
    char ABC[] = "abcdefghijklmnopqrstuvwxyz";
    char* ptr = ABC;
    int pos = 0;
    int lugar = 0;
    int longABC = strlen(ABC);

    while(*str)
    {
        pos++;

        while(*str && !ES_LETRA(*str))
            str++;

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
    }
}

/*
char* normalizar(char* str)
{
    char* inicio = str;
    char* ptrAux = str;

    while(*str)
    {
        while(*str && esLETRA(*str))
        {
            if(str == inicio)
                *ptrAux = aMAYUS(*str);
            else if(!esLETRA(*(str-1)))
                *ptrAux = aMAYUS(*str);
            else
                *ptrAux = aMINUS(*str);

            ptrAux++;
            str++;
        }

        if(*str && *str == '_')
        {
            *ptrAux = ' ';
            ptrAux++;
        }

        str++;
    }

    *ptrAux = '\0';

    return inicio;
}*/

