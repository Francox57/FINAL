#include "funciones.h"

void mat_copia(bloque copia[3][3], int i, int j); // Funcion que copia a la matriz referencia en un 3x3
void nace (bloque mat[ALTO][ANCHO], int i, int j);//Funcion que crea nuevos bloques
void muere (bloque mat[ALTO][ANCHO], int i, int j);//Funcion que mata las anteriores
void print_mat (bloque mat[ALTO][ANCHO]);//Funcion para imprimir la matriz

void llenar_mat (bloque mat[ALTO][ANCHO]) {
	
	int i,j;  //Valor de fila y columna
	for(i=0;i<ALTO;i++) {
		
		for(j=0;j<ANCHO;j++) {
			
			mat[i][j].estado = true;  //llena la matriz de bloques vivos
		}
	}
}


void nace (bloque mat[ALTO][ANCHO], int i, int j) {                     
	
	mat[i][j].estado = true;
}


void muere (bloque mat[ALTO][ANCHO], int i, int j) {
	
	mat[i][j].estado = false;
}


void print_mat (bloque mat[ALTO][ANCHO]) { //Funcion para imprimir matriz
	
	int i,j;  //Valor de fila y columna
	int cont = 0;  //Inicio un contador que va a tener el valor de los indices en 0
	printf("|^_^|");
	while (cont<ANCHO) {  //Imprime una linea con los valores de las celdas
		
		printf("%2d |", cont);
		cont++;
	}
	putchar('\n');
	cont = 0;
	for (i=0;i<ALTO;i++) {
		
		putchar('|');
		printf("%2d |", cont);  //Imprime una fila con los valores de las celdas
		cont++;
		for (j=0;j<ANCHO;j++) {
			
			printf(" %d |", mat[i][j].estado);  //Imprime el caracter de vivo o muerto
		}
		putchar('\n');
	}
}

// FUNCIONES CON ALLEGRO //

void must_init(bool test, const char *descripcion) {  // Se asegura que todo se inicialize bien

	if(test) return; // Si se inicializo bien deja seguir el programa
	printf("No se pudo inicializar %s\n", descripcion);
	exit(1); // Si no se sale del programa y avisa cual no se inicializo correctamente
}


void dibujar_all(int dispAlto, int dispAncho, float lado, ALLEGRO_COLOR color, bounding_box plat, entities ball, bloque mat[ALTO][ANCHO]) { // Dibuja las filas y columnas calculando el offset
	
	int i,j;
	for (i = 0; i < ALTO; i++) { // Recorre que tan ancho es la matriz
		
		for (j = 0; j < ANCHO; j++){     
			
			if (mat[i][j].estado == true) {
				
				al_draw_rectangle(mat[i][j].bounding.ulx, mat[i][j].bounding.uly, mat[i][j].bounding.drx, mat[i][j].bounding.dry, color, 1);     
			}
		}
	}
	float radio = ((2.5*(float)dispAncho)/255);
	al_draw_filled_rectangle(plat.ulx, plat.uly, plat.drx, plat.dry, color); //dibuja la plataforma
	al_draw_filled_circle(ball.x,ball.y+2*radio,radio,color); //dibuja la pelota
	//al_draw_rectangle(ball.bounding.ulx,ball.bounding.uly,ball.bounding.drx,ball.bounding.dry,al_map_rgb(255, 92, 194),1);
	//al_draw_rectangle(plat.ulx,plat.uly,plat.drx,plat.dry,al_map_rgb(255, 92, 194),1); //dibujan las hitboxes de ambos elementos
}


bounding_box set_bounding(float ulx ,float uly ,float  drx ,float dry) {
	bounding_box hitbox  = {ulx,uly,drx,dry};
	return hitbox;
}


char collide (int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2) {

	if (ax1 >= bx2) return 0;
	if (ax2 <= bx1) return 0;  //casos en los cuales los dos bloques no se tocan
	if (ay1 >= by2) return 0;
	if (ay2 <= by1) return 0;
	float overlapX = (ax2 < bx2 ? ax2 : bx2) - (ax1 > bx1 ? ax1 : bx1);  // superposicion de ambos cuerpos en vista 
	float overlapY = (ay2 < by2 ? ay2 : by2) - (ay1 > by1 ? ay1 : by1);  // horizontal y vertical
	if (overlapX < overlapY) {  // en caso de que sea vertical
		return -1;
	}
	else if (overlapX > overlapY) {  // caso horizontal
		return 1;
	}
	else {  // la pelota choca justo con una de las esquinas del bloque
		return 2;
	}
}