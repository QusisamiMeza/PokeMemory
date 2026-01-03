#include "baraja.h"
#include <time.h>

typedef struct carta {
	void *dato;
	enum estado estado;
} carta_t;

struct baraja {
	size_t cantidad;
	size_t capacidad;
	carta_t *cartas;
};

//Retorna true si la carta esta ocupada, osea si su estado esta BOCA_ABAJO o BOCA_ARRIBA
bool es_carta_ocupada(carta_t carta)
{
	return carta.estado == BOCA_ABAJO || carta.estado == BOCA_ARRIBA;
}

baraja_t *crear_baraja(size_t capacidad)
{
	if (capacidad == 0)
		return NULL;

	baraja_t *baraja = calloc(1, sizeof(baraja_t));
	if (!baraja)
		return NULL;

	baraja->cartas = calloc(capacidad, sizeof(carta_t));
	if (!baraja->cartas) {
		free(baraja);
		return NULL;
	}

	baraja->capacidad = capacidad;

	return baraja;
}

size_t cantidad_baraja(baraja_t *baraja)
{
	if (!baraja)
		return 0;

	return baraja->cantidad;
}

bool baraja_mezclar(baraja_t *baraja, unsigned int semilla)
{
	if (!baraja)
		return false;

	srand(semilla);

	for (size_t i = baraja->capacidad - 1; i > 0; i--) {
		size_t j = (size_t)rand() % (i + 1);

		carta_t temp = baraja->cartas[i];
		baraja->cartas[i] = baraja->cartas[j];
		baraja->cartas[j] = temp;
	}

	return true;
}

bool baraja_insertar(baraja_t *baraja, void *dato)
{
	if (!baraja || baraja->capacidad == baraja->cantidad)
		return false;

	bool agregado = false;
	for (size_t i = 0; i < baraja->capacidad && !agregado; i++) {
		if (!es_carta_ocupada(baraja->cartas[i])) {
			baraja->cartas[i].dato = dato;
			baraja->cartas[i].estado = BOCA_ABAJO;
			agregado = true;
			baraja->cantidad++;
		}
	}

	return agregado;
}

void *baraja_eliminar(baraja_t *baraja, size_t posicion)
{
	if (!baraja || posicion >= baraja->capacidad ||
	    !es_carta_ocupada(baraja->cartas[posicion]))
		return NULL;

	baraja->cartas[posicion].estado = VACIA;
	baraja->cantidad--;
	void *dato = baraja->cartas[posicion].dato;
	baraja->cartas[posicion].dato = NULL;
	return dato;
}

void *baraja_obtener(baraja_t *baraja, size_t posicion)
{
	if (!baraja || posicion >= baraja->capacidad ||
	    baraja->cartas[posicion].estado != BOCA_ARRIBA)
		return NULL;

	return baraja->cartas[posicion].dato;
}

int baraja_estado_de_elemento(baraja_t *baraja, size_t posicion)
{
	if (!baraja || posicion >= baraja->capacidad)
		return -1;

	return baraja->cartas[posicion].estado;
}

int baraja_posicion(baraja_t *baraja, void *elemento,
		    int (*comparador)(void *, void *))
{
	if (!baraja || !comparador)
		return -1;

	bool encontrado = false;
	int posicion = -1;
	for (size_t i = 0; i < baraja->capacidad && !encontrado; i++) {
		if (es_carta_ocupada(baraja->cartas[i]) &&
		    comparador(baraja->cartas[i].dato, elemento) == 0) {
			posicion = (int)i;
			encontrado = true;
		}
	}

	return posicion;
}

bool baraja_boca_arriba_elemento(baraja_t *baraja, size_t posicion)
{
	if (!baraja || posicion >= baraja->capacidad ||
	    baraja->cartas[posicion].estado != BOCA_ABAJO)
		return false;

	baraja->cartas[posicion].estado = BOCA_ARRIBA;
	return true;
}

bool baraja_boca_abajo_elemento(baraja_t *baraja, size_t posicion)
{
	if (!baraja || posicion >= baraja->capacidad ||
	    baraja->cartas[posicion].estado != BOCA_ARRIBA)
		return false;

	baraja->cartas[posicion].estado = BOCA_ABAJO;
	return true;
}

size_t baraja_iterador(baraja_t *baraja, enum baraja_recorrido recorrido,
		       bool (*f)(void *, void *), void *extra)
{
	if (!baraja || baraja->cantidad == 0)
		return 0;

	size_t contador = 0;
	bool seguir = true;

	switch (recorrido) {
	case BARAJA_RECORRER_TODAS:
		for (size_t i = 0; i < baraja->capacidad && seguir; i++) {
			if (es_carta_ocupada(baraja->cartas[i])) {
				seguir = f(baraja->cartas[i].dato, extra);
				contador++;
			}
		}
		break;
	case BARAJA_RECORRER_BOCA_ABAJO:
		for (size_t i = 0; i < baraja->capacidad && seguir; i++) {
			if (baraja->cartas[i].estado == BOCA_ABAJO) {
				seguir = f(baraja->cartas[i].dato, extra);
				contador++;
			}
		}
		break;
	case BARAJA_RECORRER_BOCA_ARRIBA:
		for (size_t i = 0; i < baraja->capacidad && seguir; i++) {
			if (baraja->cartas[i].estado == BOCA_ARRIBA) {
				seguir = f(baraja->cartas[i].dato, extra);
				contador++;
			}
		}
		break;
	}

	return contador;
}

void destruir_baraja(baraja_t *baraja)
{
	destruir_baraja_todo(baraja, NULL);
}

void destruir_baraja_todo(baraja_t *baraja, void (*destructor)(void *))
{
	if (!baraja)
		return;

	if (destructor) {
		for (size_t i = 0; i < baraja->capacidad; i++) {
			if (es_carta_ocupada(baraja->cartas[i]))
				destructor(baraja->cartas[i].dato);
		}
	}
	free(baraja->cartas);
	free(baraja);
}