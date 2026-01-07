CC := gcc
CFLAGS := -Wall
AFLAGS := -lallegro -lallegro_image -lallegro_font -lallegro_ttf -lallegro_primitives -lallegro_audio -lallegro_acodec -lallegro_dialog -lallegro_color -lallegro_physfs -lallegro_video
OBJS := main.o funciones.o

juego: ${OBJS}
	${CC} ${OBJS} -o juego ${AFLAGS}

main.o: main.c funciones.h
	${CC} ${CFLAGS} -c main.c

funciones.o: funciones.c funciones.h
	${CC} ${CFLAGS} -c funciones.c
	
clean:
	rm -f *.o juego