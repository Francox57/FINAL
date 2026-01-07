#include <stdio.h>
#include "funciones.h"

void mat_copia(int copia[3][3], int i, int j); // Funcion que copia a la matriz referencia en un 3x3
void nace (int mat[ALTO][ANCHO], int i, int j);//Funcion que nace nuevas celulas
void muere (int mat[ALTO][ANCHO], int i, int j);//Funcion que mata las anteriores
void print_mat (int mat[ALTO][ANCHO]);//Funcion para imprimir la matriz


void llenar_mat (int mat[ALTO][ANCHO]){
    int i,j;//Valor de fila y columna
    for(i=0;i<ALTO;i++){//Llena la matriz con celulas muertas
        for(j=0;j<ANCHO;j++){
            mat[i][j] = '*'; 
        }
    }
}


void nace (int mat[ALTO][ANCHO], int i, int j){                     
    mat[i][j]= '*';
}

void muere (int mat[ALTO][ANCHO], int i, int j){
    mat[i][j]= ' ';
}

void print_mat (int mat[ALTO][ANCHO]){ //Funcion para imprimir matriz
    int i,j;//Valor de fila y columna
    int cont = 0;//Inicio un contador que va a tener el valor de los indices en 0
    printf("|^_^|");
    while(cont<ANCHO){ //Imprime una linea con los valores de las celdas
        printf("%2d |", cont);
        cont++;
    }
    putchar('\n');
    cont = 0;
    for(i=0;i<ALTO;i++){
        putchar('|');
        printf("%2d |", cont);//Imprime una fila con los valores de las celdas
        cont++;
        for(j=0;j<ANCHO;j++){
            printf(" %c |", mat[i][j]);//Imprime el caracter de vivo o muerto
        }
        putchar('\n');
    }
}

// FUNCIONES CON ALLEGRO //

void must_init(bool test, const char *descripcion) // Se asegura que todo se inicialize bien
{
    if(test) return; // Si se inicializo bien deja seguir el programa

    printf("No se pudo inicializar %s\n", descripcion);
    exit(1); // Si no se sale del programa y avisa cual no se inicializo correctamente
}

void dibujar_all(int dispAlto, int dispAncho,float lado,ALLEGRO_COLOR color,float platx,float platy,float ballx,float bally){ // Dibuja las filas y columnas calculando el offset
    int i;
    float anchorect = (float)dispAncho/ANCHO; // ancho variable de los rectangulos
    float offsety = (dispAlto/3 - lado*ALTO)/2; // Que tanto de Y se desplaza la grilla en caso de que ANCHO > ALTO para evitar formar rectangulos verticales
    for (i = 0; i <= ANCHO; i++){ // Recorre que tan ancho es la matriz
        float x = i*(anchorect);
        al_draw_line(x,offsety, x, offsety+lado*ALTO, color, 0); // dibuja las lineas que forman las columnas
    }
    for (i = 0; i <= ALTO; i++){
        float y = offsety + i*lado;
        al_draw_line(0,y, dispAncho, y, color, 0);// dibuja las lineas que forman las filas
    }
    
    al_draw_filled_rectangle(platx-3*anchorect, dispAlto - (platy-lado), platx+3*anchorect, dispAlto-(platy), color); // el 3 es un coeficiente se podria agregar algo para poder cambiarlo in game
    al_draw_filled_circle(ballx,dispAlto - (bally+(6*anchorect)/32),(6*anchorect)/32,color);
}

