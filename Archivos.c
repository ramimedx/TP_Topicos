#include "Archivos.h"

//FUNCIONES PRINCIPALES

void archivoNormalizarIcc(FILE* pf)
{
    char linea[59];
    sArchivo arch;

    rewind(pf);
    fgets(linea, sizeof(linea), pf);

    while(fgets(linea, sizeof(linea), pf))
    {
        sscanf(linea, "\"%d/%d/%d\";\"%31[^\"]\";%s", &arch.fecha.dia, &arch.fecha.mes, &arch.fecha.anio, arch.nivel, arch.indice);
        printf("%s", linea);
        fseek(pf, (-1)*(strlen(linea)+1), SEEK_CUR);
        fprintf(pf, "\"%d-%d-%d\";\"%s\";%s\n", arch.fecha.anio, arch.fecha.mes, arch.fecha.dia, arch.nivel, arch.indice);
        fflush(pf);
    }
}

//FUNCIONES AUXILIARES
/*
void desencriptar (char* str)
{
    char* ABC = "abcdefghijklmnñopqrstuvwxyz";
    char* ptrIn = ABC;
    int pos;
    int longABC = strlong(ABC);

    while(*str)
    {
        while(*str && !esLetra(*str))
            str++;

        while(*str && esLetra(*str))
        {
            pos = buscarStr(ABC, *str);
            ABC += pos;

            if(pos%2 == 0)
            {
                *str = *(ABC + 4);
            }

            else
            {
                *str = *(ABC + 2);
            }

            ABC = ptrIn;
            str++;
        }

        str++;
    }
}


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

