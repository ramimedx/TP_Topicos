#ifndef ARCHIVOS_H_INCLUDED
#define ARCHIVOS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ES_LETRA(x) (((x) >= 'a' && (x) <= 'z') || ((x) >= 'A' && (x) <= 'Z') )
#define A_MAYUS(x) ((x) >= 'a' && (x) <= 'z' ? (x) - ('a' - 'A') : (x))
#define A_MINUS(x) ((x) >= 'A' && (x) <= 'Z' ? (x) + ('a' - 'A') : (x))

typedef struct
{
    int dia;
    int mes;
    int anio;
}sFecha;

typedef struct
{
    sFecha fecha;
    char nivel[31];
    char indice[17];
}sArchivo;

// FUNCIONES PRINCIPALES
void archivoNormalizarIcc(FILE* pf);

// FUNCIONES AUXILIARES
void convertirFecha(char* linea);
void desencriptar(char* str);
char* normalizar(char* str);

#endif // ARCHIVOS_H_INCLUDED
