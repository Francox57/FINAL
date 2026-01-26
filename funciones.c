#include "funciones.h"


void must_init(bool test, const char *descripcion) { // verifica que el argumento inicie bien
	if(test) return; // si se inicializo bien deja seguir el programa
	printf("No se pudo inicializar %s\n", descripcion);
	exit(1); // si falla, cierra el programa y avisa que cosa fallo
}


bounding_box set_bounding(float ulx ,float uly ,float  drx ,float dry) {
	bounding_box hitbox  = {ulx,uly,drx,dry};
	return hitbox;
}


void init_stars(Star stars[], int ancho, int alto) {
	for (int i = 0; i < MAX_STARS; i++) {
		stars[i].x = rand() % ancho;
		stars[i].y = rand() % alto;
		stars[i].velocidad = (rand() % 5) + 1;
		stars[i].tamano = (rand() % 3) + 1;
	}
}


void llenar_mat (bloque mat[ALTO][ANCHO] , int nivel) {
	int i,j;
	srand(time(NULL));
	printf("%d",nivel);
	if (nivel > 2) {
		for (i = 0 ; i < ALTO ; i++) {
			for (j = 0 ; j < ANCHO ; j++) {
				mat[i][j].cant_impactos_total = (rand() % 5)+1;
				mat[i][j].cant_impactos_actual = mat[i][j].cant_impactos_total;
				if (mat[i][j].cant_impactos_actual > 0) {
					mat[i][j].estado = true;
				}
				if (mat[i][j].cant_impactos_actual == 0) {
					mat[i][j].estado = false;
				}
			}
		}
		return;
	}else{
		for (i = 0 ; i < ALTO ; i++) {
			for (j = 0 ; j < ANCHO ; j++) {
				mat[i][j].cant_impactos_total = mapas_niveles[nivel][i][j];
				mat[i][j].cant_impactos_actual = mat[i][j].cant_impactos_total;
				if (mat[i][j].cant_impactos_actual > 0) {
					mat[i][j].estado = true;
				}
				if (mat[i][j].cant_impactos_actual == 0) {
					mat[i][j].estado = false;
				}
			}
		}
	}
}


void dibujar_all (int dispAlto, int dispAncho, float lado, ALLEGRO_COLOR color, bounding_box plat, entities ball, bloque mat[ALTO][ANCHO],ALLEGRO_COLOR color_fondo, ALLEGRO_BITMAP* sprite, bool estado_bomba) {
	int i,j;
	ALLEGRO_COLOR color_bloque;
	for (i = 0; i < ALTO; i++) {                                    
		for (j = 0; j < ANCHO; j++) {     
			if (mat[i][j].estado == true) {
				switch (mat[i][j].cant_impactos_actual) {
					case 1: color_bloque = al_map_rgb(0, 224, 0); break; // verde
					case 2: color_bloque = al_map_rgb(224, 224, 0); break; // amarillo
					case 3: color_bloque = al_map_rgb(0, 100, 255); break; // azul
					case 4: color_bloque = al_map_rgb(224, 0, 224); break; // violeta
					case 5: color_bloque = al_map_rgb(255, 0, 0); break; // rojo
					default: color_bloque = al_map_rgb(128, 128, 128); break; // gris
				}
				al_draw_filled_rectangle(mat[i][j].bounding.ulx, mat[i][j].bounding.uly, mat[i][j].bounding.drx, mat[i][j].bounding.dry, color_bloque);
				al_draw_rectangle(mat[i][j].bounding.ulx, mat[i][j].bounding.uly, mat[i][j].bounding.drx, mat[i][j].bounding.dry, color_fondo,2);         
			}
		}
	}
	al_draw_filled_rectangle(plat.ulx, plat.uly, plat.drx, plat.dry, color); //dibuja la plataforma
	float radio = ((2.5*(float)dispAncho)/255);
	
	if(estado_bomba) {
		int w = al_get_bitmap_width(sprite);
		int h = al_get_bitmap_height(sprite);
		float bomb_size = radio * 2.5;
		al_draw_scaled_bitmap(sprite, 0, 0, w, h, ball.x - (bomb_size/2), ball.y - (bomb_size/2), bomb_size, bomb_size, 0);
	}
	else {
		al_draw_filled_circle(ball.x,ball.y+2*radio,radio,color); //dibuja la pelota
	}	
}


