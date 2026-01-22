#ifndef FUNCIONES_H
	#define FUNCIONES_H
	#define ANCHO 10
	#define ALTO 10
	#define PI 3.14159265358979
	#define DEGTORAD(deg) ((deg*PI)/180)
	#define MAXTHEMES 3
#if (ANCHO<=0 || ALTO<=0)
	#error valor invalido para matriz
#elif(ANCHO>50 || ALTO>50 || ANCHO*ALTO>1000)
	#warning parametros muy altos, sera dificil de ver la matriz
#endif
#include <allegro5/allegro5.h> // Allegro
#include <allegro5/allegro_font.h> // Inicializa fuente de allegro
#include <stdbool.h> // introduce tipo de dato booleano
#include <stdio.h> // introduce funciones generales
#include <allegro5/allegro_ttf.h> // introudce fuentestipo ttf
#include <allegro5/allegro_image.h> // introduce imagenes
#include <allegro5/allegro_primitives.h> // introduce primitvas para dibujar 
#include <allegro5/allegro_audio.h> // introduce audio
#include <allegro5/allegro_acodec.h> // permite la generacion de musica
#include <time.h> // la usamos para randomizar la semilla
#include <math.h> // introduce funciones trigonometricas

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

typedef struct {
	char * texto;
	float dx,dy;
} modo;

typedef enum {
	POWERUP_INACTIVE,
	POWERUP_FALLING,
	POWERUP_ACTIVE,
} powerupstate;


typedef struct {
	bounding_box bounding;
	float dy;
	char type;
	powerupstate state;
} powerup;


typedef struct { // Se crea la estructura temas
	char * nombre;
	ALLEGRO_COLOR color_texto;
	ALLEGRO_COLOR color_pantalla;
	ALLEGRO_COLOR color_lineas;
	ALLEGRO_COLOR color_casilleros;
} THEMES;

extern int mapas_niveles[3][ALTO][ANCHO];

void llenar_mat (bloque mat[ALTO][ANCHO], int nivel);  //Funcion para llenar matriz
int numero (void);  //Pasar de getchar a int
void copiar (bloque mat[ALTO][ANCHO], bloque matReferencia[ALTO][ANCHO]);  // Funcion para copiar matriz
int recuento_bloques(bloque mat[ANCHO][ALTO]);


// FUNCIONES PARA ALLEGRO //
void must_init (bool test, const char *descrpcion);  // Se fija que inicialize bien todo
void destroy_init (bool test, const char *descripcion);
void dibujar_all (int dispAlto, int dispAncho, float lado, ALLEGRO_COLOR color, bounding_box plat, entities ball, bloque mat[ALTO][ANCHO],ALLEGRO_COLOR color_fondo); // dibuja las filas y columnas
void dibujar_ball (float cx, float cy, float r);
void dibujar_endgame (ALLEGRO_BITMAP* end_screen);
/////////////////////////

// FUNCIONES GAMEPLAY//
bounding_box set_bounding (float ulx, float uly, float  drx, float dry);
char collide (int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2);
void nace (bloque mat[ALTO][ANCHO], int i, int j);
void muere (bloque mat[ALTO][ANCHO], int i, int j);
//////////////////////

#endif