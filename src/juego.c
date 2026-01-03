#include "juego.h"
#include "lista.h"
#include "baraja.h"
#include "tp1.h"
#include <string.h>
#include "ansi.h"
#include "define_programa.h"
#include <unistd.h>

#define MAX_JUGADORES 2
const size_t MAX_CARTAS = 18;

typedef struct jugador {
	int puntaje;
	lista_t *historial;
} jugador_t;

typedef struct jugada {
	int jugador;
	size_t carta_1;
	size_t carta_2;
	bool acierto;
} jugada_t;

struct juego {
	jugador_t jugadores[MAX_JUGADORES];
	int jugador_actual;
	lista_t *ultimas_jugadas;
	baraja_t *baraja_pok;
	jugada_t *jugada_actual;
};

typedef struct aux_historial {
	size_t idx_actual;
	size_t idx_inicio;
} aux_historial_t;

bool agregar_pok_baraja(struct pokemon *pok, void *baraja)
{
	baraja_t *baraja_pok = baraja;
	if (baraja_insertar(baraja_pok, pok) &&
	    baraja_insertar(baraja_pok, pok))
		return true;
	return false;
}

size_t agregar_pokemones_baraja(baraja_t *baraja, tp1_t *tp1)
{
	if (cantidad_baraja(baraja) >= MAX_CARTAS || tp1_cantidad(tp1) == 0)
		return 0;

	size_t agregados =
		2 * (tp1_con_cada_pokemon(tp1, agregar_pok_baraja, baraja) - 1);

	while 
	if (agregados == 0)
		return agregados;

	return agregados + agregar_pokemones_baraja(baraja, tp1);
}

juego_t *iniciar_juego(tp1_t *pokedex, unsigned int semilla)
{
	if (!pokedex) {
		return NULL;
	}

	juego_t *juego = malloc(sizeof(juego_t));
	if (!juego)
		return NULL;

	juego->jugador_actual = 0;
	juego->ultimas_jugadas = lista_crear();
	juego->baraja_pok = crear_baraja(MAX_CARTAS);
	juego->jugada_actual = NULL;

	if (!juego->ultimas_jugadas || !juego->baraja_pok) {
		lista_destruir(juego->ultimas_jugadas);
		destruir_baraja(juego->baraja_pok);
		free(juego);
		return NULL;
	}

	size_t cant_agregados =
		agregar_pokemones_baraja(juego->baraja_pok, pokedex);
	if (cant_agregados != MAX_CARTAS ||
	    !baraja_mezclar(juego->baraja_pok, semilla)) {
		lista_destruir(juego->ultimas_jugadas);
		destruir_baraja(juego->baraja_pok);
		free(juego);
		return NULL;
	}

	bool fallo = false;
	for (int i = 0; i < MAX_JUGADORES && !fallo; i++) {
		juego->jugadores[i].puntaje = 0;
		juego->jugadores[i].historial = lista_crear();
		if (!juego->jugadores[i].historial) {
			for (int j = 0; j < i; j++)
				lista_destruir(juego->jugadores[j].historial);
			lista_destruir(juego->ultimas_jugadas);
			destruir_baraja(juego->baraja_pok);
			free(juego);
			fallo = true;
		}
	}
	if (fallo)
		return NULL;

	return juego;
}

jugada_t *crear_primera_jugada(int jugador_actual, size_t pok_idx)
{
	jugada_t *jugada = calloc(1, sizeof(jugada_t));
	if (!jugada)
		return NULL;

	jugada->jugador = jugador_actual;
	jugada->carta_1 = pok_idx;
	return jugada;
}

enum resultado_jugada realizar_jugada(juego_t *juego, int num_carta)
{
	if (!juego)
		return JUGADA_INVALIDA;

	if (num_carta < 0 || num_carta >= MAX_CARTAS)
		return JUGADA_INVALIDA;

	size_t idx_carta_actual = (size_t)num_carta;
	int estado =
		baraja_estado_de_elemento(juego->baraja_pok, idx_carta_actual);
	if (estado == -1 || estado == VACIA || estado == BOCA_ARRIBA)
		return JUGADA_INVALIDA;
	else if (estado == BOCA_ABAJO)
		baraja_boca_arriba_elemento(juego->baraja_pok,
					    idx_carta_actual);

	struct pokemon *pokemon_actual =
		baraja_obtener(juego->baraja_pok, idx_carta_actual);
	if (!pokemon_actual)
		return JUGADA_INVALIDA;

	if (juego->jugada_actual == NULL) {
		juego->jugada_actual = crear_primera_jugada(
			juego->jugador_actual, idx_carta_actual);
		return PRIMERA_CARTA;
	}

	juego->jugada_actual->carta_2 = idx_carta_actual;

	return SEGUNDA_CARTA_LISTA;
}

