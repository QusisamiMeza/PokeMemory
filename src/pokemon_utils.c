#include "pokemon_utils.h"
#include <ctype.h>
#include <stdio.h>
#include "auxiliares_tp1.h"

char *convertir_string_a_minusculas(char *string)
{
	if (!string) {
		return NULL;
	}
	for (int i = 0; string[i] != '\0'; i++) {
		string[i] = (char)tolower((unsigned char)string[i]);
	}
	return string;
}

void imprimir_pokemon(struct pokemon *pokemon)
{
	if (!pokemon) {
		return;
	}

	printf(FORMATO_POKEMON, pokemon->id, pokemon->nombre,
	       obtener_string_tipo(pokemon->tipo), pokemon->ataque,
	       pokemon->defensa, pokemon->velocidad);
}

int es_nombre_igual(const void *a, const void *b)
{
	if (!a || !b) {
		return 1;
	}
	const struct pokemon *pokemon = (const struct pokemon *)a;
	const char *nombre_pok = b;
	return strcmp(pokemon->nombre, nombre_pok) == 0 ? 0 : 1;
}

int es_id_igual(const void *a, const void *b)
{
	if (!a || !b) {
		return 1;
	}
	const struct pokemon *pokemon = (const struct pokemon *)a;
	const int *id_pok = b;
	return pokemon->id == *id_pok ? 0 : 1;
}
