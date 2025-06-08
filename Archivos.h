#ifndef ARCHIVOS_H_INCLUDED
#define ARCHIVOS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_LINEA 95

#define ARCH_ICC_GENERAL_CAPITULOS "indices_icc_general_capitulos.csv"
#define ARCH_ITEMS_OBRA "Indices_items_obra.csv"

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
    char nivel[50];
    char indice[18];
    char clasificador[31];
    char variacionmensual [6];
    char variacioninteranual [8];
}sArchivo;

// FUNCIONES PRINCIPALES
void archivoCorregir(FILE* pf, FILE* temp, const char* band);
void leerLinea(char* linea, sArchivo* arch);
void modificarLinea(char* linea, sArchivo* arch, const char* band);
void comaAPunto(char* indice);
void escribirLinea(FILE* pf, char* linea, sArchivo* arch);
void agregarCampo(sArchivo* arch, const char* band);
FILE* ordenarArchivo (FILE* temp);
FILE* AgregarVariacionMensual (FILE *temp);
FILE* AgregarVariacionInteranual (FILE *temp);


// FUNCIONES AUXILIARES
void desencriptarICC(char* str);
void desencriptarItemsObra(char* str);
void normalizarICC(char* str);
void normalizarIO(char* str);
sArchivo* cargarRegistros (FILE* temp, int *cant_registros);
void ordenarPorBurbujeo (sArchivo* vec_archivos, int cant_registros);
int compararClasificador(const char* clasificador);
int contarLineas (FILE *temp);
void calcularVariacionMensual (sArchivo *vec_archivos, int cant_registros);
double buscarIndiceActual (sArchivo *vec_archivos, int cant_registros, int i_mesAnterior, int *i_actual, const int band);
void calcularVariacionMensual(sArchivo *vec_archivos, int cant_registros);
void calcularVariacionInteranual(sArchivo *vec_archivos, int cant_registros);
#endif // ARCHIVOS_H_INCLUDED
