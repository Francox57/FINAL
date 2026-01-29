#include "funciones.h"

int main() {
	srand(time(NULL)); // semilla random para la funcion rand()
	bloque mat[ALTO][ANCHO]; // matriz principal
	option_box matriz_menu[9];
	option_box matriz_pausa[3];

	// INICICIALIZACION //
	must_init(al_init(), "allegro"); // Cada must_init verifica que se haya inicializado cada cosa
	must_init(al_install_keyboard(), "keyboard"); // Instala el uso de teclado
	must_init(al_init_primitives_addon(), "primitivas"); // Instala las primitivas
	must_init(al_init_image_addon(), "imagenes");// Instala el uso de imagenes
	must_init(al_install_mouse(), "mouse"); // Instala el uso de mouse
	must_init(al_install_audio(), "audio"); // Instala el uso de audio
	must_init(al_init_acodec_addon(), "audio codecs");
	
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 4, ALLEGRO_SUGGEST);
	al_set_config_value(al_get_system_config(), "audio", "frequency", "44100");
	
	must_init(al_reserve_samples(16), "reserve samples");
	must_init(al_init_ttf_addon(), "TTF");
	////////////////////////////////

	// INICIALIZACION DE VARIABLES ALLEGRO //
	int disAlto = 720; // Alto de la pantalla
	int disAncho = 720; // Ancho de la pantalla

	ALLEGRO_DISPLAY *disp = al_create_display(disAncho, disAlto); // Puntero que representa al display
	ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0); // Timer
	ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue(); // queue
	ALLEGRO_AUDIO_STREAM *music = al_load_audio_stream("resources/proper_summer.flac", 2, 2048); // musica
	ALLEGRO_FONT *font = al_load_ttf_font("resources/OpenSans.ttf", 15, 0); // fuente
	ALLEGRO_FONT *gameoverfont = al_load_ttf_font("resources/Tiny5-Regular.ttf", 30, 0);
	ALLEGRO_MOUSE_STATE mouse;
	ALLEGRO_BITMAP *imagen_bomba = al_load_bitmap("resources/bombita.png");
	ALLEGRO_SAMPLE *sonido_nave = al_load_sample("resources/rebote.wav");
	ALLEGRO_SAMPLE *sonido_choque = al_load_sample("resources/choque.wav");
	ALLEGRO_SAMPLE *sonido_explosion = al_load_sample("resources/explosion.wav");
	ALLEGRO_SAMPLE *sonido_vida = al_load_sample("resources/power1.wav");
	ALLEGRO_SAMPLE *sonido_azul = al_load_sample("resources/power2.wav");
	ALLEGRO_SAMPLE *sonido_rojo = al_load_sample("resources/power3.wav");
	ALLEGRO_SAMPLE *sonido_nivel = al_load_sample("resources/nivel.wav");
	ALLEGRO_SAMPLE *sonido_menos = al_load_sample("resources/menos.wav");
	ALLEGRO_SAMPLE *sonido_over = al_load_sample("resources/over.wav");
	ALLEGRO_SAMPLE *sonido_nobarra = al_load_sample("resources/nobarra.wav");

	must_init(disp, "display"); // se verifica que estos punteros se hayan inicializado correctamente
	must_init(timer, "timer");
	must_init(queue, "queue");
	must_init(font, "font");
	must_init(gameoverfont, "font");
	must_init(music, "music");
	must_init(imagen_bomba, "imagen de la bomba");
	must_init(sonido_azul, "azul");
	must_init(sonido_choque, "choque");
	must_init(sonido_explosion, "explosion");
	must_init(sonido_menos, "menos");
	must_init(sonido_nave, "nave");
	must_init(sonido_nivel, "nivel");
	must_init(sonido_nobarra, "no barra");
	must_init(sonido_over, "over sound");
	must_init(sonido_rojo, "rojo");
	must_init(sonido_vida, "verde");
	/////////////////////////////////

	///////////// VARIABLES /////////////////
	bool key_left = false; // variable que sirve para ver cuando el usuario mueve la plataforma a la izquierda
	bool key_right = false;// variable que sirve para ver cuando el usuario mueve la plataforma a la derecha
	bool redraw = true; // variable que admite el redibujar la pantalla
	bool cerrar = false; // variable que cierra todo el programa
	bool fuera_mainmenu = false; // variable que permite dibujar la pantalla del juego
	bool pausa = false; // indica si esta en pausa
	bool menu_opciones = false; // indica si esta en el menu de opciones
	bool game_over = false; // indica si termino el juego
	bool nivel_ganado = false; // indica si se gano un nivel
	bool modo_demo = false; // indica si esta en modo demo
	bool super_romper = false;  // instakill del modo demo
	bool aumento = false; // indica si esta alargada la plataforma
	bool bloq_tit = false; // indica si esta titilando la plataforma
	bool estado_bomba = false; // indica si esta la pelota en modo bomba
	bool animacion_go_hecha = false;
	float hit; // variable que sirve para saber en que posicion pego la pelota
	float anchorect; // ancho de los bloques
	float anchoplat; // ancho de la plataforma
	float radio; // radio de la pelota
	float lado; // alto de los bloques
	float angulo = 90; // angulo de la pelota
	float max_angulo = 70; // maximo angulo a la primera salida de la pelota
	int contador = 0; // variable que sirve para determinar que tema usar
	int puntaje = 0; 
	int dice = 3; // dado que decidira cuando sale un poder
	int nivel = 0; // indica  el nivel en el que esta
	int bloques_vivos = 0; // indicara la cantidad de bloques vivos
	int wider_timer = 0; 
	int titileo = 0;
	int radio_explo = 1;
	char vidas = 3;

	// variables galaga //
	Star stars[MAX_STARS];
	bool en_transicion = false;
	int frames_transicion = 0;
	init_stars(stars, disAncho, disAlto);

	ALLEGRO_COLOR galaga_red = al_map_rgb(255, 0, 0);
	ALLEGRO_COLOR galaga_white = al_map_rgb(255, 255, 255);
	ALLEGRO_COLOR galaga_yellow = al_map_rgb(222, 222, 0);
	///////////////////////////////////////////////////

	// INICIALIZACION DE ENTIDADES //
	entities platform;
	entities ball1;

	platform.dx = 5;
	platform.dy = 0;
	ball1.dx = 5;
	ball1.dy = 5;
	ball1.vx = 0;
	ball1.vy = 0;

	matriz_menu[0].texto = "JUGAR";
	matriz_menu[1].texto = "OPCIONES";
	matriz_menu[2].texto = "SALIR";
	matriz_menu[3].texto = "VELOCIDAD:";
	matriz_menu[4].texto = "<";
	matriz_menu[5].texto = ">";
	matriz_menu[6].texto = "ATRAS";
	matriz_menu[7].texto = "<";
	matriz_menu[8].texto = ">";

	matriz_pausa[0].texto = "REANUDAR";
	matriz_pausa[1].texto = "REINICIAR";
	matriz_pausa[2].texto = "VOLVER A MENU";

	powerup extra_life;
	extra_life.bounding = set_bounding(0, 0, 0, 0);
	extra_life.state = POWERUP_INACTIVE;

	powerup wider_platform;
	wider_platform.bounding = set_bounding(0, 0, 0, 0);
	wider_platform.state = POWERUP_INACTIVE;

	powerup bomb;
	bomb.bounding = set_bounding(0, 0, 0, 0);
	bomb.state = POWERUP_INACTIVE;

	powerup powerups_mat[3] = { extra_life, wider_platform, bomb };
	//////////////////////////////////////////////

	// OPCIONES DE COLORES PARA EL JUEGO //
	THEMES dark = { "Dark", al_map_rgb(255, 255, 255), al_map_rgb(0, 0, 0),
			al_map_rgb(192, 192, 192), al_map_rgb(204, 204, 0) };
	THEMES light = { "Light", al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255),
			al_map_rgb(192, 192, 192), al_map_rgb(0, 102, 204) };
	THEMES bluescreen =
			{ "Blue Screen", al_map_rgb(255, 255, 255), al_map_rgb(102, 102,
					255), al_map_rgb(192, 192, 192), al_map_rgb(255, 255, 255) };
	THEMES themeslist[MAXTHEMES] = { dark, light, bluescreen };
	////////////////////////////////////////////////////////////////

	///////////////////// BLOQUE DE REGISTRO DE EVENTOS ///////////////////////////
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_attach_audio_stream_to_mixer(music, al_get_default_mixer()); //inicia el audio
	al_set_audio_stream_playmode(music, ALLEGRO_PLAYMODE_LOOP); //mantiene en loop el audio
	ALLEGRO_EVENT event; // se inicializa la varible evento que determinara que evento sucedio
	// Indica al programa de que fuente pueden venir las señales que manda el usuario //
	////////////////////////////////////////////////////////////////////////////////

	al_start_timer(timer); // empieza el timer

	while (!cerrar) {
		al_get_mouse_state(&mouse);

		// MENU PRINCIPAL //
		if (!fuera_mainmenu && !ball1.vx && !ball1.vy) {
			lado = (disAlto / 2) / ALTO; // cuanto mide los lados verticales de las casillas
			radio = ((2.5 * (float) disAncho) / 255);
			anchorect = (float) disAncho / ANCHO;
			anchoplat = (float) disAncho / 22;
			ball1.x = (float) disAncho / 2;
			ball1.y = disAlto
					- ((disAlto - ((float) disAlto * 0.90))
							+ (6 * anchorect) / 32);
			ball1.bounding = set_bounding(ball1.x - radio, (ball1.y - radio),
					ball1.x + radio, (ball1.y + radio));
			platform.bounding = set_bounding(
					((float) disAncho / 2) - 2.5 * anchoplat,
					disAlto - (disAlto - ((float) disAlto * 0.9)),
					((float) disAncho / 2) + 2.5 * anchoplat,
					disAlto - (disAlto - ((float) disAlto * 0.895) - 16)); // 2.5 COEFICIENTE DE TAMAÑO

			matriz_menu[0].bounding = set_bounding(0.3333 * disAncho,
					disAlto / 3 + lado * 2, disAncho - (disAncho / 3),
					disAlto / 3 + lado * 4);
			matriz_menu[1].bounding = set_bounding(0.3333 * disAncho,
					disAlto / 3 + lado * 4, disAncho - (disAncho / 3),
					disAlto / 3 + lado * 6);
			matriz_menu[2].bounding = set_bounding(0.3333 * disAncho,
					disAlto / 3 + lado * 6, disAncho - (disAncho / 3),
					disAlto / 3 + lado * 8);
			matriz_menu[3].bounding = set_bounding(0.3333 * disAncho,
					disAlto / 8 + lado * 2, disAncho - (disAncho / 3),
					disAlto / 8 + lado * 4);
			matriz_menu[4].bounding = set_bounding(0.3333 * disAncho - 60,
					disAlto / 8 + lado * 4, (disAncho / 3),
					disAlto / 8 + lado * 6);
			matriz_menu[5].bounding = set_bounding(disAncho - (disAncho / 3),
					disAlto / 8 + lado * 4, disAncho - (disAncho / 3 - 60),
					disAlto / 8 + lado * 6);
			matriz_menu[6].bounding = set_bounding(0.3333 * disAncho,
					disAlto / 3 + lado * 10, disAncho - (disAncho / 3),
					disAlto / 3 + lado * 12);
			matriz_menu[7].bounding = set_bounding(0.3333 * disAncho - 60,
					disAlto / 3 + lado * 4, (disAncho / 3),
					disAlto / 3 + lado * 6);
			matriz_menu[8].bounding = set_bounding(disAncho - (disAncho / 3),
					disAlto / 3 + lado * 4, disAncho - (disAncho / 3 - 60),
					disAlto / 3 + lado * 6);

			matriz_pausa[0].bounding = set_bounding(0.3333 * disAncho,
					disAlto / 3 + lado * 2, disAncho - (disAncho / 3),
					disAlto / 3 + lado * 4);
			matriz_pausa[1].bounding = set_bounding(0.3333 * disAncho,
					disAlto / 3 + lado * 4, disAncho - (disAncho / 3),
					disAlto / 3 + lado * 6);
			matriz_pausa[2].bounding = set_bounding(0.25 * disAncho,
					disAlto / 3 + lado * 6, disAncho - (disAncho / 4),
					disAlto / 3 + lado * 8);
		}
		/////////////////////////////////////////////////////////////////////////////////
		al_wait_for_event(queue, &event); // indica al programa que revise si hay eventos
		
		if (event.type == ALLEGRO_EVENT_TIMER) {
			if (en_transicion) { // pantalla animada entre niveles
				frames_transicion++;
				for (int i = 0; i < MAX_STARS; i++) {
					stars[i].y += stars[i].velocidad;
					if (stars[i].y > disAlto) {
						stars[i].y = 0;
						stars[i].x = rand() % disAncho;
					}
				}
				if (frames_transicion > 180) { // transicion de tres segundos
					en_transicion = false;
				}
				redraw = true;
			}
			else {
				if (!fuera_mainmenu && !menu_opciones) { // pantalla principal
					for (int i = 0; i < 3; i++) {
						if (collide(mouse.x, mouse.y, mouse.x, mouse.y,
								matriz_menu[i].bounding.ulx,
								matriz_menu[i].bounding.uly,
								matriz_menu[i].bounding.drx,
								matriz_menu[i].bounding.dry)) {
							al_set_system_mouse_cursor(disp,
									ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
							break;
						} else {
							al_set_system_mouse_cursor(disp,
									ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
						}
					}
				}
				else if (menu_opciones) {
					for (int i = 4; i <= 8; i++) {
						if (collide(mouse.x, mouse.y, mouse.x, mouse.y,
								matriz_menu[i].bounding.ulx,
								matriz_menu[i].bounding.uly,
								matriz_menu[i].bounding.drx,
								matriz_menu[i].bounding.dry)) {
							al_set_system_mouse_cursor(disp, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
							break;
						}
						else {
							al_set_system_mouse_cursor(disp, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
						}
					}
				}
				else if (fuera_mainmenu && !menu_opciones) { // se toco el boton "JUGAR"
					if (pausa) {
						for (int i = 0; i < 3; i++) {
							if (collide(mouse.x, mouse.y, mouse.x, mouse.y,
									matriz_pausa[i].bounding.ulx,
									matriz_pausa[i].bounding.uly,
									matriz_pausa[i].bounding.drx,
									matriz_pausa[i].bounding.dry)) {
								al_set_system_mouse_cursor(disp, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
								break;
							} else {
								al_set_system_mouse_cursor(disp, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
							}
						}
					}
					else {
						al_set_system_mouse_cursor(disp, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
					}
				}
				if (fuera_mainmenu && !pausa && !menu_opciones) { // el usuario empezo a jugar

					if (key_left && platform.bounding.ulx > 0) {
						platform.bounding.ulx -= platform.dx;
						platform.bounding.drx -= platform.dx;

					}
					if (key_right && platform.bounding.drx < disAncho) {
						platform.bounding.ulx += platform.dx;
						platform.bounding.drx += platform.dx;

					}
					if (ball1.vx == 0 && ball1.vy == 0) {
						float centro_paleta = platform.bounding.ulx +
								(platform.bounding.drx - platform.bounding.ulx) / 2;
						ball1.x = centro_paleta;
						ball1.y = platform.bounding.uly - radio - 1;
						ball1.bounding = set_bounding(ball1.x - radio, ball1.y - radio,
								ball1.x + radio, ball1.y + radio);
					}

					// LA PELOTA COLISIONA CON LOS BORDES DE PANTALLA //
					if (collide(ball1.bounding.ulx, ball1.bounding.uly, ball1.bounding.drx,
							ball1.bounding.dry, -10, 0, 0, disAlto)) {
						ball1.vx = fabs(ball1.vx);
						ball1.x = radio + 3;
					}
					if (collide(ball1.bounding.ulx, ball1.bounding.uly, ball1.bounding.drx,
							ball1.bounding.dry, disAncho - 10, 0, disAncho, disAlto)) {
						ball1.vx = -fabs(ball1.vx);
						ball1.x = disAncho - radio - 3;
					}
					if (collide(ball1.bounding.ulx, ball1.bounding.uly, ball1.bounding.drx,
							ball1.bounding.dry, -5, -10, disAncho + 5, 0)) {
						ball1.vy = -ball1.vy;
						ball1.y = radio + 3;
					}
					if (collide(ball1.bounding.ulx, ball1.bounding.uly, ball1.bounding.drx,
							ball1.bounding.dry, -5, disAlto, disAncho + 5, disAlto + 10)) {
						ball1.vx = 0;
						ball1.vy = 0;
						ball1.x = (float) disAncho / 2;
						ball1.y = disAlto - ((disAlto - ((float) disAlto * 0.90)) + (6 * anchorect) / 32);
						ball1.bounding = set_bounding(ball1.x - radio, (ball1.y - radio),
								ball1.x + radio, (ball1.y + radio));
						platform.bounding = set_bounding(((float) disAncho / 2) - 2.5 * anchoplat,
								disAlto - (disAlto - ((float) disAlto * 0.90)),
								((float) disAncho / 2) + 2.5 * anchoplat,
								disAlto - (disAlto - ((float) disAlto * 0.895) - 16));
						vidas--;
						if (vidas >= 1) {
							al_play_sample(sonido_menos, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						}
						else {
							if (!animacion_go_hecha) {
								al_play_sample(sonido_over, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
								animacion_game_over_fade(gameoverfont, puntaje, disAncho, disAlto);
								animacion_go_hecha = true;
								al_flush_event_queue(queue); // se limpia la fila de eventos pendientes
							}
						}
					}
					////////////////////////////////////////////////////
					
					// COLISION ENTRE LA PELOTA Y LA PLATAFORMA //
					char collision_wp = collide(ball1.bounding.ulx,	ball1.bounding.uly, ball1.bounding.drx,
							ball1.bounding.dry, platform.bounding.ulx, platform.bounding.uly,
							platform.bounding.drx, platform.bounding.dry);

					if (collision_wp == 1 && ball1.bounding.dry >= platform.bounding.uly) {
						ball1.y = platform.bounding.uly - radio - 1;
						hit = (ball1.x - platform.bounding.ulx) / (platform.bounding.drx - platform.bounding.ulx);
						angulo = (hit - 0.5) * 2.0 * max_angulo;
						ball1.vx = ball1.dx * sin(DEGTORAD(angulo));
						ball1.vy = -ball1.dy * cos(DEGTORAD(angulo));
						al_play_sample(sonido_nave, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					}
					bool colisiono = false;
					//////////////////////////////////////////////
					
					// BUCLE PARA ANALIZAR CHOQUES EN CADA LADRILLO //
					for (int i = 0; i < ALTO && !colisiono; i++) {
						for (int j = 0; j < ANCHO && !colisiono; j++) {
							char collision_wb = collide(ball1.bounding.ulx, ball1.bounding.uly, ball1.bounding.drx,
									ball1.bounding.dry, mat[i][j].bounding.ulx, mat[i][j].bounding.uly,
									mat[i][j].bounding.drx,	mat[i][j].bounding.dry);
									
							if (collision_wb == -1) {
								if (mat[i][j].cant_impactos_actual >= 1) {
									if (modo_demo && super_romper) {
										for (int di = -radio_explo; di <= radio_explo; di++) {
											for (int dj = -radio_explo; dj <= radio_explo; dj++) {
												int ni = i + di;
												int nj = j + dj;
												if (ni >= 0 && ni < ALTO && nj >= 0 && nj < ANCHO) {
													puntaje += (mat[ni][nj].cant_impactos_actual * 100);
													mat[ni][nj].cant_impactos_actual = 0;
												}
											}
										}
									}
									else if (estado_bomba) {
										for (int di = -radio_explo; di <= radio_explo; di++) {
											for (int dj = -radio_explo; dj <= radio_explo; dj++) {
												int ni = i + di;
												int nj = j + dj;
												if (ni >= 0 && ni < ALTO && nj >= 0 && nj < ANCHO) {
													--mat[ni][nj].cant_impactos_actual;
													puntaje += 100;
												}
											}
										}
										al_play_sample(sonido_explosion, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
										estado_bomba = false;
										powerups_mat[2].state =	POWERUP_INACTIVE;

									}
									else {
										--mat[i][j].cant_impactos_actual;
										puntaje += 100;
										al_play_sample(sonido_choque, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
									}
								}
								if (mat[i][j].estado) {
									ball1.vx = -ball1.vx;
									colisiono = true;
								}
							}
							else if (collision_wb == 1) {
								if (mat[i][j].cant_impactos_actual >= 1) {
									if (modo_demo && super_romper) {
										for (int di = -radio_explo; di <= radio_explo; di++) {
											for (int dj = -radio_explo; dj <= radio_explo; dj++) {
												int ni = i + di;
												int nj = j + dj;
												if (ni >= 0 && ni < ALTO && nj >= 0 && nj < ANCHO) {
													puntaje += (mat[ni][nj].cant_impactos_actual * 100);
													mat[ni][nj].cant_impactos_actual = 0;
													
												}
											}
										}
									}
									else if (estado_bomba) {
										for (int di = -radio_explo; di <= radio_explo; di++) {
											for (int dj = -radio_explo; dj <= radio_explo; dj++) {
												int ni = i + di;
												int nj = j + dj;
												if (ni >= 0 && ni < ALTO && nj >= 0 && nj < ANCHO) {
													--mat[ni][nj].cant_impactos_actual;
													puntaje += 100;
												}
											}
										}
										al_play_sample(sonido_explosion, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
										estado_bomba = false;
										powerups_mat[2].state =	POWERUP_INACTIVE;
									}
									else {
										--mat[i][j].cant_impactos_actual;
										puntaje += 100;
										al_play_sample(sonido_choque, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
									}
								}
								if (mat[i][j].estado) {
									ball1.vy = -ball1.vy;
									colisiono = true;
								}
							}
							else if (collision_wb == 2) {
								if (mat[i][j].cant_impactos_actual >= 1) {
									if (modo_demo && super_romper) {
										for (int di = -radio_explo; di <= radio_explo; di++) {
											for (int dj = -radio_explo; dj <= radio_explo; dj++) {
												int ni = i + di;
												int nj = j + dj;
												if (ni >= 0 && ni < ALTO && nj >= 0 && nj < ANCHO) {
													puntaje += (mat[i][j].cant_impactos_actual * 100);
													mat[ni][nj].cant_impactos_actual = 0;
												}
											}
										}
									}
									else if (estado_bomba) {
										for (int di = -radio_explo; di <= radio_explo; di++) {
											for (int dj = -radio_explo; dj <= radio_explo; dj++) {
												int ni = i + di;
												int nj = j + dj;
												if (ni >= 0 && ni < ALTO && nj >= 0 && nj < ANCHO) {
													--mat[ni][nj].cant_impactos_actual;
													puntaje += 100;
													
												}
											}
										}
										al_play_sample(sonido_explosion, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
										estado_bomba = false;
										powerups_mat[2].state =	POWERUP_INACTIVE;
									}
									else {
										--mat[i][j].cant_impactos_actual;
										puntaje += 100;
										al_play_sample(sonido_choque, 1.0, 0.0,	1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
									}
								}
								if (mat[i][j].estado) {
									ball1.vy = -ball1.vy;
									ball1.vx = -ball1.vx;
									colisiono = true;
								}
							}
							
							// BLOQUE DE APARICION DE POWERUPS //
							if (mat[i][j].cant_impactos_actual < 1 && mat[i][j].estado) {
								mat[i][j].estado = false;
								for (int k = 0; k < 3; k++) {
									// nota: solo puede haber un powerup del mismo tipo activo
									if (!(dice = rand() % 15) && powerups_mat[k].state == POWERUP_INACTIVE) {
										powerups_mat[k].state = POWERUP_FALLING;
										powerups_mat[k].bounding = set_bounding(mat[i][j].bounding.ulx + anchorect / 4,
												mat[i][j].bounding.uly, mat[i][j].bounding.drx - anchorect / 4,
												mat[i][j].bounding.dry);
										powerups_mat[k].dy = 3;
										break;
									}
								}
							}
							/////////////////////////////////////////
						}
					}
					/////////////////////////////////////////////////////////////////
					
					bloques_vivos = recuento_bloques(mat);
					if (bloques_vivos) {
						bloques_vivos = 0; // reinicio la variable
					}
					else {
						nivel_ganado = true;
						bloques_vivos = 0;
					}
					if (nivel_ganado) {
						nivel++;
						al_play_sample(sonido_nivel, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						if (ball1.dx < 10) { // aumenta la velocidad de la pelota
							ball1.dx += 0.5;
							ball1.dy += 0.5;
						}
						printf("NIVEL %d ALCANZADO - Nueva velocidad: %f\n", nivel, ball1.dx);
						llenar_mat(mat, nivel);
						ball1.vx = 0;
						ball1.vy = 0;
						ball1.x = (float) disAncho / 2;
						ball1.y = disAlto - ((disAlto - ((float) disAlto * 0.90)) + (6 * anchorect) / 32);
						ball1.bounding = set_bounding(ball1.x - radio, ball1.y - radio, ball1.x + radio, ball1.y + radio);
						platform.bounding = set_bounding(((float) disAncho / 2) - 2.5 * anchoplat,
								disAlto	- (disAlto - ((float) disAlto * 0.90)),	((float) disAncho / 2) + 2.5 * anchoplat,
								disAlto	- (disAlto - ((float) disAlto * 0.895) - 16));
						nivel_ganado = false;
						en_transicion = true; // modo galaga
						frames_transicion = 0;
						init_stars(stars, disAncho, disAlto);
					}

					ball1.bounding.ulx += ball1.vx;
					ball1.bounding.uly += ball1.vy;
					ball1.bounding.drx += ball1.vx;
					ball1.bounding.dry += ball1.vy;
					ball1.x = ball1.bounding.ulx + radio;
					ball1.y = ball1.bounding.uly - radio;

					for (int p = 0; p < 3; p++) {
						if (powerups_mat[p].state == POWERUP_FALLING) {
							powerups_mat[p].bounding.uly += powerups_mat[p].dy;
							powerups_mat[p].bounding.dry += powerups_mat[p].dy;
							if (powerups_mat[p].bounding.uly > disAlto) {
								powerups_mat[p].state = POWERUP_INACTIVE;
							} else if (collide(platform.bounding.ulx, platform.bounding.uly, platform.bounding.drx,
									platform.bounding.dry, powerups_mat[p].bounding.ulx, powerups_mat[p].bounding.uly,
									powerups_mat[p].bounding.drx, powerups_mat[p].bounding.dry)) {
								powerups_mat[p].state = POWERUP_ACTIVE;
							}
						}
					}

					if (powerups_mat[0].state == POWERUP_ACTIVE) { // powerup de vida extra
						vidas++;
						al_play_sample(sonido_vida, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						powerups_mat[0].state = POWERUP_INACTIVE;
					}
					if (powerups_mat[1].state == POWERUP_ACTIVE) { // powerup que agranda la barra
						wider_timer++;
						if (!aumento) {
							platform.bounding.ulx -= 20;
							platform.bounding.drx += 20;
							aumento = true;
							al_play_sample(sonido_azul, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						}
						if (wider_timer == 480) {
							powerups_mat[1].state = POWERUP_INACTIVE;
							platform.bounding.ulx += 20;
							platform.bounding.drx -= 20;
							aumento = false;
							wider_timer = 0;
							al_play_sample(sonido_nobarra, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						}
					}
					if (powerups_mat[2].state == POWERUP_ACTIVE) { // powerup de bomba
						if (!estado_bomba) {
							al_play_sample(sonido_rojo, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
							estado_bomba = true;
						}
						if (modo_demo && super_romper) {
							estado_bomba = false;
							powerups_mat[2].state = POWERUP_INACTIVE;
						}
					}
					if (vidas == 0) {
						game_over = true;
						nivel = 0;
					}
				}
				al_clear_to_color(themeslist[contador].color_pantalla);
				redraw = true;
			}
		}


		else if ((event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)) { // capta cuando el mouse toca la x de la pantalla
			cerrar = true;
		}
		
		
		else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			if (event.keyboard.keycode == ALLEGRO_KEY_A || event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
				key_left = true;
			}
			if (event.keyboard.keycode == ALLEGRO_KEY_D || event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
				key_right = true;
			}

		}
		
		
		else if (event.type == ALLEGRO_EVENT_KEY_UP) {
			if (event.keyboard.keycode == ALLEGRO_KEY_A || event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
				key_left = false;
			}
			if (event.keyboard.keycode == ALLEGRO_KEY_D || event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
				key_right = false;
			}
		}
		
		
		else if (event.type == ALLEGRO_EVENT_KEY_CHAR) { // se fija si se toca alguna tecla
			if (event.keyboard.keycode == ALLEGRO_KEY_SPACE && fuera_mainmenu && 
					(ball1.vx == 0 && ball1.vy == 0) && !pausa && !menu_opciones) {
				if (game_over) {
					vidas = 3;
					game_over = false;
					nivel = 0;
					llenar_mat(mat, nivel);
					animacion_go_hecha = false;
					puntaje = 0;
					for (int i = 0; i < 3; i++) {
						powerups_mat[i].state = POWERUP_INACTIVE;
					}
					wider_timer = 0;
					estado_bomba = false;
				}
				angulo = (rand() % 100) + 15;
				ball1.vx = (float) ball1.dx * cos(DEGTORAD(angulo));
				ball1.vy = -((float) ball1.dy * sin(DEGTORAD(angulo)));
			}
			else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) { // si se toca escape se pone en pausa o vuelve al menu principal
				if (game_over) { // si el usuario perdio vuelve al menu principal
					vidas = 3;
					pausa = false;
					fuera_mainmenu = false;
					nivel = 0;
					llenar_mat(mat, nivel);
					game_over = false;
					animacion_go_hecha = false;
					puntaje = 0;
					for (int i = 0; i < 3; i++) {
						powerups_mat[i].state = POWERUP_INACTIVE;
					}
					wider_timer = 0;
					estado_bomba = false;
					
				}
				else if (!pausa) {
					pausa = true;
				}
				else {
					pausa = false;
				}
			}

			// MODO DEMO //
			else if (fuera_mainmenu && modo_demo) {
				if (event.keyboard.keycode == ALLEGRO_KEY_1) { // vida extra
					vidas++;
				}
				if (event.keyboard.keycode == ALLEGRO_KEY_2) { // pasar de nivel
					nivel_ganado = true;
					al_play_sample(sonido_nivel, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				if (event.keyboard.keycode == ALLEGRO_KEY_3) { // super romper
					super_romper = !super_romper;
				}
			}
			///////////////
		}
		
		
		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) { // se fija si se toca el boton del mouse
			if (!fuera_mainmenu && !menu_opciones) { // el jugador esta en el menu principal
				if (collide(event.mouse.x, event.mouse.y, event.mouse.x, event.mouse.y, matriz_menu[0].bounding.ulx,
						matriz_menu[0].bounding.uly, matriz_menu[0].bounding.drx, matriz_menu[0].bounding.dry)) { // si clickea en "JUGAR"
					fuera_mainmenu = true;
					for (int i = 0; i < ALTO; i++) {
						for (int j = 0; j < ANCHO; j++) {
							mat[i][j].bounding.ulx = i * (float) disAncho/ANCHO;
							mat[i][j].bounding.uly = j * lado;
							mat[i][j].bounding.drx = i * (float) disAncho / ANCHO + disAlto / ALTO;
							mat[i][j].bounding.dry = j * lado + disAncho / ANCHO;
							mat[i][j].bounding = set_bounding( j * (float) disAncho / ANCHO, (i * lado) + 3 * lado,
									j * (float) disAncho / ANCHO + (float) disAncho / ANCHO, i * lado + 4 * lado);
						}
					}
					llenar_mat(mat, 0);
				} 
				else if (collide(event.mouse.x, event.mouse.y, event.mouse.x, event.mouse.y, matriz_menu[1].bounding.ulx, // si clickea en "OPCIONES"
						matriz_menu[1].bounding.uly, matriz_menu[1].bounding.drx, matriz_menu[1].bounding.dry)) { 
					menu_opciones = true;
				}
				else if (collide(event.mouse.x, event.mouse.y, event.mouse.x, event.mouse.y, matriz_menu[2].bounding.ulx, // si clickea en "SALIR"
						matriz_menu[2].bounding.uly, matriz_menu[2].bounding.drx, matriz_menu[2].bounding.dry)) {
					cerrar = true;
				}
			}
			else if (pausa) {
				if (collide(event.mouse.x, event.mouse.y, event.mouse.x, event.mouse.y, matriz_pausa[0].bounding.ulx, // si clickea en "REANUDAR"
						matriz_pausa[0].bounding.uly, matriz_pausa[0].bounding.drx, matriz_pausa[0].bounding.dry)) {
					pausa = false;
				}
				else if (collide(event.mouse.x, event.mouse.y, event.mouse.x, event.mouse.y, matriz_pausa[1].bounding.ulx, // si clickea en "REINICIAR"
						matriz_pausa[1].bounding.uly, matriz_pausa[1].bounding.drx, matriz_pausa[1].bounding.dry)) {
					
					// se vuelve al estado inicial
					pausa = false;
					vidas = 3;
					puntaje = 0;
					nivel = 0;
					animacion_go_hecha = false;
					llenar_mat(mat, nivel);
					for (int i = 0; i < 3; i++) {
						powerups_mat[i].state = POWERUP_INACTIVE;
					}
					wider_timer = 0;
					estado_bomba = false;
					ball1.vx = 0;
					ball1.vy = 0;
					ball1.x = (float) disAncho / 2;
					ball1.y = disAlto - ((disAlto - ((float) disAlto * 0.90)) + (6 * anchorect) / 32);
					ball1.dx = 5;
					ball1.dy = 5;
					ball1.bounding = set_bounding(ball1.x - radio, (ball1.y - radio), ball1.x + radio, (ball1.y + radio));
					platform.bounding = set_bounding(((float) disAncho / 2) - 2.5 * anchoplat, disAlto - (disAlto - ((float) disAlto * 0.90)),
							((float) disAncho / 2) + 2.5 * anchoplat, disAlto - (disAlto - ((float) disAlto * 0.90)	- 16));
				}
				else if (collide(event.mouse.x, event.mouse.y, event.mouse.x, event.mouse.y, matriz_pausa[2].bounding.ulx,
						matriz_pausa[2].bounding.uly, matriz_pausa[2].bounding.drx, matriz_pausa[2].bounding.dry)) { // si clickea en "VOLVER A MENU"

					// se vuelve al estado inicial y se habilita la variable del menu principal
					vidas = 3;
					pausa = false;
					nivel = 0;
					llenar_mat(mat, nivel);
					animacion_go_hecha = false;
					for (int i = 0; i < 3; i++) {
						powerups_mat[i].state = POWERUP_INACTIVE;
					}
					wider_timer = 0;
					estado_bomba = false;
					ball1.vx = 0;
					ball1.vy = 0;
					ball1.x = (float) disAncho / 2;
					ball1.y = disAlto - ((disAlto - ((float) disAlto * 0.90)) + (6 * anchorect) / 32);
					ball1.bounding = set_bounding(ball1.x - radio, (ball1.y - radio), ball1.x + radio, (ball1.y + radio));
					platform.bounding = set_bounding(((float) disAncho / 2) - 2.5 * anchoplat, disAlto - (disAlto - ((float) disAlto * 0.90)), 
							((float) disAncho / 2) + 2.5 * anchoplat, disAlto - (disAlto - ((float) disAlto * 0.90) - 16));
					fuera_mainmenu = false;
				}
			}
			else if (menu_opciones) {
				// zona de temas
				if (event.mouse.y > 200 && event.mouse.y < 260) {
					// Click a la izquierda (Flecha <)
					if (event.mouse.x < disAncho / 2) {
						if (contador == 0) {
							contador = 2;
						}
						else {
							contador--;
						}
					}
					// Click a la derecha (Flecha >)
					else {
						if (contador == 2) {
							contador = 0;
						}
						else {
							contador++;
						}
					}
				}
				// zona de velocidad
				if (event.mouse.y > 320 && event.mouse.y < 380) {
					// Click a la izquierda (Menos velocidad)
					if (event.mouse.x < disAncho / 2) {
						if (ball1.dx > 4.0) {
							ball1.dx -= 1.0;
						}
					}
					// Click a la derecha (Mas velocidad)
					else {
						if (ball1.dx < 10.0) {
							ball1.dx += 1.0;
						}
					}
				}

				// zona de modo demo
				if (event.mouse.y > 440 && event.mouse.y < 500) {
					// Click en todo el ancho central
					if (event.mouse.x > disAncho / 2 - 250 && event.mouse.x < disAncho / 2 + 250) {
						modo_demo = !modo_demo;
						if (!modo_demo) {
							super_romper = false;
						}
					}
				}

				//  zona del boton "ATRAS"
				if (event.mouse.y > 580 && event.mouse.y < 630) {
					if (event.mouse.x > disAncho / 2 - 100 && event.mouse.x < disAncho / 2 + 100) {
						menu_opciones = false;
					}
				}
			}
		}


		else if (event.type == ALLEGRO_EVENT_MOUSE_AXES && event.mouse.dz != 0 && !fuera_mainmenu) { // se fija si se movio la rueda del mouse y si esta en el menu
			if (event.mouse.dz > 0) { // Se fija que el cambio del mouse scroll sea para arriba
				disAlto += 10;
				disAncho += 10;
				redraw = true;
			}
			else if (event.mouse.dz < 0) { // Se fija que el cambio del mouse scroll sea para abajo	
				disAlto -= 10;
				disAncho -= 10;
				redraw = true;
			}
			if (disAlto < 720) { // impide que el display sea muy pequeño para que las letras entren
				disAlto = 720;
			}
			if (disAncho < 720) { // impide que el display sea muy pequeño para que las letras entren
				disAncho = 720;
			}
			al_resize_display(disp, disAncho, disAlto); // Cambia tamaño de pantalla
			al_clear_to_color(themeslist[contador].color_pantalla); // limpia la pantalla para que no se escriba texto por encima
		}
		if (redraw && al_is_event_queue_empty(queue)) { // si esta para redibujar

			al_clear_to_color(themeslist[contador].color_pantalla); // limpieza general por seguridad

			if (fuera_mainmenu && !menu_opciones) { // si no esta en el menu principal ni en el menu opciones

				if (en_transicion) { // dibuja transicion galaga 
					al_clear_to_color(al_map_rgb(0, 0, 0)); // Fondo negro

					for (int i = 0; i < MAX_STARS; i++) {
						al_draw_filled_rectangle(stars[i].x, stars[i].y, stars[i].x + stars[i].tamano,
								stars[i].y + stars[i].tamano, al_map_rgb(255, 255, 255));
					}
					if (frames_transicion % 40 < 30) { // texto parpadeante
						al_draw_textf(gameoverfont, al_map_rgb(0, 255, 255), disAncho / 2, disAlto / 2, ALLEGRO_ALIGN_CENTER,
								"STAGE %d", nivel + 1);
						al_draw_text(font, al_map_rgb(255, 0, 0), disAncho / 2, disAlto / 2 + 40, ALLEGRO_ALIGN_CENTER, "READY");
					}
					al_flip_display();
					redraw = false;
				}
				else { // dibuja el juego normal
					al_clear_to_color(themeslist[contador].color_pantalla);

					if (wider_timer >= 300 && !bloq_tit) {
						dibujar_all(disAlto, disAncho, lado, al_map_rgb(255, 0, 0), platform.bounding, ball1,
								mat, themeslist[contador].color_pantalla, imagen_bomba, estado_bomba);
						titileo++;
						if (titileo == 20) {
							bloq_tit = true;
						}
					}
					else {
						dibujar_all(disAlto, disAncho, lado, themeslist[contador].color_lineas, platform.bounding, ball1, mat,
								themeslist[contador].color_pantalla, imagen_bomba, estado_bomba);
						if (titileo >= 0) { // para evitar que la variable tome valores negativos
							titileo--;
							if (titileo == 0) {
								bloq_tit = false;
							}
						}
					}
					dibujar_powerups(powerups_mat);

					if (pausa) {
						al_draw_text(gameoverfont, themeslist[contador].color_texto, disAncho / 2,
								matriz_menu[0].bounding.dry - (matriz_menu[0].bounding.dry - matriz_menu[0].bounding.uly) * 0.75,
								ALLEGRO_ALIGN_CENTER, matriz_pausa[0].texto);
						al_draw_text(gameoverfont, themeslist[contador].color_texto, disAncho / 2,
								matriz_menu[1].bounding.dry - (matriz_menu[1].bounding.dry - matriz_menu[1].bounding.uly) * 0.75,
								ALLEGRO_ALIGN_CENTER, matriz_pausa[1].texto);
						al_draw_text(gameoverfont, themeslist[contador].color_texto, disAncho / 2,
								matriz_menu[2].bounding.dry - (matriz_menu[2].bounding.dry - matriz_menu[2].bounding.uly) * 0.75,
								ALLEGRO_ALIGN_CENTER, matriz_pausa[2].texto);
					}

					if (game_over) { 

						// recuadro negro semitransparente de fondo
						al_draw_filled_rectangle(0, disAlto / 3, disAncho, disAlto * 2 / 3, al_map_rgba(0, 0, 0, 200));

						// Titulo GAME OVER en Rojo
						al_draw_text(gameoverfont, al_map_rgb(255, 0, 0), disAncho / 2, disAlto / 2 - 50,
								ALLEGRO_ALIGN_CENTER, "GAME OVER");

						// PUNTAJE 
						al_draw_textf(gameoverfont, al_map_rgb(255, 255, 255), disAncho / 2, disAlto / 2, ALLEGRO_ALIGN_CENTER,
								"PUNTAJE FINAL: %d", puntaje);

						// instrucciones
						al_draw_text(gameoverfont, al_map_rgb(255, 255, 0), disAncho / 2, disAlto / 2 + 60, ALLEGRO_ALIGN_CENTER,
								"[ESPACIO] Reiniciar    -    [ESC] Menu Principal");
					}

					// HUD
					al_draw_text(gameoverfont, galaga_red, 10, disAlto - 30, ALLEGRO_ALIGN_LEFT, "LIVES");
					for (int i = 0; i < vidas; i++) {
						float bx = 100 + (i * 25);
						float by = disAlto - 5;
						al_draw_filled_triangle(bx, by, bx + 8, by - 15, bx + 16, by, galaga_yellow);
					}
					al_draw_text(gameoverfont, galaga_red, disAncho / 2, 5, ALLEGRO_ALIGN_CENTER, "SCORE");
					al_draw_textf(gameoverfont, galaga_white, disAncho / 2, 30, ALLEGRO_ALIGN_CENTER, "%d", puntaje);
					if (modo_demo) {
						al_draw_text(font, al_map_rgb(255, 255, 0), disAncho / 2, 60, ALLEGRO_ALIGN_CENTER, "--- MODO DEMO ---");
						al_draw_text(font, al_map_rgb(255, 255, 0), disAncho / 2, 80, ALLEGRO_ALIGN_CENTER, "1:Vida | 2:Nivel | 3:InstaKill");
						if (super_romper) {
							al_draw_text(gameoverfont, al_map_rgb(255, 0, 0), disAncho / 2, disAlto / 2 + 50,
									ALLEGRO_ALIGN_CENTER, "!! SUPER KILL !!");
						}
					}
					al_flip_display();
					redraw = false;
				}

			}
			else if (!fuera_mainmenu) { // menu principal
				al_draw_text(gameoverfont, galaga_yellow, disAncho / 2, 100, ALLEGRO_ALIGN_CENTER, "BREAKOUT 8-BIT");
				if (menu_opciones) {
					al_draw_text(gameoverfont, themeslist[contador].color_texto, disAncho / 2, 200, ALLEGRO_ALIGN_CENTER, "TEMA:");
					al_draw_text(gameoverfont, themeslist[contador].color_texto, disAncho / 2 - 160, 230, ALLEGRO_ALIGN_CENTER, "<");
					al_draw_text(gameoverfont, themeslist[contador].color_texto,
							disAncho / 2, 230, ALLEGRO_ALIGN_CENTER, themeslist[contador].nombre);
					al_draw_text(gameoverfont, themeslist[contador].color_texto, disAncho / 2 + 160, 230, ALLEGRO_ALIGN_CENTER, ">");
					al_draw_text(gameoverfont, themeslist[contador].color_texto, disAncho / 2, 320, ALLEGRO_ALIGN_CENTER, "VELOCIDAD:");
					al_draw_text(gameoverfont, themeslist[contador].color_texto, disAncho / 2 - 160, 350, ALLEGRO_ALIGN_CENTER, "<");
					al_draw_textf(gameoverfont, themeslist[contador].color_texto, disAncho / 2, 350,
							ALLEGRO_ALIGN_CENTER, "%.1f", ball1.dx);
					al_draw_text(gameoverfont, themeslist[contador].color_texto, disAncho / 2 + 160, 350, ALLEGRO_ALIGN_CENTER, ">");
					ALLEGRO_COLOR color_demo = modo_demo ? al_map_rgb(0, 255, 0) : al_map_rgb(255, 0, 0);
					al_draw_text(gameoverfont, themeslist[contador].color_texto, disAncho / 2, 440, ALLEGRO_ALIGN_CENTER, "MODO DEMO:");
					al_draw_text(gameoverfont, themeslist[contador].color_texto, disAncho / 2 - 160, 470, ALLEGRO_ALIGN_CENTER, "<");
					al_draw_text(gameoverfont, color_demo, disAncho / 2, 470, ALLEGRO_ALIGN_CENTER, modo_demo ? "ACTIVADO" : "DESACTIVADO");
					al_draw_text(gameoverfont, themeslist[contador].color_texto, disAncho / 2 + 160, 470, ALLEGRO_ALIGN_CENTER, ">");
					al_draw_text(gameoverfont, themeslist[contador].color_texto, disAncho / 2, 600, ALLEGRO_ALIGN_CENTER, "ATRAS");
				}
				else { // botones del menu principal
					al_draw_text(gameoverfont, themeslist[contador].color_texto, disAncho / 2,
							matriz_menu[0].bounding.dry - (matriz_menu[0].bounding.dry - matriz_menu[0].bounding.uly) * 0.75,
							ALLEGRO_ALIGN_CENTER, matriz_menu[0].texto);
					al_draw_text(gameoverfont, themeslist[contador].color_texto, disAncho / 2,
							matriz_menu[1].bounding.dry - (matriz_menu[1].bounding.dry - matriz_menu[1].bounding.uly) * 0.75,
							ALLEGRO_ALIGN_CENTER, matriz_menu[1].texto);
					al_draw_text(gameoverfont, themeslist[contador].color_texto, disAncho / 2,
							matriz_menu[2].bounding.dry - (matriz_menu[2].bounding.dry - matriz_menu[2].bounding.uly) * 0.75,
							ALLEGRO_ALIGN_CENTER, matriz_menu[2].texto);
				}
				al_flip_display();
				redraw = false;
			}
		}
	}
	
	// Destruye todo lo que se uso de allegro
	printf("Destruyendo la fuente...\n");
	al_destroy_font(font);
	printf("Destruyendo el sonido...\n");
	al_destroy_display(disp);
	printf("Destruyendo el timer...\n");
	al_destroy_timer(timer);
	printf("Destruyendo los eventos...\n");
	al_destroy_event_queue(queue);
	printf("Destruyendo la musica...\n");
	al_destroy_bitmap(imagen_bomba);
	printf("Destruyendo imagen...\n");
	al_destroy_sample(sonido_nave);
	al_destroy_sample(sonido_choque);
	al_destroy_sample(sonido_explosion);
	al_destroy_sample(sonido_vida);
	al_destroy_sample(sonido_azul);
	al_destroy_sample(sonido_rojo);
	al_destroy_sample(sonido_nivel);
	al_destroy_sample(sonido_menos);
	al_destroy_sample(sonido_over);
	al_destroy_sample(sonido_nobarra);
	printf("Destruyendo la musica...\n");
	return 0;
}
