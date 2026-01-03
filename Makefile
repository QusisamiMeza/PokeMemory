VALGRIND_FLAGS=--leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all --trace-children=yes
CFLAGS =-std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g
CC = gcc

TEST_BIN = pruebas_alumno
MAIN_BIN = juego

ARCH_TDA_ACTUAL = src/menu.* src/juego.* src/baraja.* src/comando.*

FORMAT = clang-format -i -style=file
ZIP = zip tp2.zip

TEST_C = pruebas_alumno.c
ARCH_VARIOS = Enunciado.md README.md Makefile pa2m.h pruebas_alumno.c

all: clean valgrind-alumno

valgrind-alumno: pruebas_alumno
	valgrind $(VALGRIND_FLAGS) ./pruebas_alumno

pruebas_alumno: src/*.c pruebas_alumno.c
	$(CC) $(CFLAGS) src/*.c $(TEST_C) -o pruebas_alumno

clean:
	rm -f pruebas_alumno 

compilar:
	$(CC) $(CFLAGS) tp2.c src/*.c -o $(MAIN_BIN)

format:
	$(FORMAT) $(TEST_C) tp2.c src/*.c src/*.h

zip: src/*.c src/*.h format
	$(ZIP) src/*.c src/*.h img/* ejemplos/*.csv tp2.c $(ARCH_VARIOS)

juego: compilar
	./juego $(ARCH)

juego-sin-arch: compilar
	./juego

juego-valgrind: compilar
	valgrind $(VALGRIND_FLAGS) ./juego $(ARCH)

juego-sin-arch-valgrind: compilar
	valgrind $(VALGRIND_FLAGS) ./juego