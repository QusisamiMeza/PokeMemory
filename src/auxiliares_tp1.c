#include "auxiliares_tp1.h"

char *leer_linea(FILE *archivo)
{
	if (!archivo) {
		return NULL;
	}

	size_t tamaño_linea = TAMAÑO_INICIAL_LINEA;
	size_t cant_caracteres = CANT_INICIAL_CARACETERES;
	char *linea = malloc(tamaño_linea);

	if (!linea) {
		return NULL;
	}

	int caracter;
	while ((caracter = fgetc(archivo)) != EOF && caracter != '\n') {
		if (cant_caracteres + FIN_LINEA >= tamaño_linea) {
			tamaño_linea *= FACTOR_CRECIMIENTO;
			char *nueva_linea = realloc(linea, tamaño_linea);
			if (!nueva_linea) {
				free(linea);
				return NULL;
			}
			linea = nueva_linea;
		}
		linea[cant_caracteres] = (char)caracter;
		cant_caracteres++;
	}

	if (caracter == EOF && cant_caracteres == SIN_CARACTERES) {
		free(linea);
		return NULL;
	}

	linea[cant_caracteres] = '\0';
	return linea;
}

char *copiar_string(char *string)
{
	if (!string) {
		return NULL;
	}
	char *copia = malloc(strlen(string) + 1);
	if (!copia) {
		return NULL;
	}
	strcpy(copia, string);
	return copia;
}

void agregar_pokemon(struct pokemon **pokemones, size_t *cant_pokemones,
		     struct pokemon pokemon_agregar)
{
	if (!pokemones || !cant_pokemones) {
		return;
	}
	size_t total_pok = (size_t)(*cant_pokemones) + 1;
	struct pokemon *temp =
		realloc(*pokemones, sizeof(struct pokemon) * total_pok);
	if (temp == NULL) {
		return;
	}
	*pokemones = temp;
	(*pokemones)[*cant_pokemones] = pokemon_agregar;
	(*pokemones)[*cant_pokemones].nombre =
		copiar_string(pokemon_agregar.nombre);
	(*cant_pokemones)++;
}

char *extraer_campo(int *index_linea, char *linea)
{
	if (!linea || !index_linea) {
		return NULL;
	}

	size_t tamaño_campo = TAMAÑO_INICIAL_LINEA;
	size_t cant_caracteres = CANT_INICIAL_CARACETERES;
	char *campo_limpio = malloc(tamaño_campo);

	if (!campo_limpio) {
		return NULL;
	}

	while (linea[*index_linea] != ',' && linea[*index_linea] != '\n' &&
	       linea[*index_linea] != '\0') {
		if (cant_caracteres + FIN_LINEA >= tamaño_campo) {
			tamaño_campo *= FACTOR_CRECIMIENTO;
			char *nuevo_campo = realloc(campo_limpio, tamaño_campo);
			if (!nuevo_campo) {
				free(campo_limpio);
				return NULL;
			}
			campo_limpio = nuevo_campo;
		}
		campo_limpio[cant_caracteres] = linea[*index_linea];
		(*index_linea)++;
		cant_caracteres++;
	}

	if (linea[*index_linea] == ',') {
		(*index_linea)++;
	}

	campo_limpio[cant_caracteres] = '\0';
	return campo_limpio;
}

void asignar_campo_int(int *campo_int, char *linea, int *i)
{
	char *campo_nuevo = extraer_campo(i, linea);
	if (campo_nuevo == NULL) {
		*campo_int = INVALIDO;
		return;
	}

	*campo_int = atoi(campo_nuevo);
	free(campo_nuevo);
}

int asignar_tipo(char *linea, int *i)
{
	char *campo_nuevo = extraer_campo(i, linea);
	int TIPO = TIPO_INVALIDO;

	if (campo_nuevo == NULL) {
		return TIPO;
	}

	if (strcmp(campo_nuevo, COD_ELEC) == IGUALES) {
		TIPO = TIPO_ELEC;
	} else if (strcmp(campo_nuevo, COD_FUEG) == IGUALES) {
		TIPO = TIPO_FUEG;
	} else if (strcmp(campo_nuevo, COD_PLAN) == IGUALES) {
		TIPO = TIPO_PLAN;
	} else if (strcmp(campo_nuevo, COD_AGUA) == IGUALES) {
		TIPO = TIPO_AGUA;
	} else if (strcmp(campo_nuevo, COD_NORM) == IGUALES) {
		TIPO = TIPO_NORM;
	} else if (strcmp(campo_nuevo, COD_FANT) == IGUALES) {
		TIPO = TIPO_FANT;
	} else if (strcmp(campo_nuevo, COD_LUCH) == IGUALES) {
		TIPO = TIPO_LUCH;
	}
	free(campo_nuevo);
	return TIPO;
}