void procesar_jugada(juego_t *juego)
{
	if (!juego || !juego->jugada_actual)
		return;

	size_t c1 = juego->jugada_actual->carta_1;
	size_t c2 = juego->jugada_actual->carta_2;
	struct pokemon *p1 = baraja_obtener(juego->baraja_pok, c1);
	struct pokemon *p2 = baraja_obtener(juego->baraja_pok, c2);
	if (!p1 || !p2)
		return;

	juego->jugada_actual->acierto = (p1->id == p2->id);

	lista_agregar(juego->ultimas_jugadas, juego->jugada_actual);
	lista_agregar(juego->jugadores[juego->jugador_actual].historial,
		      juego->jugada_actual);

	if (juego->jugada_actual->acierto) {
		juego->jugadores[juego->jugador_actual].puntaje++;
		baraja_eliminar(juego->baraja_pok, c1);
		baraja_eliminar(juego->baraja_pok, c2);
	} else {
		baraja_boca_abajo_elemento(juego->baraja_pok, c1);
		baraja_boca_abajo_elemento(juego->baraja_pok, c2);
		juego->jugador_actual =
			(juego->jugador_actual + 1) % MAX_JUGADORES;
	}

	juego->jugada_actual = NULL;
}

void imprimir_cartas(baraja_t *baraja)
{
	if (!baraja)
		return;
	int num_filas = 3;
	int num_columas = 6;
	size_t k = 0;
	for (size_t i = 0; i < num_filas; i++) {
		printf("\t\t");
		for (size_t j = 0; j < num_columas; j++) {
			int estado = baraja_estado_de_elemento(baraja, k);
			if (estado == BOCA_ABAJO) {
				printf("\t" ANSI_COLOR_BOLD
					       ANSI_COLOR_WHITE ANSI_BG_WHITE
				       "▐▁" ANSI_COLOR_BLACK
				       "%zu" ANSI_COLOR_WHITE
				       "▔▍" ANSI_BG_RESET ANSI_COLOR_RESET,
				       k);
			} else if (estado == VACIA) {
				printf("\t\t");
			} else if (estado == BOCA_ARRIBA) {
				struct pokemon *pok = baraja_obtener(baraja, k);
				printf("\t▐▁" ANSI_COLOR_MAGENTA ANSI_COLOR_BOLD
				       "%s" ANSI_COLOR_RESET "▔▍",
				       pok->nombre);
			}
			printf("   ");
			k++;
		}
		printf("\n\n\n");
	}
}

bool imprimir_cinco_ultimos(void *dato, void *extra)
{
	jugada_t *jugada = dato;
	aux_historial_t *aux_historial = extra;

	if (aux_historial->idx_actual >= aux_historial->idx_inicio) {
		printf("\t\t\t\t\t\t\tJugador %i: %zu-%zu ",
		       jugada->jugador + 1, jugada->carta_1, jugada->carta_2);
		if (jugada->acierto)
			printf(ANSI_COLOR_GREEN ANSI_COLOR_BOLD
			       "ACERTADO\n" ANSI_COLOR_RESET);
		else
			printf(ANSI_COLOR_RED ANSI_COLOR_BOLD
			       "FAILED\n" ANSI_COLOR_RESET);
	}
	aux_historial->idx_actual++;
	return true;
}

void mostrar_juego(juego_t *juego)
{
	printf("\n\n\t\t\t¡Bienvenido al juego de memoria! Todas las cartas están boca abajo. Elegí una posición \n \
		\tpara dar vuelta una carta y usá tu memoria para encontrar su pareja.\n \
		\t¡A ver cuántos pares podés descubrir!\n");
	printf("\n\n\t\t\tPuntos jugador 1:" ANSI_COLOR_GREEN ANSI_COLOR_BOLD
	       " %i ---------------------------------------------- " ANSI_COLOR_RESET
	       "puntos jugador 2: " ANSI_COLOR_GREEN ANSI_COLOR_BOLD
	       "%i\n \n" ANSI_COLOR_RESET,
	       juego->jugadores[0].puntaje, juego->jugadores[1].puntaje);
	imprimir_cartas(juego->baraja_pok);
	printf("\t\t\t" ANSI_COLOR_GREEN ANSI_COLOR_BOLD
	       " -------------------------------------------------------------------------------------- \n" ANSI_COLOR_RESET);
	aux_historial_t aux;
	size_t total = lista_cantidad(juego->ultimas_jugadas);
	if (total > 0) {
		aux.idx_inicio = (total > 5) ? (total - 5) : 0;
		aux.idx_actual = 0;
		printf("\t\t\t\t\t\t╔═══════════════════════════════════╗\n");
		lista_con_cada_elemento(juego->ultimas_jugadas,
					imprimir_cinco_ultimos, &aux);
		printf("\t\t\t\t\t\t╚═══════════════════════════════════╝\n\n");
	}
	printf("\t\t\tSelecciona una carta ingresando su numero.");
	printf("\t\t\tJugador %i:", juego->jugador_actual + 1);
}

int estado_juego(juego_t *juego)
{
	if (cantidad_baraja(juego->baraja_pok) == 0)
		return TERMINADO;
	else
		return JUGANDO;
}

void mostrar_ganador(juego_t *juego)
{
	int ganador =
		(juego->jugadores[0].puntaje > juego->jugadores[1].puntaje) ?
			1 :
			2;
	printf(POK_JIGGLY);
	printf(ANSI_COLOR_MAGENTA ANSI_COLOR_BOLD
	       "\t\t\tGanó el jugador numero %i\n" ANSI_COLOR_RESET,
	       ganador);
}

void destruir_juego(juego_t *juego)
{
	if (!juego)
		return;
	for (int i = 0; i < MAX_JUGADORES; i++)
		lista_destruir(juego->jugadores[i].historial);

	lista_destruir_todo(juego->ultimas_jugadas, free);
	destruir_baraja(juego->baraja_pok);
	free(juego);
}