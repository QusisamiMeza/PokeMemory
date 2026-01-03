#ifndef AUXILIARES_H_
#define AUXILIARES_H_
#include <string.h>
#include <stdio.h>
#include "tp1.h"
#define ERROR 1
//ARREGLAR CONSTANTES - TIENEN QUE SER MUY GENERALES Y REUTILIZABLES

#define COD_ELEC "ELEC"
#define COD_FUEG "FUEG"
#define COD_PLAN "PLAN"
#define COD_AGUA "AGUA"
#define COD_NORM "NORM"
#define COD_FANT "FANT"
#define COD_PSI "PSI"
#define COD_LUCH "LUCH"

#define FIN_LINEA 1
#define FACTOR_CRECIMIENTO 2
#define TIPO_INVALIDO -1
#define INVALIDO -1
#define IGUALES 0
#define SIN_CARACTERES 0
#define TAMAÑO_INICIAL_LINEA 100
#define CANT_INICIAL_CARACETERES 0
#define ES_ULTIMA_COL true
#define ES_POKEMON_VALIDO true

/**
 * PRE:
 * -
 * POST:
 * Lee una línea entera de un archivo abierto. Devuelve la línea leída o NULL en
 * caso de error.
 */
char *leer_linea(FILE *archivo);

/**
 * PRE:
 * - Para un parseo exitoso ` linea  ` debe tener el formato de 'id,nombre,cod_tipo,ataque,defensa,velocidad'
 * POST:
 * - Se reserva memoria para un pokemon
 * - Se extrae de la linea la informacion del pokemon y lo asigna en cada campo de la estructura reservada.
 */
struct pokemon *parsear_pokemon(char *linea);

/**
 * PRE:
 * ' pokemones ' y 'pokemon_agregar ' deben ser validos.
 * POST:
 * Aumenta el tamaño de ' pokemones '
 * Copia la informacion de ' pokemon_agregar ' en el recien añadido espacio
 * Aumenta en uno ' cant_pokemones '.
 */
void agregar_pokemon(struct pokemon **pokemones, size_t *cant_pokemones,
		     struct pokemon pokemon_agregar);

/**
 * PRE:
 *  - 'pokemones' debe ser un arreglo válido de al menos 'cantidad' elementos.
 * POST:
 * - Reserva memoria para un arreglo dinamico de punteros a 'struct pokemon' con los mismos elementos de 'pokemones',
 *    ordenados según la función de comparación 'f'.
 *  - No modifica el arreglo original 'pokemones'.
 */
struct pokemon **
asignar_pokemones_ordenados(struct pokemon *pokemones, size_t cantidad,
			    bool (*f)(struct pokemon *, struct pokemon *));

/**
 * PRE:
 * 
 * POST:
 * - Devuelve 'true' si el id de 'pokemon_A' es menor que el de 'pokemon_B'.
 * - Devuelve 'false' si el id de 'pokemon_A' no es menor, o si alguno de los punteros es NULL.
 */
bool es_menor_id(struct pokemon *pokemon_A, struct pokemon *pokemon_B);

/**
 * PRE:
 * -
 * POST:
 * Devuelve la string correspondiente segun el ' tipo_pokemon '.
 */
char *obtener_string_tipo(int tipo_pokemon);

/**
 * PRE:
 * 
 * POST:
 * Dado un string `linea` en formato CSV, crea un pokemon con los
 * datos del mismo. Devuelve el pokemon creado o NULL en caso de error.
 */
struct pokemon *parsear_pokemon(char *linea);

/**
 * PRE:
 * ' pokemones ' debe ser valido.
 * ' extra ' es un puntero a un archivo abierto previamente.
 * POST:
 * Escribe en el archivo la informacion pasado por `void *extra` de cada pokemon en formato:
 *  ' ID,nombre,tipo,ataque,defensa,velocidad '.
 */
bool escribir_pokemones_archivo(struct pokemon *pokemon, void *extra);

/**
 * PRE:
 * ' pokemones ' debe ser valido
 * POST:
 * Libera la memoria de los nombres de cada pokemon y el arreglo de pokemones.
 */
void destruir_pokemones(struct pokemon *pokemones, size_t cantidad_pokemones);

#endif // AUXILIARES_H_