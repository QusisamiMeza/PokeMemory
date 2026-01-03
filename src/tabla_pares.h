#ifndef __TABLA_PARES_H__
#define __TABLA_PARES_H__

#include <stdbool.h>
#include <stddef.h>
#include "lista.h"

typedef struct par {
	char *clave;
	void *valor;
} par_t;

typedef struct aux_destruir_contenido {
	void (*destructor)(void *);
} aux_destruir_contenido_t;

//Reserva memoria para una tabla de `capacidad` listas.
lista_t **crear_tabla(size_t capacidad);

//Reserva memoria para un `par_t` y lo inicializa
par_t *crear_par(char *clave, void *valor);

//Itera en un determinado bucket buscando el par asociado a la clave, en caso de encontrarlo lo devuelve
//si no lo encuentra, devuelve NULL.
par_t *encontrar_par(char *clave, lista_t *lista_tabla);

//Devuelve 0 si el string a(clave) es igual a la clave del par b
int comparador_par_clave(const void *a, const void *b);

//Itera en un determinado bucket hasta que `f` de false. Devuelve la cantidad de elementos iterados
size_t iterador_bucket(lista_t *lista_tabla, bool (*f)(char *, void *, void *),
		       void *ctx, bool *seguir);

//Itera en un determinado bucket hasta que `f` de false. Devuelve la cantidad de pares iterados
size_t iterador_bucket_rehash(lista_t *lista_tabla,
			      bool (*f)(char *, void *, void *), void *ctx,
			      bool *seguir);

//Libera la memoria del valor en `par_a` con el destructor pasado por el parametro `ctx`
bool destruir_par_valor(void *par_a, void *ctx);

//Libera la memoria relacionada con `par_t`
void destruir_par(void *par);

//Libera la memoria toda la memoria de la tabla
void destruir_tabla(lista_t **tabla, size_t capacidad);

#endif /* __TABLA_PARES_H__ */