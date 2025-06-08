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
        escribirLinea(temp, linea, &arch, 0);

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

void escribirLinea(FILE* pf, char* linea, sArchivo* arch, const int band)
{
    if (band == 0) //Se escribe con el campo clasificador
    {
        fprintf(pf, "\"%d-%d-%d\";\"%s\";%s;\"%s\"\n", arch->fecha.anio, arch->fecha.mes, arch->fecha.dia, arch->nivel, arch->indice, arch->clasificador);
        fflush(pf);
    }
    else if (band == 1) //Se escribe con el campo Variacion Mensual
    {
        fprintf(pf, "\"%d-%d-%d\";\"%s\";%s;\"%s\";%s\n", arch->fecha.anio, arch->fecha.mes, arch->fecha.dia, arch->nivel, arch->indice, arch->clasificador, arch->variacionmensual);
        fflush(pf);
    }
    else //Se escribe con el campo Variacion Anual
    {
       fprintf(pf, "\"%d-%d-%d\";\"%s\";%s;\"%s\";%s;%s\n", arch->fecha.anio, arch->fecha.mes, arch->fecha.dia, arch->nivel, arch->indice, arch->clasificador, arch->variacionmensual
               ,arch->variacioninteranual);
       fflush(pf);
    }
}

FILE* ordenarArchivo (FILE* temp)
{
    sArchivo *Vector_archivos;
    char linea[TAM_LINEA];
    int cant_registros, i;
    FILE *ordenado;

    ordenado = fopen("ordenado.csv","w+");

    if (ordenado == NULL)
    {
        puts ("Hubo un error al crear el archivo ordenado.");
        exit (1);
    }

    Vector_archivos = cargarRegistros (temp, &cant_registros);

    if ( Vector_archivos == NULL)
    {
        puts ("Hubo un error al cargar los registros en el vector.");
        exit (1);
    }

    ordenarPorBurbujeo (Vector_archivos, cant_registros);

    i = 0;

    fprintf(ordenado, "\"periodo\";\"nivel_general_aperturas\";\"indice_icc\";\"clasificador\"\n");

    for (i = 0; i < cant_registros; i++)
    {
        escribirLinea(ordenado, linea, Vector_archivos+i, 0);
    }

    fclose(ordenado);
    fclose(temp);
    remove("temporal.csv");
    rename("ordenado.csv","temporal.csv");

    ordenado = fopen("temporal.csv","r+");

    if (ordenado == NULL)
    {
        puts ("Hubo un error al crear el archivo ordenado.");
        exit (1);
    }

    free(Vector_archivos);

    return ordenado;
}

FILE* AgregarVariacionMensual (FILE *temp)
{
    char linea[TAM_LINEA];
    sArchivo *vec_archivos;
    int cant_registros, i;

    vec_archivos = cargarRegistros(temp, &cant_registros);

    calcularVariacionMensual(vec_archivos,cant_registros);

    fclose (temp);

    temp = fopen("temporal.csv","w+");

    if (temp == NULL)
    {
        puts ("Hubo un error al crear el archivo temporal.");
        exit (1);
    }

    fprintf(temp, "\"periodo\";\"nivel_general_aperturas\";\"indice_icc\";\"clasificador\";\"var_mensual\"\n");

    for (i = 0; i < cant_registros; i++)
        escribirLinea (temp, linea, vec_archivos+i, 1);

    free (vec_archivos);

    return temp;
}