void dibujar_powerups(powerup powerups_mat[3]) {
	for (int i = 0 ; i < 3 ; i++) {
		if (powerups_mat[i].state == POWERUP_FALLING) {
			ALLEGRO_COLOR color_powerup;
			switch (i) {
				case 0: color_powerup = al_map_rgb(0, 255, 0); break;
				case 1: color_powerup = al_map_rgb(0, 0, 255); break;
				default: color_powerup = al_map_rgb(255, 0, 0); break;
			}
			float radio = (powerups_mat[i].bounding.drx - powerups_mat[i].bounding.ulx) / 2.0;
			float cx = powerups_mat[i].bounding.ulx + radio;
			float cy = powerups_mat[i].bounding.uly + radio;
			al_draw_filled_circle(cx, cy, radio, color_powerup);
			al_draw_circle(cx, cy, radio, al_map_rgb(255, 255, 255), 2);
		}
	}
	
}


void animacion_game_over_fade(ALLEGRO_FONT *fuente, int puntaje, int ancho, int alto) {
	// Bucle para el efecto de "Fade In" (Aparecer suavemente)
	for (float alpha = 0.0; alpha <= 1.0; alpha += 0.005) { // 0.005 es la velocidad
		al_clear_to_color(al_map_rgb(0, 0, 0)); // Fondo negro

		// Color rojo que va apareciendo (alpha aumenta)
		ALLEGRO_COLOR color_fade = al_map_rgba_f(1.0, 0.0, 0.0, alpha);
		ALLEGRO_COLOR color_blanco_fade = al_map_rgba_f(1.0, 1.0, 1.0, alpha);

		// Texto GAME OVER
		al_draw_text(fuente, color_fade, ancho / 2, alto / 2 - 50,
				ALLEGRO_ALIGN_CENTER, "GAME OVER");

		// Texto del PUNTAJE
		al_draw_textf(fuente, color_blanco_fade, ancho / 2, alto / 2 + 20,
				ALLEGRO_ALIGN_CENTER, "TU PUNTAJE: %d", puntaje);

		al_flip_display();
		al_rest(0.01); // Pequeña pausa para que la animación sea fluida
	}

	al_rest(1.0); // Esperar 1 segundo con el texto en pantalla antes de mostrar los botones
}


int recuento_bloques(bloque mat[ANCHO][ALTO]) {
	int vivos = 0;
	for (int i = 0 ; i < ALTO ; i++) {
		for (int j = 0 ; j < ANCHO ; j++) {
			if (mat[i][j].estado && mat[i][j].cant_impactos_actual > 0) {
				vivos++;
			}
		}
	}
	return vivos;
}


char collide (int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2) {

	// situaciones en las que los dos cuerpos no chocan entre si
	if(ax1 >= bx2) return 0;
	if(ax2 <= bx1) return 0;
	if(ay1 >= by2) return 0;
	if(ay2 <= by1) return 0;
	   
	float overlapX = (ax2 < bx2 ? ax2 : bx2) - (ax1 > bx1 ? ax1 : bx1); // se fija en que direccion 
	float overlapY = (ay2 < by2 ? ay2 : by2) - (ay1 > by1 ? ay1 : by1); // colisiono mas
	if (overlapX < overlapY) { // en caso de que sea vertical
		return -1;
	}
	else if (overlapX > overlapY) {
		return 1;// caso horizontal
	}
	else { 
		return 2;
	}
}