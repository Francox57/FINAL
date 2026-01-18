#ifndef FUNCIONES_H
	#define FUNCIONES_H
	#define ANCHO 10
	#define ALTO 10
	#define PI 3.14159265358979
	#define DEGTORAD(deg) ((deg*PI)/180)
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
#include <math.h>


typedef struct {
	float ulx,uly,drx,dry;
} bounding_box;


typedef struct {
	float x,y;
	float dx,dy;
	float vx,vy;
	bounding_box bounding;
} entities;


typedef struct {
	bool estado;
	char cant_impactos_actual;
	char cant_impactos_total;
	bounding_box bounding;
} bloque;


typedef struct {
	bounding_box bounding;
	char * texto;
} option_box;


void llenar_mat (bloque mat[ALTO][ANCHO]);  //Funcion para llenar matriz
int numero (void);  //Pasar de getchar a int
void copiar (bloque mat[ALTO][ANCHO], bloque matReferencia[ALTO][ANCHO]);  // Funcion para copiar matriz


// FUNCIONES PARA ALLEGRO 
void must_init (bool test, const char *descrpcion);  // Se fija que inicialize bien todo
void destroy_init (bool test, const char *descripcion);
void dibujar_all (int dispAlto, int dispAncho, float lado, ALLEGRO_COLOR color, bounding_box plat, entities ball, bloque mat[ALTO][ANCHO]); // dibuja las filas y columnas
void dibujar_ball (float cx, float cy, float r);
void dibujar_endgame (ALLEGRO_BITMAP* end_screen);
// FUNCIONES GAMEPLAY


bounding_box set_bounding (float ulx, float uly, float  drx, float dry);
char collide (int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2);
void nace (bloque mat[ALTO][ANCHO], int i, int j);
void muere (bloque mat[ALTO][ANCHO], int i, int j);
#endif