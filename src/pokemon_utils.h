#ifndef POKEMON_UTILS_H
#define POKEMON_UTILS_H
#define FORMATO_POKEMON          \
	"===== Pokémon =====\n" \
	"ID      : %d\n"         \
	"Nombre  : %s\n"         \
	"Tipo    : %s\n"         \
	"Ataque  : %d\n"         \
	"Defensa : %d\n"         \
	"Velocidad: %d\n"        \
	"==================\n"

#include "tp1.h"
#include <stdbool.h>

/**
 * POST:
 * Convierte todos los caracteres de 'string' a minúsculas.
 */
char *convertir_string_a_minusculas(char *string);

/**
 * PRE:
 * `pokemon` debe ser valida
 * POST:
 * Imprime el pokemon por pantalla
 */
void imprimir_pokemon(struct pokemon *pokemon);

/**
 * PRE:
 * `pokemon` debe ser valida
 * POST:
 * Retorna 0 si `id` es igual al campo id `pokemon`, -1 en caso contrario
 */
int es_id_igual(const void *a, const void *b);

/**
 * PRE:
 * `pokemon` debe ser valida
 * POST:
 * Retorna 0 si `nombre` es igual al campo nombre de `pokemon`, -1 en caso contrario
 */
int es_nombre_igual(const void *a, const void *b);

#endif