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
    ALLEGRO_AUDIO_STREAM* music = al_load_audio_stream("resources/HarvestDawn.ogg",2, 2048); // musica
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
    ball1.vx = 0;
    ball1.vy = 0;
    float angulo = 90;
    float max_angulo = 70;
    float largo;
    float hit;
    char vidas = 3;
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
            matriz_menu[0].bounding = set_bounding(0.3333*disAncho,disAlto/3+lado*2,disAncho-(disAncho/3),disAlto/3+lado*4);
            matriz_menu[1].bounding = set_bounding(0.3333*disAncho,disAlto/3+lado*4,disAncho-(disAncho/3),disAlto/3+lado*6);
            matriz_menu[2].bounding = set_bounding(0.3333*disAncho,disAlto/3+lado*6,disAncho-(disAncho/3),disAlto/3+lado*8); 
        }
        



        al_wait_for_event(queue, &event); // indica al programa que revise si hay eventos

        if(event.type == ALLEGRO_EVENT_TIMER){ // si el usuario no ingresa nada simplemente se redibuja el estado en el que estaba
            if (fuera_mainmenu){
          
                if (collide(ball1.bounding.ulx,ball1.bounding.uly,ball1.bounding.drx,ball1.bounding.dry,-10,0,0,disAlto))
                {
                    ball1.vx = -ball1.vx;
                    ball1.x = radio+1;
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
                    ball1.x = (float)disAncho/2;
                    ball1.y = disAlto - ((disAlto-((float)disAlto*0.90))+(6*anchorect)/32);
                    ball1.bounding =  set_bounding(ball1.x-radio,(ball1.y-radio),ball1.x+radio,(ball1.y+radio));
                    platform.bounding = set_bounding(((float)disAncho/2)-2.5*anchoplat,disAlto - (disAlto-((float)disAlto*0.90)),((float)disAncho/2)+2.5*anchoplat,disAlto-(disAlto-((float)disAlto*0.90)-16));
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
                            mat[i][j].estado = false;
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
        else if (event.type == ALLEGRO_EVENT_KEY_CHAR){  // Se fija si se toco alguna tecla
            if (event.keyboard.keycode == ALLEGRO_KEY_SPACE && !fuera_mainmenu){
                for (int i = 0; i < ALTO; i++){
                    for (int j = 0; j < ANCHO; j++)
                    {
                        mat[i][j].bounding.ulx = i*(float)disAncho/ANCHO;
                        mat[i][j].bounding.uly = j*lado;
                        mat[i][j].bounding.drx = i*(float)disAncho/ANCHO+disAlto/ALTO;
                        mat[i][j].bounding.dry = j*lado+disAncho/ANCHO;
                        mat[i][j].estado = true;
                        mat[i][j].cant_impactos_total = 1;
                        mat[i][j].cant_impactos_actual = mat[i][j].cant_impactos_total;
                        mat[i][j].bounding = set_bounding(j*(float)disAncho/ANCHO,i*lado,j*(float)disAncho/ANCHO+(float)disAncho/ANCHO,i*lado+lado);
                    }
                }
                largo = platform.bounding.drx - platform.bounding.ulx; // Si se toca el espacio y el juego no se inicio
                platform.x = largo/2;
                fuera_mainmenu = true; // da la orden para poder iniciar el jueg
                al_clear_to_color(themeslist[contador].color_pantalla); // limpia la pantalla y saca el texto
            }else if (event.keyboard.keycode == ALLEGRO_KEY_SPACE && fuera_mainmenu && (ball1.vx == 0 && ball1.vy == 0)){
                    if(game_over){
                        vidas = 3;
                        game_over = false;
                        for (int i = 0; i < ALTO; i++)
                        {
                            for (int j = 0; j < ANCHO; j++)
                            {
                                mat[i][j].estado = true;
                                mat[i][j].cant_impactos_actual = mat[i][j].cant_impactos_total;
                            }
                            
                        }
                    }
                    angulo = (rand() % 100)+15;
                    ball1.vx = (float)ball1.dx*cos(DEGTORAD(angulo));
                    ball1.vy = -((float)ball1.dy*sin(DEGTORAD(angulo)));
            } 
            else if ((event.keyboard.keycode == ALLEGRO_KEY_RIGHT || event.keyboard.keycode == ALLEGRO_KEY_LEFT) && !fuera_mainmenu){ // Se fija cuando se toca las teclas y si esta en el menu
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
            else if (((event.keyboard.keycode == ALLEGRO_KEY_A || event.keyboard.keycode == ALLEGRO_KEY_D) && fuera_mainmenu) && (ball1.vx != 0 || ball1.vy != 0)){
                float leftside = platform.bounding.ulx;
                float rightside = platform.bounding.drx;
                if(event.keyboard.keycode == ALLEGRO_KEY_A){
                    if(leftside > 0){
                        platform.bounding.ulx -= platform.dx;
                        platform.bounding.drx -= platform.dx;
                        al_clear_to_color(themeslist[contador].color_pantalla);
                        redraw = true;
                    }
                }
                if(event.keyboard.keycode == ALLEGRO_KEY_D){
                    if(rightside < disAncho){
                        platform.bounding.ulx += platform.dx;
                        platform.bounding.drx += platform.dx;
                        al_clear_to_color(themeslist[contador].color_pantalla);
                        redraw = true;
                    }
                }
            }
            
            
            
        }else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && !fuera_mainmenu){
            if (collide(event.mouse.x,event.mouse.y,event.mouse.x,event.mouse.y,matriz_menu[0].bounding.ulx,matriz_menu[0].bounding.uly,matriz_menu[0].bounding.drx,matriz_menu[0].bounding.dry)){
                fuera_mainmenu = true;
                for (int i = 0; i < ALTO; i++){
                    for (int j = 0; j < ANCHO; j++)
                    {
                        mat[i][j].bounding.ulx = i*(float)disAncho/ANCHO;
                        mat[i][j].bounding.uly = j*lado;
                        mat[i][j].bounding.drx = i*(float)disAncho/ANCHO+disAlto/ALTO;
                        mat[i][j].bounding.dry = j*lado+disAncho/ANCHO;
                        mat[i][j].estado = true;
                        mat[i][j].cant_impactos_total = 1;
                        mat[i][j].cant_impactos_actual = mat[i][j].cant_impactos_total;
                        mat[i][j].bounding = set_bounding(j*(float)disAncho/ANCHO,i*lado,j*(float)disAncho/ANCHO+(float)disAncho/ANCHO,i*lado+lado);
                    }
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
        
               

        if(redraw && al_is_event_queue_empty(queue)) // se fija que se pueda redibujar y que no haya eventos en la fila de eventos
        {   
            if(fuera_mainmenu){
            dibujar_all(disAlto,disAncho,lado,themeslist[contador].color_lineas,platform.bounding,ball1,mat);
            al_flip_display();
            if (game_over){
                al_draw_multiline_text(gameoverfont,themeslist[contador].color_texto,disAncho/2,disAlto/2,disAncho-disAncho/3,30,ALLEGRO_ALIGN_CENTER,"GAME OVER\n PRESIONE [ESC] PARA IR AL MENU PRINCIPAL O [SPACE] PARA VOLVER A INTENTAR");}
                al_flip_display();
            redraw = false;
            }else{ // Si no se inicio el programa se pone el menu
                al_draw_text(gameoverfont,themeslist[contador].color_texto,disAncho/2,matriz_menu[0].bounding.dry-(matriz_menu[0].bounding.dry-matriz_menu[0].bounding.uly)*0.75,ALLEGRO_ALIGN_CENTER,"JUGAR");
                al_draw_text(gameoverfont,themeslist[contador].color_texto,disAncho/2,matriz_menu[1].bounding.dry-(matriz_menu[1].bounding.dry-matriz_menu[1].bounding.uly)*0.75,ALLEGRO_ALIGN_CENTER,"OPCIONES");
                al_draw_text(gameoverfont,themeslist[contador].color_texto,disAncho/2,matriz_menu[2].bounding.dry-(matriz_menu[2].bounding.dry-matriz_menu[2].bounding.uly)*0.75,ALLEGRO_ALIGN_CENTER,"SALIR");


                al_draw_rectangle(matriz_menu[0].bounding.ulx,matriz_menu[0].bounding.uly,matriz_menu[0].bounding.drx,matriz_menu[0].bounding.dry,al_map_rgb(255, 92, 194),1);
                al_draw_rectangle(matriz_menu[1].bounding.ulx,matriz_menu[1].bounding.uly,matriz_menu[1].bounding.drx,matriz_menu[1].bounding.dry,al_map_rgb(255, 92, 194),1);
                al_draw_rectangle(matriz_menu[2].bounding.ulx,matriz_menu[2].bounding.uly,matriz_menu[2].bounding.drx,matriz_menu[2].bounding.dry,al_map_rgb(255, 92, 194),1);
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

//  al_draw_multiline_textf(font,themeslist[contador].color_texto,disAncho/2,disAlto/8,disAncho-disAncho/3,20,ALLEGRO_ALIGN_CENTER,"Bienvenido al juego de la vida, para empezar presione ESPACIO, para pasar una generacion presione ENTER (se recomienda dejar apretado para un mejor efecto), haga click en las celdas que desee matar o resucitar, mouse scroll para cambiar tamaño de pantalla (solo en este menu). Podra elegir cuantas generaciones escribiendo el numero de generaciones y apretando enter una vez iniciado el juego (El numero debe ser menor a %u, se recomienda que no sea un numero tan grande para que no vaya tan lento el programa),pasar '0' generaciones implicara pasar 1 generacion \n\nTema actual: %s",UINT_MAX,themeslist[contador].nombre);