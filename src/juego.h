#ifndef JUEGO_H_
#define JUEGO_H_
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "tp1.h"

typedef struct juego juego_t;

enum estado_juego {
	JUGANDO,
	TERMINADO,
};

enum resultado_jugada {
	JUGADA_INVALIDA = -1,
	PRIMERA_CARTA = 0,
	SEGUNDA_CARTA_LISTA = 1
};

//reserva memoria para un juego, se cargaran 9 pokemones de la pokedex
//si esta tiene menos pokemones, entonces se volverán a cargar los mismos hasta su tope.
juego_t *iniciar_juego(tp1_t *pokedex, unsigned int semilla);

//Retorna el estado de la jugada que se hizo.
enum resultado_jugada realizar_jugada(juego_t *juego, int num_carta);

//Si la jugada es la segunda y es valida, entonces se procesa guardandose en el historial
void procesar_jugada(juego_t *juego);

//imprime todo el juego por pantalla
void mostrar_juego(juego_t *juego);

//devuelve el estado de juego actual, puede ser JUGANDO o TERMINADO.
int estado_juego(juego_t *juego);

//Imprime por pantalla el jugador que tiene mas puntos
void mostrar_ganador(juego_t *juego);

//Libera la memoria relacionada al juego
void destruir_juego(juego_t *juego);

#endif // JUEGO_H_