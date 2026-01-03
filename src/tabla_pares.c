#include "tabla_pares.h"
#include <string.h>

// Crea y devuelve una copia dinámica de la clave.
char *copiar_clave(const char *clave)
{
	char *copia = calloc(strlen(clave) + 1, sizeof(char));
	if (!copia) {
		return NULL;
	}
	return strcpy(copia, clave);
}

int comparador_par_clave(const void *a, const void *b)
{
	const par_t *actual = a;
	const char *clave = b;
	return strcmp(actual->clave, clave);
}

lista_t **crear_tabla(size_t capacidad)
{
	lista_t **nueva_tabla = calloc(capacidad, sizeof(lista_t *));
	if (!nueva_tabla) {
		return NULL;
	}

	bool fallo = false;
	for (size_t i = 0; i < capacidad && !fallo; i++) {
		nueva_tabla[i] = lista_crear();
		if (!nueva_tabla[i]) {
			fallo = true;
			for (size_t j = 0; j < i; j++) {
				lista_destruir(nueva_tabla[j]);
			}
			free(nueva_tabla);
			nueva_tabla = NULL;
		}
	}

	return nueva_tabla;
}

par_t *crear_par(char *clave, void *valor)
{
	if (!clave) {
		return NULL;
	}

	par_t *nuevo_par = calloc(1, sizeof(par_t));
	char *copia_clave = copiar_clave(clave);

	if (!nuevo_par || !copia_clave) {
		free(copia_clave);
		free(nuevo_par);
		return NULL;
	}

	nuevo_par->valor = valor;
	nuevo_par->clave = copia_clave;
	return nuevo_par;
}

size_t iterador_bucket(lista_t *lista_tabla, bool (*f)(char *, void *, void *),
		       void *ctx, bool *seguir)
{
	lista_iterador_t *iterador = lista_iterador_crear(lista_tabla);
	size_t cantidad = 0;

	while (lista_iterador_hay_mas_elementos(iterador) && (*seguir)) {
		par_t *actual = lista_iterador_obtener_actual(iterador);
		*seguir = f(actual->clave, actual->valor, ctx);
		lista_iterador_siguiente(iterador);
		cantidad++;
	}
	lista_iterador_destruir(iterador);
	return cantidad;
}

size_t iterador_bucket_rehash(lista_t *lista_tabla,
			      bool (*f)(char *, void *, void *), void *ctx,
			      bool *seguir)
{
	lista_iterador_t *iterador = lista_iterador_crear(lista_tabla);
	size_t cantidad = 0;

	while (lista_iterador_hay_mas_elementos(iterador) && (*seguir)) {
		par_t *actual = lista_iterador_obtener_actual(iterador);
		*seguir = f(actual->clave, actual, ctx);
		lista_iterador_siguiente(iterador);
		cantidad++;
	}
	lista_iterador_destruir(iterador);
	return cantidad;
}

par_t *encontrar_par(char *clave, lista_t *lista_tabla)
{
	lista_iterador_t *iterador = lista_iterador_crear(lista_tabla);
	par_t *par = NULL;

	bool encontrado = false;
	while (lista_iterador_hay_mas_elementos(iterador) && !encontrado) {
		void *actual = lista_iterador_obtener_actual(iterador);
		if (comparador_par_clave(actual, clave) == 0) {
			par = actual;
			encontrado = true;
		}
		lista_iterador_siguiente(iterador);
	}
	lista_iterador_destruir(iterador);
	return par;
}

void destruir_par(void *par_a)
{
	par_t *par = par_a;
	free(par->clave);
	free(par);
}

bool destruir_par_valor(void *par_a, void *ctx)
{
	par_t *par = par_a;
	aux_destruir_contenido_t *aux = ctx;
	aux->destructor(par->valor);
	return true;
}

void destruir_tabla(lista_t **tabla, size_t capacidad)
{
	for (size_t i = 0; i < capacidad; i++) {
		lista_destruir(tabla[i]);
	}
	free(tabla);
}