FILE* AgregarVariacionInteranual (FILE* temp)
{
    char linea[TAM_LINEA];
    sArchivo *vec_archivos;
    int cant_registros, i;

    vec_archivos = cargarRegistros(temp, &cant_registros);

    calcularVariacionInteranual(vec_archivos, cant_registros);

    fclose(temp);

    temp = fopen("temporal.csv","w+");

    if (temp == NULL)
    {
        puts ("Hubo un error al crear el archivo temporal.");
        exit (1);
    }

    fprintf(temp, "\"periodo\";\"nivel_general_aperturas\";\"indice_icc\";\"clasificador\";\"var_mensual\";\"var_interanual\"\n");

    for (i = 0; i < cant_registros; i++)
        escribirLinea(temp, linea, vec_archivos+i,2);

    free (vec_archivos);

    return temp;
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

sArchivo* cargarRegistros (FILE *temp, int *cant_registros)
{
    sArchivo *vec, *vec2;
    int i;
    char linea[TAM_LINEA], *nl;

    *cant_registros = contarLineas (temp);

    vec = malloc (sizeof(sArchivo) * (*cant_registros));

    if (vec == NULL)
    {
        puts("No se pudo crear el vector dinamico.");
        return vec;
    }

    vec2 = vec;

    rewind(temp);

    fgets(linea, TAM_LINEA,temp);

    for (i = 0; i < *cant_registros; i++)
    {
        vec2 = vec + i;
        fgets(linea, TAM_LINEA, temp);
       sscanf(linea, "\"%d-%d-%d\";\"%49[^\"]\";%17[^;];\"%35[^\"]\"", &vec2->fecha.anio, &vec2->fecha.mes, &vec2->fecha.dia, vec2->nivel, vec2->indice, vec2->clasificador);
        nl = strchr(vec2->clasificador, '\n');
        if(nl)
            *nl = '\0';
    }

    return vec;
}

int compararClasificador (const char* clasificador)
{
    if (strcmp(clasificador,"Nivel general") == 0)
        return 0;
    if (strcmp(clasificador,"Capitulos") == 0)
        return 1;
    else
        return 2;
}

void ordenarPorBurbujeo(sArchivo* vec_archivos, int cant_registros)
{
    sArchivo aux, *vec;
    int cambiar, i, j;

    vec = vec_archivos;

    for (i = 0; i < cant_registros - 1; i++)
    {
        for (j = 0; j < cant_registros - i - 1; j++)
        {
            cambiar = 0;

            //comparo fechas
            if ((vec+j)->fecha.anio > (vec+j+1)->fecha.anio)
                cambiar = 1;
            else if ( (vec+j)->fecha.anio == (vec+j+1)->fecha.anio && (vec+j)->fecha.mes > (vec+j+1)->fecha.mes)
                cambiar = 1;
            else if ((vec+j)->fecha.anio == (vec+j+1)->fecha.anio && (vec+j)->fecha.mes == (vec+j+1)->fecha.mes && (vec+j)->fecha.dia > (vec+j+1)->fecha.dia)
                cambiar = 1;
            else if ((vec+j)->fecha.anio == (vec+j+1)->fecha.anio && (vec+j)->fecha.mes == (vec+j+1)->fecha.mes && (vec+j)->fecha.dia == (vec+j+1)->fecha.dia)
            {
                if (compararClasificador((vec+j)->clasificador) > compararClasificador((vec+j+1)->clasificador))
                    cambiar = 1;
            }

            if (cambiar)
            {
                aux = *(vec+j);
                *(vec+j) = *(vec+j+1);
                *(vec+j+1) = aux;
            }
        }
    }
}


int contarLineas (FILE *temp)
{
    char linea [TAM_LINEA];
    int contador = 0;

    rewind(temp);
    fgets(linea, TAM_LINEA, temp);

    while (fgets(linea, TAM_LINEA, temp))
        contador++;

    return contador;
}

void calcularVariacionMensual (sArchivo *vec_archivos, int cant_registros)
{
    double variacion_mensual, indice_actual, indice_mesanterior;
    sFecha primeraFecha;
    int i, j;

    primeraFecha = vec_archivos->fecha;

    for (i = 0; i < cant_registros; i++)
    {
        indice_mesanterior = atof((vec_archivos+i)->indice);
        indice_actual = buscarIndiceActual (vec_archivos, cant_registros, i, &j, 0);

        variacion_mensual = (indice_actual/indice_mesanterior - 1) * 100;

        sprintf((vec_archivos+j)->variacionmensual, "%.2f", variacion_mensual);
    }

    i = 0;

    while ((vec_archivos+i)->fecha.anio == primeraFecha.anio && (vec_archivos+i)->fecha.mes == primeraFecha.mes &&(vec_archivos+i)->fecha.dia == primeraFecha.dia)
    {
        strcpy((vec_archivos+i)->variacionmensual,"NA"); //Se pone "NA", porque al ser el periodo mas antiguo no hay uno anterior para comparar.
        i++;
    }
}

double buscarIndiceActual (sArchivo *vec_archivos, int cant_registros, int i_anterior, int *i_actual, const int band)
{
    int encontrado = 0;
    double indice_actual;

    *i_actual = i_anterior+1;

    if (!band) //busca indice mensual
    {
        while (*i_actual < cant_registros && !encontrado)
        {
            if (!strcmp((vec_archivos+i_anterior)->nivel, (vec_archivos+(*i_actual))->nivel))
                encontrado = 1;
            else
                (*i_actual)++;
        }
    }
    else //busca indice interanual
    {
        while (*i_actual < cant_registros && !encontrado)
        {
            if (!strcmp((vec_archivos+i_anterior)->nivel, (vec_archivos+(*i_actual))->nivel) && (vec_archivos+i_anterior)->fecha.anio == ((vec_archivos+(*i_actual))->fecha.anio - 1)
                && (vec_archivos+i_anterior)->fecha.mes == (vec_archivos+(*i_actual))->fecha.mes)
                encontrado = 1;
            else
                (*i_actual)++;
        }
    }

    indice_actual = atof((vec_archivos+(*i_actual))->indice);

    return indice_actual;
}

void calcularVariacionInteranual(sArchivo *vec_archivos, int cant_registros)
{
    double variacion_internanual, indice_actual, indice_anioanterior;
    sFecha primeraFecha, ultimafecha;
    int i, j;

    i = 0;

    primeraFecha = vec_archivos->fecha;
    ultimafecha = (vec_archivos+(cant_registros-1))->fecha;

    while (((vec_archivos+i)->fecha.anio+1 != ultimafecha.anio || (vec_archivos+i)->fecha.mes-1 != ultimafecha.mes) && i < cant_registros)
    {
        indice_anioanterior = atof ((vec_archivos+i)->indice);
        indice_actual = buscarIndiceActual(vec_archivos, cant_registros, i, &j, 1);

        variacion_internanual = (indice_actual/indice_anioanterior - 1)*100;

        sprintf((vec_archivos+j)->variacioninteranual, "%.2f", variacion_internanual);
        i++;
    }

    j = 0;

    while((vec_archivos+j)->fecha.anio == primeraFecha.anio)
    {
        strcpy((vec_archivos+j)->variacioninteranual,"NA"); //Se pone "NA", porque al ser el periodo mas antiguo no hay uno anterior para comparar.
        j++;
    }

    if (i != cant_registros)
    {
        while (i < cant_registros)
        {
            strcpy((vec_archivos+i)->variacioninteranual,"NA"); //Se pone "NA", porque no hay otros registros que sean un anio superiores.
            i++;
        }
    }
}
