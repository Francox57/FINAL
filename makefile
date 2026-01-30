# --- VARIABLES ---
CC = gcc
CFLAGS = -Wall

# Librerías para PC (Allegro)
LIBS_PC = -lallegro -lallegro_image -lallegro_font -lallegro_ttf -lallegro_primitives -lallegro_audio -lallegro_acodec -lallegro_dialog -lallegro_color -lallegro_physfs -lallegro_video -lm

# --- OBJETIVOS ---

all: pc raspi

# 1. VERSIÓN PC
pc: main.c funciones.c niveles.c funciones.h
	@echo "Compilando version PC..."
	$(CC) $(CFLAGS) main.c funciones.c niveles.c -o juego_pc $(LIBS_PC)

# 2. VERSIÓN RASPI (CORREGIDA)
# Esta regla dice: "Necesito que existan disdrv.o y joydrv.o, pero NO se como fabricarlos, asi que espero que ya esten ahi"
raspi: probandocosas.c disdrv.o joydrv.o
	@echo "Compilando version Raspi..."
	$(CC) $(CFLAGS) probandocosas.c disdrv.o joydrv.o -o juego_rpi

# Limpieza
clean:
	rm -f juego_pc juego_rpi