#include "funciones.h"





typedef struct {
    float x,y;
    float dx,dy;
} entities;

int main()
{
    
    srand(time(NULL)); // semilla random para la funcion rand()
    int mat [ALTO][ANCHO]; // matriz principal
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
    ALLEGRO_AUDIO_STREAM* music = al_load_audio_stream("resources/HarvestDawn.ogg",2, 2048); // musica
    ALLEGRO_FONT* font = al_load_ttf_font("resources/OpenSans.ttf",15,0); // fuente
    must_init(disp,"display"); // se verifica que estos punteros se hayan inicializado correctamente
    must_init(disp,"timer");
    must_init(disp,"queue");
    must_init(font,"font");
    must_init(music, "music");
    bool redraw = true; // variable que admite el redibujar la pantalla
    bool cerrar = false; // variable que cierra todo el programa
    bool inicio = false; // variable que va dar inicio al juego y la impresion de la grilla y los casilleros correspondientes
    int contador = 0; // variable que sirve para determinar que tema usar
    

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
    //////////////////////


    llenar_mat(mat); // llena la matriz de espacios
    al_start_timer(timer); // empieza el timer

    while(!cerrar)
    {   
        float lado = (disAncho/(float)ANCHO) < ((disAlto/2)/(float)ALTO) ? disAncho/(float)ANCHO : (disAlto/2)/(float)ALTO; // cuanto mide los lados verticales de las casillas
        THEMES themeslist[MAXTHEMES] = {dark,light,bluescreen,random}; // Arreglo de los temas disponibles
        if (!inicio){
            platform.x = (float)disAncho/2;
            platform.y = disAlto-((float)disAlto*0.90);

            ball1.x = platform.x;
            ball1.y = platform.y;
        }
        
        al_wait_for_event(queue, &event); // indica al programa que revise si hay eventos

        if(event.type == ALLEGRO_EVENT_TIMER){ // si el usuario no ingresa nada simplemente se redibuja el estado en el que estaba
            redraw = true;
        }
        else if((event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)){ // si se apreta la X de la ventana se cierra el programa
            cerrar = true;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_CHAR){  // Se fija si se toco alguna tecla
            if (event.keyboard.keycode == ALLEGRO_KEY_SPACE && !inicio){ // Si se toca el espacio y el juego no se inicio
                inicio = true; // da la orden para poder iniciar el jueg
                al_clear_to_color(themeslist[contador].color_pantalla); // limpia la pantalla y saca el texto
            }else if ((event.keyboard.keycode == ALLEGRO_KEY_RIGHT || event.keyboard.keycode == ALLEGRO_KEY_LEFT) && !inicio){ // Se fija cuando se toca las teclas y si esta en el menu
                if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT){ // en caso de que se toca la flecha derecha
                    if (contador >= 0 && contador <= MAXTHEMES-1){ 
                        contador++; // Se aumenta un contador que determinara que tema usar
                        
                        if (contador > MAXTHEMES-1){
                            contador = 0;
                        }
                    }
                }else{ // en caso de que se toca la flecha izquierda
                    if (contador >= 0 && contador <= MAXTHEMES-1){
                        contador--; // Se decrementa un contador que determinara que tema usar
                        if (contador < 0)
                        {
                            contador = MAXTHEMES-1;
                        }
                    }
                }

                if (contador == MAXTHEMES-1){ // En caso de que sea la 4 opcion (random)
                random.color_casilleros = al_map_rgb(rand()%256,rand()%256,rand()% 256); // Se randomiza los colores de la cuarta opcion
                random.color_pantalla = al_map_rgb(rand()%256,rand()%256,rand()% 256);
                random.color_texto = al_map_rgb(rand()%256,rand()%256,rand()% 256);
                random.color_lineas = al_map_rgb(rand()%256,rand()%256,rand()% 256);
                }
                
                
            al_clear_to_color(themeslist[contador].color_pantalla);   // se limpia la pnatalla  
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){ // Si se toca escape se sale del programa
                cerrar = true;
            }
            else if ((event.keyboard.keycode == ALLEGRO_KEY_A || event.keyboard.keycode == ALLEGRO_KEY_D) && inicio){
                float leftside = platform.x-3*(float)disAncho/ANCHO;;
                float rightside = platform.x+3*(float)disAncho/ANCHO;;
                if(event.keyboard.keycode == ALLEGRO_KEY_A){
                    if(leftside > 0){
                        platform.x = platform.x-platform.dx;
                        al_clear_to_color(themeslist[contador].color_pantalla);
                        redraw = true;
                    }
                }
                if(event.keyboard.keycode == ALLEGRO_KEY_D){
                    if(rightside < disAncho){
                        platform.x = platform.x+platform.dx;
                        al_clear_to_color(themeslist[contador].color_pantalla);
                        redraw = true;
                    }
                }
            }
            
            
            
        }
        else if (event.type == ALLEGRO_EVENT_MOUSE_AXES && event.mouse.dz != 0 && !inicio){ // Detecta si se movio la rueda del boton y si esta en el menu
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
            if (disAlto < 425){ // impide que el display sea muy pequeño para que las letras entren
                disAlto = 425;
            }
            if (disAncho < 425){ // impide que el display sea muy pequeño para que las letras entren
                disAncho = 425;
            }
            al_resize_display(disp,disAncho,disAlto); // Cambia tamaño de pantalla
            al_clear_to_color(themeslist[contador].color_pantalla); // limpia la pantalla para que no se escriba texto por encima 
        }
        
               

        if(redraw && al_is_event_queue_empty(queue)) // se fija que se pueda redibujar y que no haya eventos en la fila de eventos
        {   
            if(inicio){
            dibujar_all(disAlto,disAncho,lado,themeslist[contador].color_lineas,platform.x,platform.y,ball1.x,ball1.y);
            al_flip_display();
            redraw = false;
            }else{ // Si no se inicio el programa se pone el menu
                al_draw_multiline_textf(font,themeslist[contador].color_texto,disAncho/2,disAlto/8,disAncho-disAncho/3,20,ALLEGRO_ALIGN_CENTER,"Bienvenido al juego de la vida, para empezar presione ESPACIO, para pasar una generacion presione ENTER (se recomienda dejar apretado para un mejor efecto), haga click en las celdas que desee matar o resucitar, mouse scroll para cambiar tamaño de pantalla (solo en este menu). Podra elegir cuantas generaciones escribiendo el numero de generaciones y apretando enter una vez iniciado el juego (El numero debe ser menor a %u, se recomienda que no sea un numero tan grande para que no vaya tan lento el programa),pasar '0' generaciones implicara pasar 1 generacion \n\nTema actual: %s",UINT_MAX,themeslist[contador].nombre); // Texto de bienvenida del menu
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
    return 0;
}
