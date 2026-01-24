#include "funciones.h"



int main()
{
    
    srand(time(NULL)); // semilla random para la funcion rand()
    bloque mat [ALTO][ANCHO]; // matriz principal
    option_box matriz_menu[3];
    
    
    // BLOQUE DE INICIsALIZACION //
    must_init(al_init(), "allegro"); // Cada must_init verifica que se haya inicializado cada cosa
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_init_primitives_addon(), "primitivas");
    must_init(al_install_mouse(), "mouse");
    must_init(al_install_audio(), "audio");
    must_init(al_init_acodec_addon(), "audio codecs");
    must_init(al_reserve_samples(16), "reserve samples");
    must_init(al_init_ttf_addon(),"TTF");
    ////////////////////////////////

    ///////////// BLOQUE DE VARIABLES /////////////////
    int disAlto = 720; // Alto de la pantalla
    int disAncho = 720;// Ancho de la pantalla
    ALLEGRO_DISPLAY* disp = al_create_display(disAncho, disAlto); // Display
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0); // Timer
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue(); // queue
    ALLEGRO_AUDIO_STREAM* music = al_load_audio_stream("resources/proper_summer.flac",2, 2048); // musica
    ALLEGRO_FONT* font = al_load_ttf_font("resources/OpenSans.ttf",15,0); // fuente
    ALLEGRO_FONT* gameoverfont = al_load_ttf_font("resources/Tiny5-Regular.ttf",30,0);
    must_init(disp,"display"); // se verifica que estos punteros se hayan inicializado correctamente
    must_init(disp,"timer");
    must_init(disp,"queue");
    must_init(font,"font");
    must_init(gameoverfont,"font");
    must_init(music, "music");
    bool redraw = true; // variable que admite el redibujar la pantalla
    bool cerrar = false; // variable que cierra todo el programa
    bool fuera_mainmenu = false; // variable que va dar inicio al juego y la impresion de la grilla y los casilleros correspondientes
    bool pausa = false;
    bool primer_tiro = false;
    bool game_over = false;
    bool key_left = false;
    bool key_right = false;
    int contador = 0; // variable que sirve para determinar que tema usar
    int high_score = 50000; // Un valor inicial clásico
        // Colores Galaga
        ALLEGRO_COLOR galaga_red = al_map_rgb(255, 0, 0);
        ALLEGRO_COLOR galaga_white = al_map_rgb(255, 255, 255);
        ALLEGRO_COLOR galaga_yellow = al_map_rgb(222, 222, 0);
    

    typedef struct { // Se crea la estructura temas
        char * nombre;
        ALLEGRO_COLOR color_texto;
        ALLEGRO_COLOR color_pantalla;
        ALLEGRO_COLOR color_lineas;
        ALLEGRO_COLOR color_casilleros;

    }THEMES;
    
    THEMES dark = {"Dark",al_map_rgb(255,255,255),al_map_rgb(0,0,0),al_map_rgb(192,192,192),al_map_rgb(204,204,0)}; // Se le da el valor del color a los temas
    THEMES light = {"Light",al_map_rgb(0,0,0),al_map_rgb(255,255,255),al_map_rgb(192,192,192),al_map_rgb(0,102,204)};
    THEMES bluescreen = {"Blue Screen",al_map_rgb(255,255,255),al_map_rgb(102,102,255),al_map_rgb(192,192,192),al_map_rgb(255,255,255)};
    THEMES random = {"Random",al_map_rgb(rand() % 256,rand() % 256,rand() % 256),al_map_rgb(rand() % 256,rand() % 256,rand() % 256),al_map_rgb(rand() % 256,rand() % 256,rand() % 256),al_map_rgb(rand() % 256,rand() % 256,rand() % 256)};

    ///////////////////////////////////////////////////////////////////////////////

    ///////////////////// BLOQUE DE REGISTRO DE EVENTOS ///////////////////////////
    al_register_event_source(queue, al_get_display_event_source(disp)); // Permite que exista display
    al_register_event_source(queue, al_get_timer_event_source(timer)); // Permite que exista el timer
    al_register_event_source(queue, al_get_mouse_event_source()); // Permite que el usuario pueda usar el mouse
    al_register_event_source(queue, al_get_keyboard_event_source()); // Permite que el usuario pueda usar el teclado
    al_attach_audio_stream_to_mixer(music, al_get_default_mixer()); // Permite que exista audio
    al_set_audio_stream_playmode(music, ALLEGRO_PLAYMODE_LOOP); // como se quiere escuchar la musica y la reproduce
    ALLEGRO_EVENT event; // se inicializa la varible evento que determinara que evento sucedio
    // INDICA AL PROGRAMA QUE EVENTOS PUEDEN PASAR //
    ////////////////////////////////////////////////////////////////////////////////
    



    

    // BLOQUE ENTIDADES //
    entities platform;
    entities ball1;
    

    platform.dx = 10;
    platform.dy = 0;
    ball1.dx = 5;
    ball1.dy = 5;
    ball1.vx = 0;
    ball1.vy = 0;
    float angulo = 90;
    float max_angulo = 70;
    float largo;
    float hit;
    char vidas = 3;
    int puntaje = 0;
    bool muerto = false;
    float anchorect;
    float anchoplat;
    float radio;
    float lado;
    bool menu_opciones = false;
    
    //////////////////////


    // llena la matriz de casillas vivas
    al_start_timer(timer); // empieza el timer

        while(!cerrar)
    {   
        
        THEMES themeslist[MAXTHEMES] = {dark,light,bluescreen,random}; // Arreglo de los temas disponibles
        if (!fuera_mainmenu && !ball1.vx && !ball1.vy){
            lado = (disAlto/2)/ALTO; // cuanto mide los lados verticales de las casillas
            radio = ((2.5*(float)disAncho)/255); 
            anchorect = (float)disAncho/ANCHO;
            anchoplat = (float)disAncho/22;
            ball1.x = (float)disAncho/2;
            ball1.y = disAlto - ((disAlto-((float)disAlto*0.90))+(6*anchorect)/32);
            ball1.bounding =  set_bounding(ball1.x-radio,(ball1.y-radio),ball1.x+radio,(ball1.y+radio));
            platform.bounding = set_bounding(((float)disAncho/2)-2.5*anchoplat,disAlto - (disAlto-((float)disAlto*0.90)),((float)disAncho/2)+2.5*anchoplat,disAlto-(disAlto-((float)disAlto*0.90)-16)); // 2.5 COEFICIENTE DE TAMAÑO
            float centro_x = disAncho / 2;
                        float ancho_caja = 200;
                        matriz_menu[0].bounding = set_bounding(centro_x - ancho_caja/2, disAlto/3 + lado*2, centro_x + ancho_caja/2, disAlto/3 + lado*4);
                        matriz_menu[1].bounding = set_bounding(centro_x - ancho_caja/2, disAlto/3 + lado*4, centro_x + ancho_caja/2, disAlto/3 + lado*6);
                        matriz_menu[2].bounding = set_bounding(centro_x - ancho_caja/2, disAlto/3 + lado*6, centro_x + ancho_caja/2, disAlto/3 + lado*8);
        }
        



        al_wait_for_event(queue, &event); // indica al programa que revise si hay eventos

        if(event.type == ALLEGRO_EVENT_TIMER){ // si el usuario no ingresa nada simplemente se redibuja el estado en el que estaba
            if (fuera_mainmenu){

            	// --- MOVIMIENTO DINÁMICO DE LA PALETA ---
            	// --- MOVIMIENTO DINÁMICO DE LA PALETA ---
            	if (key_left && platform.bounding.ulx > 0) {
            	    platform.bounding.ulx -= platform.dx;
            	    platform.bounding.drx -= platform.dx;
                   
            	}
            	if (key_right && platform.bounding.drx < disAncho) {
            	    platform.bounding.ulx += platform.dx;
            	    platform.bounding.drx += platform.dx;
            	}
                
            	// NUEVO: La pelota sigue a la paleta si no ha sido lanzada
            	// NUEVO: La pelota sigue a la paleta si no ha sido lanzada
            	            	if (ball1.vx == 0 && ball1.vy == 0) {
            	            	    float centro_paleta = platform.bounding.ulx + (platform.bounding.drx - platform.bounding.ulx) / 2;
            	            	    ball1.x = centro_paleta;
            	            	    ball1.y = platform.bounding.uly - radio - 1; // La pegamos justo arriba

            	            	    // Actualizamos la caja de colisión completa para que coincida visualmente
            	            	    ball1.bounding = set_bounding(ball1.x - radio, ball1.y - radio, ball1.x + radio, ball1.y + radio);
            	            	}
            	            	if (collide(ball1.bounding.ulx, ball1.bounding.uly, ball1.bounding.drx, ball1.bounding.dry, -5, -10, disAncho+5, 60))
            	            	                {
            	            	                    ball1.vy = -ball1.vy;       // Invertimos velocidad
            	            	                    ball1.y = 60 + radio + 2;   // IMPORTANTE: Teletransportamos la bola debajo del techo (60) para que no se quede pegada

            	            	                    // Actualizamos la "caja" de la pelota inmediatamente para que el juego sepa que ya salió del techo
            	            	                    ball1.bounding = set_bounding(ball1.x - radio, ball1.y - radio, ball1.x + radio, ball1.y + radio);
            	            	                }
                if (collide(ball1.bounding.ulx,ball1.bounding.uly,ball1.bounding.drx,ball1.bounding.dry,disAncho-10,0,disAncho,disAlto))
                {
                    ball1.vx = -ball1.vx;
                    ball1.x = disAncho-radio-1;
                }
                if (collide(ball1.bounding.ulx,ball1.bounding.uly,ball1.bounding.drx,ball1.bounding.dry,-5,-10,disAncho+5,0))
                {
                    ball1.vy = -ball1.vy;
                    ball1.y = radio+1;
                }
                if (collide(ball1.bounding.ulx,ball1.bounding.uly,ball1.bounding.drx,ball1.bounding.dry,-5,disAlto+10,disAncho+5,disAlto+10))
                                {
                                    ball1.vx = 0;
                                    ball1.vy = 0;
                                    // NO forzamos ball1.x ni ball1.y aquí.
                                    // Al ser vx=0 y vy=0, el bloque "NUEVO" de arriba la moverá a la paleta automáticamente en el siguiente frame.

                                    // Solo reseteamos la posición de la plataforma al centro si quieres (opcional)
                                    platform.bounding = set_bounding(((float)disAncho/2)-2.5*anchoplat, disAlto - (disAlto-((float)disAlto*0.90)), ((float)disAncho/2)+2.5*anchoplat, disAlto-(disAlto-((float)disAlto*0.90)-16));

                                    vidas--;
                                }
                        
                char collision_wp = collide(ball1.bounding.ulx,ball1.bounding.uly,ball1.bounding.drx,ball1.bounding.dry,platform.bounding.ulx,platform.bounding.uly,platform.bounding.drx,platform.bounding.dry);
                
                            
                if (collision_wp == 1){   
                    ball1.y = platform.bounding.uly-radio-1;
                    hit = (ball1.x - platform.bounding.ulx) / (platform.bounding.drx-platform.bounding.ulx);
                    angulo = (hit-0.5)*2.0*max_angulo;
                    ball1.vx = ball1.dx*sin(DEGTORAD(angulo));
                    ball1.vy = -ball1.dy*cos(DEGTORAD(angulo));
                }
                


                bool colisiono = false;
                for (int i = 0; i < ALTO && !colisiono; i++)
                {
                    for (int j = 0; j < ANCHO && !colisiono; j++)
                    {
                        char collision_wb = collide(ball1.bounding.ulx,ball1.bounding.uly,ball1.bounding.drx,ball1.bounding.dry,mat[i][j].bounding.ulx,mat[i][j].bounding.uly,mat[i][j].bounding.drx,mat[i][j].bounding.dry);
                        if (collision_wb == -1)
                        {
                            if (mat[i][j].cant_impactos_actual >= 1)
                            {
                                --mat[i][j].cant_impactos_actual;
                            }
                            if (mat[i][j].estado)
                            {
                                ball1.vx = -ball1.vx;
                                colisiono = true;
                            }
                            
                            
                        }
                        else if (collision_wb == 1 )
                        {
                            if (mat[i][j].cant_impactos_actual >= 1)
                            {
                                --mat[i][j].cant_impactos_actual;
                                
                            }
                           if (mat[i][j].estado)
                            {
                                ball1.vy = -ball1.vy;
                                colisiono = true;
                            }
                        }
                        else if (collision_wb == 2)
                        {
                            if (mat[i][j].cant_impactos_actual >= 1)
                            {
                                --mat[i][j].cant_impactos_actual;
                                
                            }
                            if (mat[i][j].estado)
                            {
                                ball1.vy = -ball1.vy;
                                ball1.vx = -ball1.vx;
                                colisiono = true;
                            }
                            
                        }
                        if (mat[i][j].cant_impactos_actual < 1)
                        {
                            if (mat[i][j].estado == true) { // Si el bloque estaba vivo
                                mat[i][j].estado = false;   // Lo matamos
                                puntaje += 100;             // ¡Sumamos 100 puntos!
                            }
                        }
                        
                        
                    }
                    
                }
                
                ball1.bounding.ulx += ball1.vx;
                ball1.bounding.uly += ball1.vy;
                ball1.bounding.drx += ball1.vx;
                ball1.bounding.dry += ball1.vy;
                ball1.x = ball1.bounding.ulx+radio;
                ball1.y = ball1.bounding.uly-radio;


                if (vidas == 0){
                   game_over = true; 
                }
                

                



                al_clear_to_color(themeslist[contador].color_pantalla);
                redraw = true;
            }
            
            
            al_clear_to_color(themeslist[contador].color_pantalla);
            redraw = true;    
        }

        else if((event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)){ // si se apreta la X de la ventana se cierra el programa
            cerrar = true;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                    if (event.keyboard.keycode == ALLEGRO_KEY_A || event.keyboard.keycode == ALLEGRO_KEY_LEFT) key_left = true;
                    if (event.keyboard.keycode == ALLEGRO_KEY_D || event.keyboard.keycode == ALLEGRO_KEY_RIGHT) key_right = true;
                }
                else if (event.type == ALLEGRO_EVENT_KEY_UP) {

                    if (event.keyboard.keycode == ALLEGRO_KEY_A || event.keyboard.keycode == ALLEGRO_KEY_LEFT) key_left = false;
                    if (event.keyboard.keycode == ALLEGRO_KEY_D || event.keyboard.keycode == ALLEGRO_KEY_RIGHT) key_right = false;
                }
        // --- BLOQUE DE TECLAS DE ACCIÓN (ESPACIO, ESCAPE, SELECCIÓN DE TEMA) ---
                else if (event.type == ALLEGRO_EVENT_KEY_CHAR) {

                    // Si estamos en el menú de opciones
                    if (menu_opciones) {
                        if (event.keyboard.keycode == ALLEGRO_KEY_B) menu_opciones = false;
                        if (event.keyboard.keycode == ALLEGRO_KEY_UP) { ball1.dx += 0.5; ball1.dy += 0.5; }
                        if (event.keyboard.keycode == ALLEGRO_KEY_DOWN && ball1.dx > 1) { ball1.dx -= 0.5; ball1.dy -= 0.5; }
                    }

                    // Iniciar juego o lanzar pelota
                    if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                                    // CASO 1: ESTAMOS EN EL MENÚ PRINCIPAL Y TOCAMOS JUGAR (ESPACIO)
                                    if (!fuera_mainmenu && !menu_opciones) {
                                        fuera_mainmenu = true;
                                        // Generamos la matriz por primera vez
                                        for (int i = 0; i < ALTO; i++){
                                            for (int j = 0; j < ANCHO; j++) {
                                                mat[i][j].estado = true;
                                                mat[i][j].cant_impactos_total = 1;
                                                mat[i][j].cant_impactos_actual = 1;
                                                mat[i][j].bounding = set_bounding(j*(float)disAncho/ANCHO, i*lado + 80, j*(float)disAncho/ANCHO+(float)disAncho/ANCHO, i*lado+lado + 80);                                            }
                                        }
                                    }
                                    // CASO 2: YA ESTAMOS JUGANDO (LANZAR PELOTA O REINTENTAR GAME OVER)
                                    else if (fuera_mainmenu && ball1.vx == 0 && ball1.vy == 0) {
                                        if(game_over) {
                                            vidas = 3;
                                            puntaje = 0;
                                            game_over = false;
                                            // REINICIO DE MATRIZ (IMPORTANTE: Esto faltaba)
                                            for (int i = 0; i < ALTO; i++) {
                                                for (int j = 0; j < ANCHO; j++) {
                                                    mat[i][j].estado = true;
                                                    mat[i][j].cant_impactos_total = 1;
                                                    mat[i][j].cant_impactos_actual = 1;
                                                    // Agregamos +80 a las coordenadas Y para bajar la grilla
                                                    mat[i][j].bounding = set_bounding(j*(float)disAncho/ANCHO, i*lado + 80, j*(float)disAncho/ANCHO+(float)disAncho/ANCHO, i*lado+lado + 80);
                                                }
                                            }
                                        }
                                        // LANZAR PELOTA
                                        angulo = (rand() % 100) + 15;
                                        ball1.vx = (float)ball1.dx * cos(DEGTORAD(angulo));
                                        ball1.vy = -((float)ball1.dy * sin(DEGTORAD(angulo)));
                                    }
                                }

                    // Cambiar temas en el menú
                    if (!fuera_mainmenu && (event.keyboard.keycode == ALLEGRO_KEY_RIGHT || event.keyboard.keycode == ALLEGRO_KEY_LEFT)) {
                        if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) contador = (contador + 1) % MAXTHEMES;
                        else contador = (contador - 1 < 0) ? MAXTHEMES - 1 : contador - 1;
                    }

                    if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) cerrar = true;
                }

                else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && !fuera_mainmenu){
            if (collide(event.mouse.x,event.mouse.y,event.mouse.x,event.mouse.y,matriz_menu[0].bounding.ulx,matriz_menu[0].bounding.uly,matriz_menu[0].bounding.drx,matriz_menu[0].bounding.dry)){
                fuera_mainmenu = true;
                for (int i = 0; i < ALTO; i++){
                    for (int j = 0; j < ANCHO; j++)
                    {
                        mat[i][j].bounding.ulx = i*(float)disAncho/ANCHO;
                        mat[i][j].bounding.uly = j*lado + 80; // Bajamos 80px
                        mat[i][j].bounding.drx = i*(float)disAncho/ANCHO+disAlto/ALTO;
                        mat[i][j].bounding.dry = j*lado+disAncho/ANCHO + 80; // Bajamos 80px
                        mat[i][j].estado = true;
                        mat[i][j].cant_impactos_total = 1;
                        mat[i][j].cant_impactos_actual = mat[i][j].cant_impactos_total;
                        mat[i][j].bounding = set_bounding(j*(float)disAncho/ANCHO, i*lado + 80, j*(float)disAncho/ANCHO+(float)disAncho/ANCHO, i*lado+lado + 80);                    }
                }
            }else if (collide(event.mouse.x,event.mouse.y,event.mouse.x,event.mouse.y,matriz_menu[1].bounding.ulx,matriz_menu[1].bounding.uly,matriz_menu[1].bounding.drx,matriz_menu[1].bounding.dry))
            {
                menu_opciones = true;
            }else if (collide(event.mouse.x,event.mouse.y,event.mouse.x,event.mouse.y,matriz_menu[2].bounding.ulx,matriz_menu[2].bounding.uly,matriz_menu[2].bounding.drx,matriz_menu[2].bounding.dry))
            {
                cerrar = true;
            }
            
            
        }
            
        
        else if (event.type == ALLEGRO_EVENT_MOUSE_AXES && event.mouse.dz != 0 && !fuera_mainmenu){ // Detecta si se movio la rueda del boton y si esta en el menu
            if (event.mouse.dz>0){ // Se fija que el cambio del mouse scroll sea para arriba
                disAlto += 10; // aumenta alto del display
                disAncho += 10; // aumenta ancho del display
                redraw = true;      
            }else if (event.mouse.dz<0) // Se fija que el cambio del mouse scroll sea para abajo
            {
                disAlto -= 10; // decrementa alto del display
                disAncho -= 10; // decrementa ancho del display
                redraw = true;      
            }
            if (disAlto < 500){ // impide que el display sea muy pequeño para que las letras entren
                disAlto = 500;
            }
            if (disAncho < 500){ // impide que el display sea muy pequeño para que las letras entren
                disAncho = 500;
            }
            al_resize_display(disp,disAncho,disAlto); // Cambia tamaño de pantalla
            al_clear_to_color(themeslist[contador].color_pantalla); // limpia la pantalla para que no se escriba texto por encima 
        }
        
               

        if(redraw && al_is_event_queue_empty(queue))
                {
                    // 1. LIMPIAR PANTALLA A NEGRO ESPACIAL (Siempre)
                    al_clear_to_color(al_map_rgb(0, 0, 0));

                    // 2. DIBUJAR ESTRELLAS (Truco visual simple)
                    // Usamos la posición i como semilla para que parezcan estáticas pero aleatorias
                    for(int s=0; s<50; s++) {
                        float sx = (s * 37 * (int)al_get_time()) % disAncho; // Movimiento lento o estatico
                        float sy = (s * 19 * s) % disAlto;
                        al_draw_pixel(sx, sy, al_map_rgb(100, 100, 100));
                    }

                    if(fuera_mainmenu){
                        // --- HUD ESTILO GALAGA ---

                        // HIGH SCORE (Centro)
                        al_draw_text(gameoverfont, galaga_red, disAncho/2, 5, ALLEGRO_ALIGN_CENTER, "HIGH SCORE");
                        if (puntaje > high_score) high_score = puntaje;
                        al_draw_textf(gameoverfont, galaga_white, disAncho/2, 30, ALLEGRO_ALIGN_CENTER, "%d", high_score);

                        // 1UP / PUNTAJE ACTUAL (Izquierda o Derecha)
                        al_draw_text(gameoverfont, galaga_red, 60, 5, ALLEGRO_ALIGN_CENTER, "1UP");
                        al_draw_textf(gameoverfont, galaga_white, 60, 30, ALLEGRO_ALIGN_CENTER, "%06d", puntaje);

                        // VIDAS (Abajo a la izquierda - Triangulitos estilo nave)
                        al_draw_text(gameoverfont, galaga_red, 10, disAlto - 30, ALLEGRO_ALIGN_LEFT, "LIVES");
                        for(int i = 0; i < vidas; i++) {
                            float bx = 100 + (i * 25); // Posicion X
                            float by = disAlto - 15;   // Posicion Y base
                            al_draw_filled_triangle(bx, by, bx+8, by-15, bx+16, by, galaga_white);
                        }

                        // DIBUJAR ELEMENTOS DEL JUEGO
                        // --- DIBUJADO PERSONALIZADO ESTILO GALAGA ---

                                        // 1. BLOQUES (Colores de enemigos de Galaga)
                                        for (int i = 0; i < ALTO; i++) {
                                            ALLEGRO_COLOR color_bloque;
                                            // Paleta de enemigos de Galaga segun la fila:
                                            switch(i % 5) {
                                                case 0: color_bloque = al_map_rgb(255, 0, 0); break;     // Rojo (Boss Galaga)
                                                case 1: color_bloque = al_map_rgb(224, 0, 224); break;   // Purpura (Zako)
                                                case 2: color_bloque = al_map_rgb(224, 224, 0); break;   // Amarillo (Abeja)
                                                case 3: color_bloque = al_map_rgb(0, 100, 255); break;   // Azul Arcade (Escorpion)
                                                default: color_bloque = al_map_rgb(0, 224, 0); break;    // Verde (Spy)
                                            }

                                            for (int j = 0; j < ANCHO; j++) {
                                                if (mat[i][j].estado) {
                                                    // Dejamos un borde negro (padding de 2px) para que parezca grilla
                                                    al_draw_filled_rectangle(
                                                        mat[i][j].bounding.ulx + 2, mat[i][j].bounding.uly + 2,
                                                        mat[i][j].bounding.drx - 2, mat[i][j].bounding.dry - 2,
                                                        color_bloque
                                                    );
                                                }
                                            }
                                        }

                                        // 2. PALETA (Nave del Jugador - Blanco Clásico)
                                        al_draw_filled_rectangle(platform.bounding.ulx, platform.bounding.uly,
                                                                 platform.bounding.drx, platform.bounding.dry,
                                                                 galaga_white);

                                        // 3. PELOTA (Cuadrada / Pixel Gigante)
                                        // Usamos filled_rectangle en vez de circulo para el look 8-bit
                                        al_draw_filled_rectangle(ball1.x - radio, ball1.y - radio,
                                                                 ball1.x + radio, ball1.y + radio,
                                                                 galaga_white);
                        // GAME OVER OVERLAY
                        if (game_over){
                            al_draw_filled_rectangle(disAncho/2 - 150, disAlto/2 - 50, disAncho/2 + 150, disAlto/2 + 50, al_map_rgb(0,0,0));
                            al_draw_rectangle(disAncho/2 - 150, disAlto/2 - 50, disAncho/2 + 150, disAlto/2 + 50, galaga_red, 2);
                            al_draw_text(gameoverfont, galaga_red, disAncho/2, disAlto/2 - 20, ALLEGRO_ALIGN_CENTER, "GAME OVER");
                            al_draw_text(font, galaga_white, disAncho/2, disAlto/2 + 10, ALLEGRO_ALIGN_CENTER, "PRESS [SPACE] TO RESTART");
                        }
                    }
                    else if(menu_opciones){
                        // --- MENU OPCIONES ---
                        al_draw_text(gameoverfont, galaga_red, disAncho/2, 100, ALLEGRO_ALIGN_CENTER, "OPCIONES");
                        al_draw_text(font, galaga_white, disAncho/2, 250, ALLEGRO_ALIGN_CENTER, "Usa ARRIBA/ABAJO para cambiar velocidad");
                        al_draw_textf(gameoverfont, galaga_yellow, disAncho/2, 350, ALLEGRO_ALIGN_CENTER, "VELOCIDAD: %.1f", ball1.dx);
                        al_draw_text(font, galaga_white, disAncho/2, 500, ALLEGRO_ALIGN_CENTER, "Presiona [B] para VOLVER");
                    }
                    else {
                        // --- MENÚ PRINCIPAL ESTILO ARCADE ---
                        float centro_x = disAncho / 2;

                        // Título grande (opcional si tenés logo, sino texto)
                        al_draw_text(gameoverfont, galaga_yellow, centro_x, 100, ALLEGRO_ALIGN_CENTER, "BREAKOUT 8-BIT");

                        // Textos del menú
                        al_draw_text(gameoverfont, galaga_white, centro_x, matriz_menu[0].bounding.uly + 10, ALLEGRO_ALIGN_CENTER, "JUGAR");
                        al_draw_text(gameoverfont, galaga_white, centro_x, matriz_menu[1].bounding.uly + 10, ALLEGRO_ALIGN_CENTER, "OPCIONES");
                        al_draw_text(gameoverfont, galaga_white, centro_x, matriz_menu[2].bounding.uly + 10, ALLEGRO_ALIGN_CENTER, "SALIR");

                        // Cajas selectoras (Retro)
                        float ancho_caja = 200;
                        for(int i = 0; i < 3; i++) {
                            al_draw_rectangle(centro_x - ancho_caja/2, matriz_menu[i].bounding.uly,
                                              centro_x + ancho_caja/2, matriz_menu[i].bounding.dry,
                                              galaga_red, 2);
                        }

                        // Créditos abajo
                        al_draw_text(font, al_map_rgb(100,100,255), disAncho/2, disAlto - 30, ALLEGRO_ALIGN_CENTER, "INSERT COIN");
                    }

                    al_flip_display();
                    redraw = false;
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

//  al_draw_multiline_textf(font,themeslist[contador].color_texto,disAncho/2,disAlto/8,disAncho-disAncho/3,20,ALLEGRO_ALIGN_CENTER,"Bienvenido al juego de la vida, para empezar presione ESPACIO, para pasar una generacion presione ENTER (se recomienda dejar apretado para un mejor efecto), haga click en las celdas que desee matar o resucitar, mouse scroll para cambiar tamaño de pantalla (solo en este menu). Podra elegir cuantas generaciones escribiendo el numero de generaciones y apretando enter una vez iniciado el juego (El numero debe ser menor a %u, se recomienda que no sea un numero tan grande para que no vaya tan lento el programa),pasar '0' generaciones implicara pasar 1 generacion \n\nTema actual: %s",UINT_MAX,themeslist[contador].nombre);
