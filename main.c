#include "funciones.h"

int main() {
	srand(time(NULL)); // semilla random para la funcion rand()
	bloque mat[ALTO][ANCHO]; // matriz principal
	option_box matriz_menu[9];
	option_box matriz_pausa[3];

	// INICICIALIZACION //
	must_init(al_init(), "allegro"); // Cada must_init verifica que se haya inicializado cada cosa
	must_init(al_install_keyboard(), "keyboard");
	must_init(al_init_primitives_addon(), "primitivas");
	must_init(al_install_mouse(), "mouse");
	must_init(al_install_audio(), "audio");
	must_init(al_init_acodec_addon(), "audio codecs");
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
	ALLEGRO_FONT *gameoverfont = al_load_ttf_font("resources/Tiny5-Regular.ttf",30, 0);
	ALLEGRO_MOUSE_STATE mouse;
	
	must_init(disp, "display"); // se verifica que estos punteros se hayan inicializado correctamente
	must_init(timer, "timer");
	must_init(queue, "queue");
	must_init(font, "font");
	must_init(gameoverfont, "font");
	must_init(music, "music");
	/////////////////////////////////
	
	///////////// VARIABLES /////////////////
	bool key_left = false;
	bool key_right = false;
	bool redraw = true; // variable que admite el redibujar la pantalla
	bool cerrar = false; // variable que cierra todo el programa
	bool fuera_mainmenu = false; // variable que permite dibujar la pantalla del juego
	bool pausa = false;
	bool menu_opciones = false;
	bool game_over = false;
	bool nivel_ganado = false;
	bool modo_demo = false;     // Activa los trucos
	bool super_romper = false;  // Truco de la tecla 3 (Romper de una)
	float hit;
	float anchorect;
	float anchoplat;
	float radio;
	float lado;
	float angulo = 90;
	float max_angulo = 70;
	int contador = 0; // variable que sirve para determinar que tema usar
	int puntaje = 0;
	int dice = 3;
	int nivel = 0;
	int bloques_vivos = 0;
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
	extra_life.bounding = set_bounding(0,0,0,0);
	extra_life.state = POWERUP_INACTIVE;
	
	powerup wider_platform;
	wider_platform.bounding = set_bounding(0,0,0,0);
	wider_platform.state = POWERUP_INACTIVE;
	
	powerup bomb;
	bomb.bounding = set_bounding(0,0,0,0);
	bomb.state = POWERUP_INACTIVE;
	
	powerup powerups_mat[3] = {extra_life, wider_platform, bomb};
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
		// se sigue en el menu principal //
		if (!fuera_mainmenu && !ball1.vx && !ball1.vy) {
			lado = (disAlto / 2) / ALTO; // cuanto mide los lados verticales de las casillas
			radio = ((2.5 * (float) disAncho) / 255);
			anchorect = (float) disAncho / ANCHO;
			anchoplat = (float) disAncho / 22;
			ball1.x = (float) disAncho / 2;
			ball1.y = disAlto - ((disAlto - ((float) disAlto * 0.90))
				+ (6 * anchorect) / 32);
			ball1.bounding = set_bounding(ball1.x - radio, (ball1.y - radio),
					ball1.x + radio, (ball1.y + radio));
			platform.bounding = set_bounding(((float) disAncho / 2) - 2.5 * anchoplat,
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
				// Mover estrellas
				for (int i = 0; i < MAX_STARS; i++) {
					stars[i].y += stars[i].velocidad;
					if (stars[i].y > disAlto) {
						stars[i].y = 0;
						stars[i].x = rand() % disAncho;
					}
				}
				// Duración: 3 segundos (180 frames)
				if (frames_transicion > 180) {
					en_transicion = false;
				}
				redraw = true;
			}
			else {
				if (!fuera_mainmenu && !menu_opciones) { // el usuario esta en la pantalla inicial de la app
					for (int i = 0; i < 3; i++) {
						if (collide(mouse.x, mouse.y, mouse.x, mouse.y, matriz_menu[i].bounding.ulx,
						matriz_menu[i].bounding.uly, matriz_menu[i].bounding.drx,
						matriz_menu[i].bounding.dry)) {
							al_set_system_mouse_cursor(disp,
							ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
							break;
						}
						else {
							al_set_system_mouse_cursor(disp, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
						}
					}
				} else if (menu_opciones) { // el usuario esta en la parte de opciones
					for (int i = 4; i <= 8; i++) {
						if (collide(mouse.x, mouse.y, mouse.x, mouse.y, matriz_menu[i].bounding.ulx,
						matriz_menu[i].bounding.uly, matriz_menu[i].bounding.drx,
						matriz_menu[i].bounding.dry)) {
							al_set_system_mouse_cursor(disp, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
							break;
						} else {
							al_set_system_mouse_cursor(disp,
									ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
						}
					}
				} else if (fuera_mainmenu && !menu_opciones) { // el usuario inicio salio de todos los menus
					if (pausa) {
						for (int i = 0; i < 3; i++) {
							if (collide(mouse.x, mouse.y, mouse.x, mouse.y,
							matriz_pausa[i].bounding.ulx, matriz_pausa[i].bounding.uly,
							matriz_pausa[i].bounding.drx, matriz_pausa[i].bounding.dry)) {
								al_set_system_mouse_cursor(disp,
								ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
								break;
							} else {
								al_set_system_mouse_cursor(disp,
										ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
							}
						}
					} else {
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
						float centro_paleta =
								platform.bounding.ulx
										+ (platform.bounding.drx
												- platform.bounding.ulx) / 2;
						ball1.x = centro_paleta;
						ball1.y = platform.bounding.uly - radio - 1;

						ball1.bounding = set_bounding(ball1.x - radio,
								ball1.y - radio, ball1.x + radio,
								ball1.y + radio);
					}

					if (collide(ball1.bounding.ulx, ball1.bounding.uly,
							ball1.bounding.drx, ball1.bounding.dry, -10, 0, 0,
							disAlto)) {
						ball1.vx = fabs(ball1.vx);
						ball1.x = radio + 3;
					}
					if (collide(ball1.bounding.ulx, ball1.bounding.uly,
							ball1.bounding.drx, ball1.bounding.dry,
							disAncho - 10, 0, disAncho, disAlto)) {
						ball1.vx = -fabs(ball1.vx);
						ball1.x = disAncho - radio - 3;
					}
					if (collide(ball1.bounding.ulx, ball1.bounding.uly,
							ball1.bounding.drx, ball1.bounding.dry, -5, -10,
							disAncho + 5, 0)) {
						ball1.vy = -ball1.vy;
						ball1.y = radio + 3;
					}
					if (collide(ball1.bounding.ulx, ball1.bounding.uly,
							ball1.bounding.drx, ball1.bounding.dry, -5, disAlto,
							disAncho + 5, disAlto + 10)) {
						ball1.vx = 0;
						ball1.vy = 0;
						ball1.x = (float) disAncho / 2;
						ball1.y = disAlto
								- ((disAlto - ((float) disAlto * 0.90))
										+ (6 * anchorect) / 32);
						ball1.bounding = set_bounding(ball1.x - radio,
								(ball1.y - radio), ball1.x + radio,
								(ball1.y + radio));
						platform.bounding = set_bounding(
								((float) disAncho / 2) - 2.5 * anchoplat,
								disAlto - (disAlto - ((float) disAlto * 0.90)),
								((float) disAncho / 2) + 2.5 * anchoplat,
								disAlto
										- (disAlto - ((float) disAlto * 0.895)
												- 16));
						vidas--;
					}

					char collision_wp = collide(ball1.bounding.ulx,
							ball1.bounding.uly, ball1.bounding.drx,
							ball1.bounding.dry, platform.bounding.ulx,
							platform.bounding.uly, platform.bounding.drx,
							platform.bounding.dry);

					if (collision_wp == 1
							&& ball1.bounding.dry >= platform.bounding.uly) {
						ball1.y = platform.bounding.uly - radio - 1;
						hit =
								(ball1.x - platform.bounding.ulx)
										/ (platform.bounding.drx
												- platform.bounding.ulx);
						angulo = (hit - 0.5) * 2.0 * max_angulo;
						ball1.vx = ball1.dx * sin(DEGTORAD(angulo));
						ball1.vy = -ball1.dy * cos(DEGTORAD(angulo));
					}

					bool colisiono = false;
					for (int i = 0; i < ALTO && !colisiono; i++) {
						for (int j = 0; j < ANCHO && !colisiono; j++) {
							char collision_wb = collide(ball1.bounding.ulx,
									ball1.bounding.uly, ball1.bounding.drx,
									ball1.bounding.dry, mat[i][j].bounding.ulx,
									mat[i][j].bounding.uly,
									mat[i][j].bounding.drx,
									mat[i][j].bounding.dry);
							if (collision_wb == -1) {
								if (mat[i][j].cant_impactos_actual >= 1) {
									if (modo_demo && super_romper) {
										mat[i][j].cant_impactos_actual = 0; // MUERE INSTANTANEAMENTE
									} else {
										--mat[i][j].cant_impactos_actual; // Daño normal
									}
								}
								if (mat[i][j].estado) {
									ball1.vx = -ball1.vx;
									
									colisiono = true;
								}

							} else if (collision_wb == 1) {
								if (mat[i][j].cant_impactos_actual >= 1) {
									if (modo_demo && super_romper) {
										mat[i][j].cant_impactos_actual = 0; // MUERE INSTANTANEAMENTE
									} else {
										--mat[i][j].cant_impactos_actual; // Daño normal
									}
								}
								if (mat[i][j].estado) {
									ball1.vy = -ball1.vy;
									
									colisiono = true;
								}

							} else if (collision_wb == 2) {
								if (mat[i][j].cant_impactos_actual >= 1) {
									if (modo_demo && super_romper) {
										mat[i][j].cant_impactos_actual = 0; // MUERE INSTANTANEAMENTE
									} else {
										--mat[i][j].cant_impactos_actual; // Daño normal
									}
								}
								if (mat[i][j].estado) {
									ball1.vy = -ball1.vy;
									ball1.vx = -ball1.vx;
									
									colisiono = true;

								}
							}
							if (mat[i][j].cant_impactos_actual < 1 && mat[i][j].estado) {
								mat[i][j].estado = false;
								for (int k = 0; k < 3; k++) {
									if (dice == rand() % 10 && powerups_mat[k].state == POWERUP_INACTIVE) { // solo puede haber un powerup del mismo tipo en pantalla
										powerups_mat[k].state = POWERUP_FALLING;
										powerups_mat[k].bounding = set_bounding(mat[i][j].bounding.ulx + anchorect / 4, mat[i][j].bounding.uly,
										mat[i][j].bounding.drx - anchorect / 4, mat[i][j].bounding.dry);
										powerups_mat[k].dy = 3;
										break;
									}
								}
								puntaje += 100;
							}

						}

					}

					bloques_vivos = recuento_bloques(mat);
					if (bloques_vivos) {
						bloques_vivos = 0;
					} else {
						nivel_ganado = true;
						bloques_vivos = 0;
					}

					if (nivel_ganado) {
						nivel++;
						if (nivel >= 3) {
							game_over = true;
							nivel_ganado = false;
							nivel = 0;
						} else {
							llenar_mat(mat, nivel);

							// Resetear posiciones (Tu código de siempre)
							ball1.vx = 0;
							ball1.vy = 0;
							ball1.x = (float) disAncho / 2;
							ball1.y = disAlto
									- ((disAlto - ((float) disAlto * 0.90))
											+ (6 * anchorect) / 32);
							ball1.bounding = set_bounding(ball1.x - radio,
									ball1.y - radio, ball1.x + radio,
									ball1.y + radio);
							platform.bounding = set_bounding(
									((float) disAncho / 2) - 2.5 * anchoplat,
									disAlto
											- (disAlto
													- ((float) disAlto * 0.90)),
									((float) disAncho / 2) + 2.5 * anchoplat,
									disAlto
											- (disAlto
													- ((float) disAlto * 0.895)
													- 16));

							// --- ACTIVAR MODO GALAGA ---
							nivel_ganado = false;
							en_transicion = true;  // ¡Aquí empieza la magia!
							frames_transicion = 0;
							init_stars(stars, disAncho, disAlto);
						}
					}

					ball1.bounding.ulx += ball1.vx;
					ball1.bounding.uly += ball1.vy;
					ball1.bounding.drx += ball1.vx;
					ball1.bounding.dry += ball1.vy;
					ball1.x = ball1.bounding.ulx + radio;
					ball1.y = ball1.bounding.uly - radio;
					
					for (int p = 0 ; p < 3 ; p++) {
						if (powerups_mat[p].state == POWERUP_FALLING) {
							powerups_mat[p].bounding.uly += powerups_mat[p].dy;
							powerups_mat[p].bounding.dry += powerups_mat[p].dy;
							if (powerups_mat[p].bounding.uly > disAlto) {
								powerups_mat[p].state = POWERUP_INACTIVE;
							}
							else if (collide(platform.bounding.ulx, platform.bounding.uly, platform.bounding.drx, platform.bounding.dry,
							powerups_mat[p].bounding.ulx, powerups_mat[p].bounding.uly, powerups_mat[p].bounding.drx, powerups_mat[p].bounding.dry)) {
								powerups_mat[p].state = POWERUP_ACTIVE;
							}
						}
					}
					
					if (powerups_mat[0].state == POWERUP_ACTIVE) {
						vidas += 1;
						powerups_mat[0].state = POWERUP_INACTIVE;
					}

					if (vidas == 0) {
						game_over = true;
						nivel = 0;
					}

					al_clear_to_color(themeslist[contador].color_pantalla);
					redraw = true;
				}

				al_clear_to_color(themeslist[contador].color_pantalla);
				redraw = true;
			}
		}

		else if ((event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)) { // si se apreta la X de la ventana se cierra el programa
			cerrar = true;
		} else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			if (event.keyboard.keycode == ALLEGRO_KEY_A
					|| event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
				key_left = true;
			}
			if (event.keyboard.keycode == ALLEGRO_KEY_D
					|| event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
				key_right = true;
			}

		} else if (event.type == ALLEGRO_EVENT_KEY_UP) {
			if (event.keyboard.keycode == ALLEGRO_KEY_A
					|| event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
				key_left = false;
			}
			if (event.keyboard.keycode == ALLEGRO_KEY_D
					|| event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
				key_right = false;
			}
		} else if (event.type == ALLEGRO_EVENT_KEY_CHAR) { // Se fija si se toco alguna tecla
			if (event.keyboard.keycode == ALLEGRO_KEY_SPACE && fuera_mainmenu
					&& (ball1.vx == 0 && ball1.vy == 0) && !pausa
					&& !menu_opciones) {
				if (game_over) {
					vidas = 3;
					game_over = false;
					llenar_mat(mat, 0);
				}
				angulo = (rand() % 100) + 15;
				ball1.vx = (float) ball1.dx * cos(DEGTORAD(angulo));
				ball1.vy = -((float) ball1.dy * sin(DEGTORAD(angulo)));
			} else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) { // Si se toca escape se pone en pausa
				if (game_over) {
					vidas = 3;
					pausa = false;
					fuera_mainmenu = false;
					llenar_mat(mat, 0);
					game_over = false;
				} else if (!pausa) {
					pausa = true;
				} else {
					pausa = false;
				}

			}

			// --- TRUCOS MODO DEMO ---
			else if (fuera_mainmenu && modo_demo) {
				// TECLA 1: Vida Extra
				if (event.keyboard.keycode == ALLEGRO_KEY_1) {
					vidas++;
				}
				// TECLA 2: Pasar de Nivel
				if (event.keyboard.keycode == ALLEGRO_KEY_2) {
					nivel_ganado = true; // ¡Esto es todo! El juego hará el resto.
				}
				// TECLA 3: Activar Super Romper (Toggle)
				if (event.keyboard.keycode == ALLEGRO_KEY_3) {
					super_romper = !super_romper;
				}
			}

		} else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			if (!fuera_mainmenu && !menu_opciones) {
				if (collide(event.mouse.x, event.mouse.y, event.mouse.x,
						event.mouse.y, matriz_menu[0].bounding.ulx,
						matriz_menu[0].bounding.uly,
						matriz_menu[0].bounding.drx,
						matriz_menu[0].bounding.dry)) {
					fuera_mainmenu = true;
					for (int i = 0; i < ALTO; i++) {
						for (int j = 0; j < ANCHO; j++) {
							mat[i][j].bounding.ulx = i * (float) disAncho/ANCHO;
							mat[i][j].bounding.uly = j * lado;
							mat[i][j].bounding.drx = i * (float) disAncho
									/ ANCHO + disAlto / ALTO;
							mat[i][j].bounding.dry =
									j * lado + disAncho / ANCHO;
							mat[i][j].bounding = set_bounding(
									j * (float) disAncho / ANCHO,
									(i * lado) + 3 * lado,
									j * (float) disAncho / ANCHO
											+ (float) disAncho / ANCHO,
									i * lado + 4 * lado);
						}
					}
					llenar_mat(mat, 0);

				} else if (collide(event.mouse.x, event.mouse.y, event.mouse.x,
						event.mouse.y, matriz_menu[1].bounding.ulx,
						matriz_menu[1].bounding.uly,
						matriz_menu[1].bounding.drx,
						matriz_menu[1].bounding.dry)) {
					menu_opciones = true;
				} else if (collide(event.mouse.x, event.mouse.y, event.mouse.x,
						event.mouse.y, matriz_menu[2].bounding.ulx,
						matriz_menu[2].bounding.uly,
						matriz_menu[2].bounding.drx,
						matriz_menu[2].bounding.dry)) {
					cerrar = true;
				}
			}

			else if (pausa) {
				if (collide(event.mouse.x, event.mouse.y, event.mouse.x,
						event.mouse.y, matriz_pausa[0].bounding.ulx,
						matriz_pausa[0].bounding.uly,
						matriz_pausa[0].bounding.drx,
						matriz_pausa[0].bounding.dry)) {
					pausa = false;
				} else if (collide(event.mouse.x, event.mouse.y, event.mouse.x,
						event.mouse.y, matriz_pausa[1].bounding.ulx,
						matriz_pausa[1].bounding.uly,
						matriz_pausa[1].bounding.drx,
						matriz_pausa[1].bounding.dry)) {
					// 1. Quitamos la pausa
					pausa = false;

					// 2. REINICIO TOTAL DE ESTADÍSTICAS
					vidas = 3;      // Volvemos a tener todas las vidas
					puntaje = 0;    // El puntaje vuelve a 0
					nivel = 0;      // Volvemos al primer nivel (Nivel 1)

					// 3. Recargamos la matriz del Nivel 0 (el primero)
					llenar_mat(mat, nivel);

					// 4. Reseteamos la bola y la plataforma al centro
					ball1.vx = 0;
					ball1.vy = 0;
					ball1.x = (float) disAncho / 2;
					ball1.y = disAlto
							- ((disAlto - ((float) disAlto * 0.90))
									+ (6 * anchorect) / 32);

					// Actualizamos las cajas de colisión (Hitbox)
					ball1.bounding = set_bounding(ball1.x - radio,
							(ball1.y - radio), ball1.x + radio,
							(ball1.y + radio));
					platform.bounding =
							set_bounding(
									((float) disAncho / 2) - 2.5 * anchoplat,
									disAlto
											- (disAlto
													- ((float) disAlto * 0.90)),
									((float) disAncho / 2) + 2.5 * anchoplat,
									disAlto
											- (disAlto
													- ((float) disAlto * 0.90)
													- 16));
				} else if (collide(event.mouse.x, event.mouse.y, event.mouse.x,
						event.mouse.y, matriz_pausa[2].bounding.ulx,
						matriz_pausa[2].bounding.uly,
						matriz_pausa[2].bounding.drx,
						matriz_pausa[2].bounding.dry)) {

					vidas = 3;
					pausa = false;
					fuera_mainmenu = false;
					llenar_mat(mat, 0);

					ball1.vx = 0;
					ball1.vy = 0;
					ball1.x = (float) disAncho / 2;
					ball1.y = disAlto
							- ((disAlto - ((float) disAlto * 0.90))
									+ (6 * anchorect) / 32);
					ball1.bounding = set_bounding(ball1.x - radio,
							(ball1.y - radio), ball1.x + radio,
							(ball1.y + radio));
					platform.bounding =
							set_bounding(
									((float) disAncho / 2) - 2.5 * anchoplat,
									disAlto
											- (disAlto
													- ((float) disAlto * 0.90)),
									((float) disAncho / 2) + 2.5 * anchoplat,
									disAlto
											- (disAlto
													- ((float) disAlto * 0.90)
													- 16));

				}
			} else if (menu_opciones) {
				// ---------------------------------------------------------
				// 1. ZONA TEMA (Altura Y entre 200 y 260)
				// ---------------------------------------------------------
				if (event.mouse.y > 200 && event.mouse.y < 260) {
					// Click a la izquierda (Flecha <)
					if (event.mouse.x < disAncho / 2) {
						if (contador == 0)
							contador = 2;
						else
							contador--;
					}
					// Click a la derecha (Flecha >)
					else {
						if (contador == 2)
							contador = 0;
						else
							contador++;
					}
				}

				// ---------------------------------------------------------
				// 2. ZONA VELOCIDAD (Altura Y entre 320 y 380)
				// ---------------------------------------------------------
				if (event.mouse.y > 320 && event.mouse.y < 380) {
					// Click a la izquierda (Menos velocidad)
					if (event.mouse.x < disAncho / 2) {
						if (ball1.dx > 4.0)
							ball1.dx -= 1.0;
					}
					// Click a la derecha (Mas velocidad)
					else {
						if (ball1.dx < 10.0)
							ball1.dx += 1.0;
					}
				}

				// ---------------------------------------------------------
				// 3. ZONA MODO DEMO (Altura Y entre 440 y 500)
				// ---------------------------------------------------------
				if (event.mouse.y > 440 && event.mouse.y < 500) {
					// Click en todo el ancho central
					if (event.mouse.x > disAncho / 2 - 250
							&& event.mouse.x < disAncho / 2 + 250) {
						modo_demo = !modo_demo;
						if (!modo_demo)
							super_romper = false;
					}
				}

				// ---------------------------------------------------------
				// 4. ZONA BOTON ATRAS (Altura Y entre 580 y 630)
				// ---------------------------------------------------------
				if (event.mouse.y > 580 && event.mouse.y < 630) {
					if (event.mouse.x > disAncho / 2 - 100
							&& event.mouse.x < disAncho / 2 + 100) {
						menu_opciones = false;
					}
				}
			}
		}

		else if (event.type == ALLEGRO_EVENT_MOUSE_AXES && event.mouse.dz != 0
				&& !fuera_mainmenu) { // Detecta si se movio la rueda del boton y si esta en el menu
			if (event.mouse.dz > 0) { // Se fija que el cambio del mouse scroll sea para arriba
				disAlto += 10; // aumenta alto del display
				disAncho += 10; // aumenta ancho del display
				redraw = true;
			} else if (event.mouse.dz < 0) // Se fija que el cambio del mouse scroll sea para abajo
					{
				disAlto -= 10; // decrementa alto del display
				disAncho -= 10; // decrementa ancho del display
				redraw = true;
			}
			if (disAlto < 500) { // impide que el display sea muy pequeño para que las letras entren
				disAlto = 500;
			}
			if (disAncho < 500) { // impide que el display sea muy pequeño para que las letras entren
				disAncho = 500;
			}
			al_resize_display(disp, disAncho, disAlto); // Cambia tamaño de pantalla
			al_clear_to_color(themeslist[contador].color_pantalla); // limpia la pantalla para que no se escriba texto por encima
		}

		if (redraw && al_is_event_queue_empty(queue)) {

			// Limpieza general por seguridad
			al_clear_to_color(themeslist[contador].color_pantalla);

			if (fuera_mainmenu && !menu_opciones) {

				if (en_transicion) {
					// --- DIBUJAR TRANSICION GALAGA ---
					al_clear_to_color(al_map_rgb(0, 0, 0)); // Fondo negro

					for (int i = 0; i < MAX_STARS; i++) {
						al_draw_filled_rectangle(stars[i].x, stars[i].y,
								stars[i].x + stars[i].tamano,
								stars[i].y + stars[i].tamano,
								al_map_rgb(255, 255, 255));
					}

					// Texto Parpadeante
					if (frames_transicion % 40 < 30) {
						al_draw_textf(gameoverfont, al_map_rgb(0, 255, 255),
								disAncho / 2, disAlto / 2, ALLEGRO_ALIGN_CENTER,
								"STAGE %d", nivel + 1);
						al_draw_text(font, al_map_rgb(255, 0, 0), disAncho / 2,
								disAlto / 2 + 40, ALLEGRO_ALIGN_CENTER,
								"READY");
					}

					// ¡¡¡ESTO ES LO QUE FALTABA!!!
					al_flip_display();
					redraw = false;
					// ---------------------------

				} else {
					// --- DIBUJAR JUEGO NORMAL ---
					al_clear_to_color(themeslist[contador].color_pantalla);

					dibujar_all(disAlto, disAncho, lado,
							themeslist[contador].color_lineas,
							platform.bounding, ball1, mat,
							themeslist[contador].color_pantalla);
							
					dibujar_powerups(powerups_mat);

					// (Borré la linea duplicada de dibujar_all que tenías)


					if (pausa) {
						// Tu logica de pausa...
						al_draw_text(gameoverfont,
								themeslist[contador].color_texto, disAncho / 2,
								matriz_menu[0].bounding.dry
										- (matriz_menu[0].bounding.dry
												- matriz_menu[0].bounding.uly)
												* 0.75, ALLEGRO_ALIGN_CENTER,
								matriz_pausa[0].texto);
						al_draw_text(gameoverfont,
								themeslist[contador].color_texto, disAncho / 2,
								matriz_menu[1].bounding.dry
										- (matriz_menu[1].bounding.dry
												- matriz_menu[1].bounding.uly)
												* 0.75, ALLEGRO_ALIGN_CENTER,
								matriz_pausa[1].texto);
						al_draw_text(gameoverfont,
								themeslist[contador].color_texto, disAncho / 2,
								matriz_menu[2].bounding.dry
										- (matriz_menu[2].bounding.dry
												- matriz_menu[2].bounding.uly)
												* 0.75, ALLEGRO_ALIGN_CENTER,
								matriz_pausa[2].texto);
					}

					if (game_over) {
						al_draw_multiline_text(gameoverfont,
								themeslist[contador].color_texto, disAncho / 2,
								disAlto / 2, disAncho - disAncho / 3, 30,
								ALLEGRO_ALIGN_CENTER,
								"GAME OVER\n PRESIONE [ESC] PARA IR AL MENU PRINCIPAL O [SPACE] PARA VOLVER A INTENTAR");
					}

					// HUD
					al_draw_text(gameoverfont, galaga_red, 10, disAlto - 30,
							ALLEGRO_ALIGN_LEFT, "LIVES");
					for (int i = 0; i < vidas; i++) {
						float bx = 100 + (i * 25);
						float by = disAlto - 5;
						al_draw_filled_triangle(bx, by, bx + 8, by - 15,
								bx + 16, by, galaga_yellow);
					}
					al_draw_text(gameoverfont, galaga_red, disAncho / 2, 5,
							ALLEGRO_ALIGN_CENTER, "SCORE");
					al_draw_textf(gameoverfont, galaga_white, disAncho / 2, 30,
							ALLEGRO_ALIGN_CENTER, "%d", puntaje);

					if (modo_demo) {
						al_draw_text(font, al_map_rgb(255, 255, 0),
								disAncho / 2, 60, ALLEGRO_ALIGN_CENTER,
								"--- MODO DEMO ---");
						al_draw_text(font, al_map_rgb(255, 255, 0),
								disAncho / 2, 80, ALLEGRO_ALIGN_CENTER,
								"1:Vida | 2:Nivel | 3:InstaKill");

						if (super_romper) {
							al_draw_text(gameoverfont, al_map_rgb(255, 0, 0),
									disAncho / 2, disAlto / 2 + 50,
									ALLEGRO_ALIGN_CENTER, "!! INSTA KILL !!");
						}
					}

					// MOSTRAR PANTALLA JUEGO NORMAL
					al_flip_display();
					redraw = false;
				}

			} else if (!fuera_mainmenu) { // MENU PRINCIPAL
				al_draw_text(gameoverfont, galaga_yellow, disAncho / 2, 100,
						ALLEGRO_ALIGN_CENTER, "BREAKOUT 8-BIT");
				if (menu_opciones) {
					// --- TUS OPCIONES ---
					al_draw_text(gameoverfont, themeslist[contador].color_texto,
							disAncho / 2, 200, ALLEGRO_ALIGN_CENTER, "TEMA:");
					al_draw_text(gameoverfont, themeslist[contador].color_texto,
							disAncho / 2 - 160, 230, ALLEGRO_ALIGN_CENTER, "<");
					al_draw_text(gameoverfont, themeslist[contador].color_texto,
							disAncho / 2, 230, ALLEGRO_ALIGN_CENTER,
							themeslist[contador].nombre);
					al_draw_text(gameoverfont, themeslist[contador].color_texto,
							disAncho / 2 + 160, 230, ALLEGRO_ALIGN_CENTER, ">");

					al_draw_text(gameoverfont, themeslist[contador].color_texto,
							disAncho / 2, 320, ALLEGRO_ALIGN_CENTER,
							"VELOCIDAD:");
					al_draw_text(gameoverfont, themeslist[contador].color_texto,
							disAncho / 2 - 160, 350, ALLEGRO_ALIGN_CENTER, "<");
					al_draw_textf(gameoverfont,
							themeslist[contador].color_texto, disAncho / 2, 350,
							ALLEGRO_ALIGN_CENTER, "%.1f", ball1.dx);
					al_draw_text(gameoverfont, themeslist[contador].color_texto,
							disAncho / 2 + 160, 350, ALLEGRO_ALIGN_CENTER, ">");

					ALLEGRO_COLOR color_demo =
							modo_demo ?
									al_map_rgb(0, 255, 0) :
									al_map_rgb(255, 0, 0);

					al_draw_text(gameoverfont, themeslist[contador].color_texto,
							disAncho / 2, 440, ALLEGRO_ALIGN_CENTER,
							"MODO DEMO:");
					al_draw_text(gameoverfont, themeslist[contador].color_texto,
							disAncho / 2 - 160, 470, ALLEGRO_ALIGN_CENTER, "<");
					al_draw_text(gameoverfont, color_demo, disAncho / 2, 470,
							ALLEGRO_ALIGN_CENTER,
							modo_demo ? "ACTIVADO" : "DESACTIVADO");
					al_draw_text(gameoverfont, themeslist[contador].color_texto,
							disAncho / 2 + 160, 470, ALLEGRO_ALIGN_CENTER, ">");

					al_draw_text(gameoverfont, themeslist[contador].color_texto,
							disAncho / 2, 600, ALLEGRO_ALIGN_CENTER, "ATRAS");

				} else {
					// BOTONES MENU PRINCIPAL
					al_draw_text(gameoverfont, themeslist[contador].color_texto,
							disAncho / 2,
							matriz_menu[0].bounding.dry
									- (matriz_menu[0].bounding.dry
											- matriz_menu[0].bounding.uly)
											* 0.75, ALLEGRO_ALIGN_CENTER,
							matriz_menu[0].texto);
					al_draw_text(gameoverfont, themeslist[contador].color_texto,
							disAncho / 2,
							matriz_menu[1].bounding.dry
									- (matriz_menu[1].bounding.dry
											- matriz_menu[1].bounding.uly)
											* 0.75, ALLEGRO_ALIGN_CENTER,
							matriz_menu[1].texto);
					al_draw_text(gameoverfont, themeslist[contador].color_texto,
							disAncho / 2,
							matriz_menu[2].bounding.dry
									- (matriz_menu[2].bounding.dry
											- matriz_menu[2].bounding.uly)
											* 0.75, ALLEGRO_ALIGN_CENTER,
							matriz_menu[2].texto);
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
	al_destroy_audio_stream(music);
	printf("Destruyendo imagen...\n");
	return 0;
}

