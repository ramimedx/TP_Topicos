#ifndef ARCHIVOS_H_INCLUDED
#define ARCHIVOS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_LINEA 59

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
    char clasificador[31];
}sArchivo;

// FUNCIONES PRINCIPALES
void archivoCorregirIcc(FILE* pf);
void archivoCorregirItemsObra(FILE* pf); //HACER
void leerLinea(char* linea, sArchivo* arch);
void modificarLinea(char* linea, sArchivo* arch);
void comaAPunto(char* indice);
void escribirLinea(FILE* pf, char* linea, sArchivo* arch);
void agregarCampo(char* linea, sArchivo* arch); //HACER
void modificarLineaIO(char* linea, sArchivo* arch);
void escribirLineaIO(FILE* pf, char* linea, sArchivo* arch);

// FUNCIONES AUXILIARES
void desencriptarICC(char* str);
void desencriptarItemsObra(char* str);
void normalizarICC(char* str);
void normalizarItemsObra(char* str);

#endif // ARCHIVOS_H_INCLUDED
