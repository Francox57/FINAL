#ifndef FUNCIONES_H
	#define FUNCIONES_H
	#define ANCHO 10
	#define ALTO 10
	#define PI 3.14159265358979
	#define DEGTORAD(deg) ((deg*PI)/180)
	#define MAXTHEMES 3
	#define MAX_STARS 100
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


// ESTRUCTURAS //
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

typedef struct {
	float x, y, velocidad;
	int tamano;
} Star;

typedef struct { // Se crea la estructura temas
	char * nombre;
	ALLEGRO_COLOR color_texto;
	ALLEGRO_COLOR color_pantalla;
	ALLEGRO_COLOR color_lineas;
	ALLEGRO_COLOR color_casilleros;
} THEMES;
///////////////////////////////


extern int mapas_niveles[3][ALTO][ANCHO];


// PROTOTIPOS DE FUNCIONES //
void must_init (bool test, const char *descrpcion);
bounding_box set_bounding (float ulx, float uly, float  drx, float dry);
void init_stars (Star stars[], int ancho, int alto);
void llenar_mat (bloque mat[ALTO][ANCHO], int nivel);
void dibujar_all (int dispAlto, int dispAncho, float lado, ALLEGRO_COLOR color, bounding_box plat, entities ball, bloque mat[ALTO][ANCHO],ALLEGRO_COLOR color_fondo, ALLEGRO_BITMAP* sprite, bool estado_bomba);
void dibujar_powerups(powerup powerups_mat[3]);
void animacion_game_over_fade(ALLEGRO_FONT *fuente, int puntaje, int ancho, int alto);
void restart_settings(void);
int recuento_bloques(bloque mat[ANCHO][ALTO]);
void spawn_powerups();
char collide (int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2);
/////////////////////////////
#endif