#include "lista.h"

typedef struct nodo nodo_t;

struct nodo {
	nodo_t *siguiente;
	void *dato;
};

struct lista {
	nodo_t *primer_elemento;
	nodo_t *ultimo_elemento;
	size_t cantidad;
};

struct lista_iterador {
	nodo_t *actual;
};

lista_t *lista_crear()
{
	lista_t *nueva_lista = malloc(sizeof(lista_t));
	if (!nueva_lista) {
		return NULL;
	}

	nueva_lista->primer_elemento = NULL;
	nueva_lista->ultimo_elemento = NULL;
	nueva_lista->cantidad = 0;
	return nueva_lista;
}

bool lista_vacia(lista_t *lista)
{
	if (!lista) {
		return true;
	}
	return lista->cantidad == 0 && lista->primer_elemento == NULL &&
	       lista->ultimo_elemento == NULL;
}

size_t lista_cantidad(lista_t *lista)
{
	if (!lista) {
		return 0;
	}
	return lista->cantidad;
}

nodo_t *crear_nodo_con_dato(void *dato)
{
	nodo_t *nuevo_nodo = malloc(sizeof(nodo_t));
	if (!nuevo_nodo) {
		return NULL;
	}

	nuevo_nodo->dato = dato;
	nuevo_nodo->siguiente = NULL;

	return nuevo_nodo;
}

bool lista_agregar(lista_t *lista, void *dato)
{
	if (!lista) {
		return false;
	}

	nodo_t *nuevo_elemento = crear_nodo_con_dato(dato);
	if (!nuevo_elemento) {
		return false;
	}

	if (lista_vacia(lista)) {
		lista->primer_elemento = nuevo_elemento;
		lista->ultimo_elemento = nuevo_elemento;
	} else {
		lista->ultimo_elemento->siguiente = nuevo_elemento;
		lista->ultimo_elemento = nuevo_elemento;
	}
	lista->cantidad++;

	return true;
}

bool lista_insertar(lista_t *lista, void *elemento, size_t posicion)
{
	if (!lista || posicion >= lista->cantidad || lista_vacia(lista)) {
		return false;
	}

	nodo_t *nuevo_nodo = crear_nodo_con_dato(elemento);
	if (!nuevo_nodo) {
		return false;
	}

	if (posicion == 0) {
		nuevo_nodo->siguiente = lista->primer_elemento;
		lista->primer_elemento = nuevo_nodo;
	} else {
		nodo_t *actual = lista->primer_elemento;
		for (size_t i = 0; i < posicion - 1; i++) {
			actual = actual->siguiente;
		}
		nuevo_nodo->siguiente = actual->siguiente;
		actual->siguiente = nuevo_nodo;
	}

	lista->cantidad++;
	return true;
}

void *lista_eliminar_elemento(lista_t *lista, size_t posicion)
{
	if (!lista || lista_vacia(lista) || posicion >= lista->cantidad) {
		return NULL;
	}

	nodo_t *nodo_a_eliminar = lista->primer_elemento;
	void *elemento = nodo_a_eliminar->dato;

	if (posicion == 0) {
		lista->primer_elemento = nodo_a_eliminar->siguiente;
		if (lista->cantidad == 1) {
			lista->ultimo_elemento = NULL;
		}
	} else if (posicion > 0) {
		nodo_t *actual = lista->primer_elemento;
		for (size_t i = 0; i < posicion - 1; i++) {
			actual = actual->siguiente;
		}
		nodo_a_eliminar = actual->siguiente;
		elemento = nodo_a_eliminar->dato;
		actual->siguiente = nodo_a_eliminar->siguiente;
		if (posicion == lista->cantidad - 1) {
			lista->ultimo_elemento = actual;
		}
	}

	free(nodo_a_eliminar);
	lista->cantidad--;
	return elemento;
}

int lista_buscar_posicion(lista_t *lista, void *elemento,
			  int (*comparador)(const void *, const void *))
{
	if (!lista || !comparador) {
		return -1;
	}
	int posicion = -1;

	nodo_t *actual = lista->primer_elemento;
	int i = 0;
	bool encontrado = false;
	while (actual != NULL && !encontrado) {
		if (comparador(actual->dato, elemento) == 0) {
			posicion = i;
			encontrado = true;
		}
		actual = actual->siguiente;
		i++;
	}

	return posicion;
}

void *lista_buscar_elemento(lista_t *lista, size_t posicion)
{
	if (!lista || posicion >= lista->cantidad) {
		return NULL;
	}

	if (posicion == 0) {
		return lista->primer_elemento->dato;
	} else if (posicion == lista->cantidad - 1) {
		return lista->ultimo_elemento->dato;
	}

	nodo_t *actual = lista->primer_elemento;
	size_t i = 0;
	while (i != posicion) {
		actual = actual->siguiente;
		i++;
	}

	return actual->dato;
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*f)(void *, void *),
			       void *extra)
{
	if (!lista || !f || lista_vacia(lista)) {
		return 0;
	}

	nodo_t *actual = lista->primer_elemento;
	size_t cantidad = 0;
	bool seguir = true;
	while (actual != NULL && seguir) {
		seguir = f(actual->dato, extra);
		actual = actual->siguiente;
		cantidad++;
	}
	return cantidad;
}

void lista_destruir_todo(lista_t *lista, void (*destructor)(void *))
{
	if (!lista) {
		return;
	}
	nodo_t *actual = lista->primer_elemento;
	while (actual != NULL) {
		nodo_t *nodo_a_eliminar = actual;
		actual = nodo_a_eliminar->siguiente;
		if (destructor) {
			destructor(nodo_a_eliminar->dato);
		}
		free(nodo_a_eliminar);
	}
	free(lista);
}

void lista_destruir(lista_t *lista)
{
	if (!lista) {
		return;
	}
	lista_destruir_todo(lista, NULL);
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (!lista) {
		return NULL;
	}

	lista_iterador_t *lista_i = malloc(sizeof(lista_iterador_t));
	if (!lista_i) {
		return NULL;
	}

	lista_i->actual = lista->primer_elemento;

	return lista_i;
}

bool lista_iterador_hay_mas_elementos(lista_iterador_t *it)
{
	if (!it) {
		return false;
	}
	return it->actual != NULL;
}

void lista_iterador_siguiente(lista_iterador_t *it)
{
	if (!it || !it->actual) {
		return;
	}
	it->actual = it->actual->siguiente;
}

void *lista_iterador_obtener_actual(lista_iterador_t *it)
{
	if (!it || !it->actual) {
		return NULL;
	}

	return it->actual->dato;
}

void lista_iterador_destruir(lista_iterador_t *it)
{
	if (!it) {
		return;
	}

	free(it);
}