bool verificar_columnas_extras(char *linea, int index_linea)
{
	if (linea[index_linea] == ',') {
		return !ES_ULTIMA_COL;
	}
	return ES_ULTIMA_COL;
}

bool verificar_campos(struct pokemon pokemon)
{
	if (pokemon.ataque > 0 && pokemon.defensa > 0 &&
	    pokemon.velocidad > 0 && pokemon.id > 0 && pokemon.nombre != NULL &&
	    pokemon.tipo != TIPO_INVALIDO) {
		return ES_POKEMON_VALIDO;
	}
	return !ES_POKEMON_VALIDO;
}

void asignar_pokemon(struct pokemon *pokemon_nuevo, char *linea, int *i)
{
	if (!pokemon_nuevo || !linea || !i) {
		return;
	}
	asignar_campo_int(&pokemon_nuevo->id, linea, i);
	pokemon_nuevo->nombre = extraer_campo(i, linea);
	pokemon_nuevo->tipo = asignar_tipo(linea, i);
	asignar_campo_int(&pokemon_nuevo->ataque, linea, i);
	asignar_campo_int(&pokemon_nuevo->defensa, linea, i);
	asignar_campo_int(&pokemon_nuevo->velocidad, linea, i);
}

bool es_menor_id(struct pokemon *pokemon_A, struct pokemon *pokemon_B)
{
	if (!pokemon_A || !pokemon_B) {
		return false;
	}
	return pokemon_A->id < pokemon_B->id;
}

struct pokemon **
asignar_pokemones_ordenados(struct pokemon *pokemones, size_t cantidad,
			    bool (*f)(struct pokemon *, struct pokemon *))
{
	if (!pokemones || cantidad == 0 || !f) {
		return NULL;
	}
	struct pokemon **pok_ordenados =
		malloc(sizeof(struct pokemon *) * cantidad);
	if (!pok_ordenados) {
		return NULL;
	}

	for (size_t i = 0; i < cantidad; i++) {
		pok_ordenados[i] = &pokemones[i];
	}

	for (size_t i = 0; i < cantidad - 1; i++) {
		size_t min_idx = i;
		for (size_t j = i + 1; j < cantidad; j++) {
			if (f(pok_ordenados[j], pok_ordenados[min_idx])) {
				min_idx = j;
			}
		}

		if (min_idx != i) {
			struct pokemon *temp = pok_ordenados[i];
			pok_ordenados[i] = pok_ordenados[min_idx];
			pok_ordenados[min_idx] = temp;
		}
	}

	return pok_ordenados;
}

struct pokemon *parsear_pokemon(char *linea)
{
	if (!linea || strcmp("", linea) == IGUALES) {
		return NULL;
	}

	struct pokemon *pokemon_nuevo = malloc(sizeof(struct pokemon));
	if (!pokemon_nuevo) {
		return NULL;
	}
	int i = 0;
	asignar_pokemon(pokemon_nuevo, linea, &i);
	if (!verificar_columnas_extras(linea, i) ||
	    !verificar_campos(*pokemon_nuevo)) {
		if (pokemon_nuevo->nombre != NULL) {
			free(pokemon_nuevo->nombre);
		}
		free(pokemon_nuevo);
		return NULL;
	}
	return pokemon_nuevo;
}

char *obtener_string_tipo(int tipo_pokemon)
{
	switch (tipo_pokemon) {
	case TIPO_ELEC:
		return COD_ELEC;
	case TIPO_FUEG:
		return COD_FUEG;
	case TIPO_PLAN:
		return COD_PLAN;
	case TIPO_AGUA:
		return COD_AGUA;
	case TIPO_NORM:
		return COD_NORM;
	case TIPO_FANT:
		return COD_FANT;
	case TIPO_PSI:
		return COD_PSI;
	case TIPO_LUCH:
		return COD_LUCH;
	default:
		return "TIPO_DESCONOCIDO";
	}
}

bool escribir_pokemones_archivo(struct pokemon *pokemon, void *extra)
{
	FILE *archivo = extra;
	if (!pokemon || !archivo) {
		return false;
	}
	int id = pokemon->id;
	char *nombre = pokemon->nombre;
	char *tipo = obtener_string_tipo(pokemon->tipo);
	int ataque = pokemon->ataque;
	int defensa = pokemon->defensa;
	int velocidad = pokemon->velocidad;
	fprintf(archivo, "%i,%s,%s,%i,%i,%i\n", id, nombre, tipo, ataque,
		defensa, velocidad);
	return true;
}

void destruir_pokemones(struct pokemon *pokemones, size_t cantidad_pokemones)
{
	if (!pokemones) {
		return;
	}
	for (int i = 0; i < cantidad_pokemones; i++) {
		free(pokemones[i].nombre);
	}
	free(pokemones);
}
