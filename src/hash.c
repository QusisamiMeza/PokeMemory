#include "hash.h"
#include "tabla_pares.h"
#include <string.h>

const size_t CAPACIDAD_MINIMA = 3;
const size_t FACTOR_CRECIMIENTO = 2;
const double FACTOR_CARGA_MAX = 0.75;

struct hash {
	lista_t **tabla;
	size_t cantidad;
	size_t capacidad;
};

//Calcula el valor hash de una cadena utilizando el algoritmo djb2.
size_t hash_djb2(char *str)
{
	size_t hash = 5381;
	int c;

	while ((c = *str++)) {
		hash = ((hash << 5) + hash) + (size_t)c;
	}

	return hash;
}

//Calcula el índice dentro de la tabla hash correspondiente a una cadena dada.
size_t func_hash(char *str, size_t capacidad)
{
	return hash_djb2(str) % capacidad;
}

hash_t *hash_crear(size_t capacidad_inicial)
{
	hash_t *hash = calloc(1, sizeof(hash_t));
	if (!hash) {
		return NULL;
	}

	hash->capacidad = (capacidad_inicial < CAPACIDAD_MINIMA) ?
				  CAPACIDAD_MINIMA :
				  capacidad_inicial;
	hash->tabla = crear_tabla(hash->capacidad);

	if (!hash->tabla) {
		free(hash);
		return NULL;
	}

	return hash;
}

size_t hash_cantidad(hash_t *hash)
{
	if (!hash) {
		return 0;
	}

	return hash->cantidad;
}

//Devuelve true si el nuevo par se agregó correctamente en la tabla hash
bool agregar_par_en_tabla(char *clave, void *par, void *hash)
{
	par_t *par_origen = par;
	hash_t *nueva_tabla = hash;
	size_t indice = func_hash(clave, nueva_tabla->capacidad);
	return lista_agregar(nueva_tabla->tabla[indice], par_origen);
}

//Crea una nueva tabla con más capacidad y copia todos los elementos, si se pudo hacer correctamente devuelve true
//O tambien devuelve true si no era necesario agrandar la capacidad.
bool rehashear(hash_t *hash)
{
	if ((double)hash->cantidad / (double)hash->capacidad >=
	    FACTOR_CARGA_MAX) {
		size_t nueva_capacidad = hash->capacidad * FACTOR_CRECIMIENTO;

		lista_t **nueva_tabla = crear_tabla(nueva_capacidad);
		if (!nueva_tabla) {
			return false;
		}

		hash_t aux;
		aux.tabla = nueva_tabla;
		aux.capacidad = nueva_capacidad;

		size_t cantidad = 0;
		bool seguir = true;
		for (size_t i = 0; i < hash->capacidad && seguir; i++) {
			cantidad += iterador_bucket_rehash(hash->tabla[i],
							   agregar_par_en_tabla,
							   &aux, &seguir);
		}

		if (cantidad != hash->cantidad) {
			destruir_tabla(nueva_tabla, nueva_capacidad);
			return false;
		}

		destruir_tabla(hash->tabla, hash->capacidad);

		hash->tabla = nueva_tabla;
		hash->capacidad = nueva_capacidad;
	}

	return true;
}

//Devuelve true si habia un par con la misma clave y actualiza su valor, el valor antiguo es devuelto en encontrado.
bool hash_actualizar_existente(char *clave, void *valor, hash_t *hash,
			       void **encontrado, size_t indice)
{
	par_t *par_existente = encontrar_par(clave, hash->tabla[indice]);

	if (!par_existente) {
		return false;
	}

	if (encontrado) {
		(*encontrado) = par_existente->valor;
	}
	par_existente->valor = valor;
	return true;
}

//Elimina el ultimo par agregado de un determinado bucket.
void revertir_insercion(hash_t *hash, size_t indice)
{
	size_t posicion = lista_cantidad(hash->tabla[indice]);
	par_t *ultimo_par_agregado =
		lista_eliminar_elemento(hash->tabla[indice], posicion - 1);
	hash->cantidad--;
	destruir_par(ultimo_par_agregado);
}

bool hash_insertar(hash_t *hash, char *clave, void *valor, void **encontrado)
{
	if (!hash || !clave) {
		return false;
	}

	size_t indice = func_hash(clave, hash->capacidad);
	if (hash_actualizar_existente(clave, valor, hash, encontrado, indice)) {
		return true;
	}

	if (encontrado) {
		(*encontrado) = NULL;
	}

	par_t *nuevo_par = crear_par(clave, valor);
	if (!nuevo_par) {
		return false;
	}

	if (!lista_agregar(hash->tabla[indice], nuevo_par)) {
		destruir_par(nuevo_par);
		return false;
	}

	hash->cantidad++;

	if (!rehashear(hash)) {
		revertir_insercion(hash, indice);
		return false;
	}

	return true;
}

void *hash_buscar(hash_t *hash, char *clave)
{
	if (!hash || !clave || hash->cantidad == 0) {
		return NULL;
	}

	size_t indice = func_hash(clave, hash->capacidad);
	par_t *par = encontrar_par(clave, hash->tabla[indice]);
	if (!par) {
		return NULL;
	}

	return par->valor;
}

bool hash_contiene(hash_t *hash, char *clave)
{
	if (!hash || !clave || hash->cantidad == 0) {
		return false;
	}

	size_t indice = func_hash(clave, hash->capacidad);

	return lista_buscar_posicion(hash->tabla[indice], clave,
				     comparador_par_clave) != -1;
}

void *hash_quitar(hash_t *hash, char *clave)
{
	if (!hash || !clave || hash->cantidad == 0) {
		return NULL;
	}

	size_t indice = func_hash(clave, hash->capacidad);

	int posicion = lista_buscar_posicion(hash->tabla[indice], clave,
					     comparador_par_clave);
	if (posicion == -1) {
		return NULL;
	}

	par_t *par_eliminado =
		lista_eliminar_elemento(hash->tabla[indice], (size_t)posicion);
	void *valor = par_eliminado->valor;
	destruir_par(par_eliminado);
	hash->cantidad--;

	return valor;
}

size_t hash_iterar(hash_t *hash, bool (*f)(char *, void *, void *), void *ctx)
{
	if (!hash || !f || hash->cantidad == 0) {
		return 0;
	}

	bool seguir = true;
	size_t cantidad = 0;

	for (size_t i = 0; i < hash->capacidad && seguir; i++) {
		cantidad += iterador_bucket(hash->tabla[i], f, ctx, &seguir);
	}

	return cantidad;
}

void hash_destruir(hash_t *hash)
{
	hash_destruir_todo(hash, NULL);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash) {
		return;
	}

	for (size_t i = 0; i < hash->capacidad; i++) {
		if (destructor) {
			aux_destruir_contenido_t aux;
			aux.destructor = destructor;
			lista_con_cada_elemento(hash->tabla[i],
						destruir_par_valor, &aux);
		}
		lista_destruir_todo(hash->tabla[i], destruir_par);
	}

	free(hash->tabla);
	free(hash);
}