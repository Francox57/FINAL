#ifndef FUNCIONES_H
#define FUNCIONES_H
#define ANCHO 30
#define ALTO 30
#define MAXTHEMES 4
#if (ANCHO<=0 || ALTO<=0)
#error valor invalido para matriz
#elif(ANCHO>50 || ALTO>50 || ANCHO*ALTO>1000)
#warning parametros muy altos, sera dificil de ver la matriz
#endif
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <stdbool.h>
#include <stdio.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <time.h>




void llenar_mat (int mat[ALTO][ANCHO]);//Funcion para llenar matriz
void generacion( int mat[ALTO][ANCHO],int matReferencia[ALTO][ANCHO], unsigned tiempo); //Funcion loop para las generaciones
int numero (void);//Pasar de getchar a int
void copiar(int mat[ALTO][ANCHO],int matReferencia[ALTO][ANCHO]);// Funcion para copiar matriz

// FUNCIONES PARA ALLEGRO 
void must_init(bool test, const char *descrpcion); // Se fija que inicialize bien todo
void destroy_init(bool test,const char *descripcion);
void dibujar_all(int dispAlto, int dispAncho,float lado,ALLEGRO_COLOR color,float platx,float platy,float ballx, float bally); // dibuja las filas y columnas
void dibujar_ball(float cx,float cy,float r);
#endif