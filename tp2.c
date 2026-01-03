#include "src/ansi.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include "src/juego.h"
#include "src/menu.h"
#include "src/tp1.h"
#include "src/auxiliares_tp1.h"
#include "src/pokemon_utils.h"
#include <unistd.h>
#include "src/define_programa.h"

const int IDX_ARCH = 1;
const int CANT_OP_MAIN = 7;
const int CANT_OP_SUBMENUS = 3;

enum estilo {
	DEFAULT,
	ESTILO1,
	ESTILO2,
};
typedef struct programa {
	menu_t *menu_principal;
	enum estilo estilo;
	tp1_t *pokedex;
} programa_t;
typedef struct aux_mostrar_nombre {
	struct pokemon **pokemones_ordenados_nombre;
	size_t cantidad_pok;
} aux_mostrar_nombre_t;

// Ejecuta la operación de cargar archivo: muestra el estado actual de la Pokédex,
//solicita al usuario un archivo válido y lo carga, o permite regresar al menú principal con 'Q'
void cargar_archivo_op(void *program, void *aux)
{
	printf(ANSI_RESET_SCREEN);
	printf(TITULO_CARGAR_ARCH);
	programa_t *programa = program;
	bool salir = false;

	while (!salir) {
		if (programa->pokedex != NULL)
			printf("Recuerda, tienes pokemones guardados, si vas a subir otro archivo esos pokemones se perderan!.\n");

		else
			printf("No tienes ningun pokemon guardado, ingresa tu archivo correctamente :)\n");

		printf("Para regresar presiona 'Q'\n");
		char *linea = leer_linea(stdin);
		if (strcmp(linea, "Q") == 0) {
			salir = true;
		} else {
			tp1_t *nuevo = tp1_leer_archivo(linea);
			if (nuevo != NULL) {
				if (programa->pokedex != NULL)
					tp1_destruir(programa->pokedex);
				programa->pokedex = nuevo;
			} else {
				printf("Hubo un error al querer leer el archivo, ingrese bien la direccion del archivo.\n");
			}
		}

		free(linea);
	}
}

// Ejecuta la salida del programa liberando la memoria de reservada del programa
// y muestra un mensaje de despedida
void salir_programa(void *program, void *aux)
{
	printf(ANSI_RESET_SCREEN);
	programa_t *programa = program;
	destruir_menu(programa->menu_principal);
	tp1_destruir(programa->pokedex);
	printf(ANSI_COLOR_YELLOW ANSI_COLOR_BOLD PIKACHI_BIG_ASSCII
		       ANSI_COLOR_RESET);
	printf(ANSI_COLOR_BOLD ANSI_COLOR_MAGENTA
	       "\t\t\tHasta la proxima!\n" ANSI_COLOR_RESET);
}

//imprime por pantalla las opciones que tiene un menu, retorna true si las imprimio
bool imprimir_opciones_menu(char *letra, char *nombre, void *aux)
{
	if (!nombre || !letra || !aux)
		return false;
	int *estilo = aux;
	printf("\n");
	switch (*estilo) {
	case DEFAULT:
		printf("\t\t\t\t____________________________\n");
		printf("\t\t\t\t        %s\n", nombre);
		printf(ANSI_COLOR_RED ANSI_COLOR_BOLD
		       "\t\t\t\t          Press %s\n" ANSI_BG_RESET,
		       letra);
		printf("\t\t\t\t_____________________________\n");
		break;
	case ESTILO1:
		printf("\t\t\t[ " ANSI_COLOR_GREEN ANSI_COLOR_BOLD
		       "%s" ANSI_COLOR_RESET " . . . . . . . . Press %s ]\n",
		       nombre, letra);
		break;
	case ESTILO2:
		printf("\t\t\t" ANSI_COLOR_MAGENTA
		       "╔══════════════════════════╗\n");
		printf("\t\t\t║  " ANSI_COLOR_RESET " %s\n", nombre);
		printf("\t\t\t" ANSI_COLOR_MAGENTA
		       "║  " ANSI_COLOR_CYAN ANSI_COLOR_BOLD
		       " [Press %s]\n" ANSI_COLOR_RESET,
		       letra);
		printf("\t\t\t" ANSI_COLOR_MAGENTA
		       "╚══════════════════════════╝\n" ANSI_COLOR_RESET);
		break;
	}
	return true;
}

//imprime toda la informacion relacionada al menu segun el estilo escogido
void imprimir_menu(menu_t *menu, bool pokedex_cargada, int estilo)
{
	if (!menu)
		return;
	char *titulo = menu_nombre(menu);
	if (!titulo)
		return;
	printf(ANSI_RESET_SCREEN);
	printf("%s", titulo);
	if (pokedex_cargada)
		printf(ANSI_COLOR_RED ANSI_COLOR_BOLD
		       "\t\t\t¡Pokedex cargada! Puede seguir en paz:)\n" ANSI_COLOR_RESET);
	else
		printf(ANSI_COLOR_RED ANSI_COLOR_BOLD
		       "\t\t\t¡Carga tu pokedex antes o no se podrá buscar, mostrar ni jugar!\n" ANSI_COLOR_RESET);
	menu_iterar(menu, imprimir_opciones_menu, &estilo);
}

//Pide al usuario ingresar 'R' para salir del bucle
void bucle_volver()
{
	bool salir = false;
	while (!salir) {
		printf("\nPara retornar al menu anterior ingrese 'R'\n");
		char *linea = leer_linea(stdin);
		if (strcmp(linea, "R") == 0)
			salir = true;
		free(linea);
	}
}

//Pide al usuario que ingrese un numero y busca el pokemon correspodiente,
//retorna cuando se ingresa el numero y pone 'R'
void buscar_por_id(void *pokedex, void *aux)
{
	if (!pokedex)
		return;

	printf(ANSI_RESET_SCREEN);
	printf(TITULO_ENCONTRAR_ID);
	printf("\nIngrese el ID del pokemon:\n");
	tp1_t *tp1 = pokedex;
	char *dato = leer_linea(stdin);
	int id = atoi(dato);
	struct pokemon *pok = tp1_buscar_id(tp1, id);
	printf(ANSI_RESET_SCREEN);
	if (pok != NULL)
		imprimir_pokemon(pok);
	else
		printf("\nNo se encontro el pokemon, asegurese de buscarlo con el mismo id.\n");
	free(dato);
	bucle_volver();
}

//Pide al usuario que ingrese un nombre y busca el pokemon correspodiente,
//retorna cuando se ingresa el nombre y pone 'R'
void buscar_por_nombre(void *pokedex, void *aux)
{
	if (!pokedex)
		return;

	printf(ANSI_RESET_SCREEN);
	printf(TITULO_ENCONTRAR_NOMBRE);
	printf("Ingrese el nombre del pokemon:\n");
	tp1_t *tp1 = pokedex;
	char *dato = leer_linea(stdin);
	struct pokemon *pok = tp1_buscar_nombre(tp1, dato);
	printf(ANSI_RESET_SCREEN);
	if (pok != NULL) {
		imprimir_pokemon(pok);
	} else {
		printf("\nNo se encontro el pokemon, asegurese de buscarlo con el mismo nombre. (Cuidado con las mayusculas)\n");
	}
	free(dato);
	bucle_volver();
}

//Imprime por pantalla que se está volviendo al menu
void volver_menu(void *aux1, void *aux2)
{
	printf("\nvolviendo al menu. . .\n");
	sleep(3);
}

//Espera que el usuario ingrese una accion para ejecutarla del menu buscar
bool procesar_entrada_busqueda(menu_t *menu, void *tp1, void *aux)
{
	char *linea = leer_linea(stdin);
	bool salir = false;

	if (strcmp(linea, "N") == 0) {
		salir = !menu_ejecutar_opcion(menu, linea, tp1, NULL);
	} else if (strcmp(linea, "I") == 0) {
		salir = !menu_ejecutar_opcion(menu, linea, tp1, NULL);
	} else if (strcmp(linea, "A") == 0) {
		salir = menu_ejecutar_opcion(menu, linea, NULL, NULL);
	}

	free(linea);
	return salir;
}

//Crea un menu buscar, agrega opciones, las ejecuta segun el usuario y lo destruye cuando el usuario
//salga del menu.
void buscar_pokemon(void *program, void *aux)
{
	menu_t *menu_busqueda = crear_menu(TITULO_BUSCAR, CANT_OP_SUBMENUS);
	if (!menu_busqueda) {
		printf("Hubo un error en crear el menu de busqueda\n");
		return;
	}

	programa_t *programa = program;
	if (!menu_agregar(menu_busqueda, "BUSCAR POR ID", "I", buscar_por_id) ||
	    !menu_agregar(menu_busqueda, "BUSCAR POR NOMBRE", "N",
			  buscar_por_nombre) ||
	    !menu_agregar(menu_busqueda, "VOLVER", "A", volver_menu)) {
		destruir_menu(menu_busqueda);
		printf("Hubo un error al iniciar el menu de busqueda.\n");
		return;
	}
	bool salir = false;
	do {
		imprimir_menu(menu_busqueda, programa->pokedex != NULL,
			      programa->estilo);
		salir = procesar_entrada_busqueda(menu_busqueda,
						  programa->pokedex, NULL);
	} while (!salir);

	destruir_menu(menu_busqueda);
}

//retorna true si se imprimio el pokemon correspondiente
bool mostrar_pok(struct pokemon *pokemon, void *extra)
{
	if (!pokemon)
		return false;

	printf(FORMATO_POKEMON, pokemon->id, pokemon->nombre,
	       obtener_string_tipo(pokemon->tipo), pokemon->ataque,
	       pokemon->defensa, pokemon->velocidad);
	return true;
}

//Itera por cada podemon de tp1 segun su id y lo imprime por pantalla
void mostrar_por_id(void *tp1_, void *aux)
{
	if (!tp1_)
		return;

	tp1_t *tp1 = tp1_;
	printf(ANSI_RESET_SCREEN);
	tp1_con_cada_pokemon(tp1, mostrar_pok, NULL);
	bucle_volver();
}

//Reserva memoria para un array de punteros a pokemones, los guarda en un auxiliar y lo retorna
aux_mostrar_nombre_t crear_aux(size_t cantidad_pokemones)
{
	aux_mostrar_nombre_t aux;
	aux.cantidad_pok = 0;
	aux.pokemones_ordenados_nombre =
		calloc(1, sizeof(struct pokemon *) * cantidad_pokemones);
	return aux;
}

//Copia el pokemon en la estructura auxiliar
bool copiar_pokemon(struct pokemon *p, void *extra)
{
	aux_mostrar_nombre_t *aux = extra;
	aux->pokemones_ordenados_nombre[aux->cantidad_pok++] = p;
	return true;
}

//Ordena un array de punteros a pokemones segun su nombre
void ordenar_pokemon_por_nombre(struct pokemon **arr, size_t cantidad)
{
	if (!arr || cantidad == 0)
		return;

	for (size_t i = 0; i < cantidad - 1; i++) {
		size_t menor = i;
		for (size_t j = i + 1; j < cantidad; j++) {
			if (strcmp(arr[j]->nombre, arr[menor]->nombre) < 0) {
				menor = j;
			}
		}
		struct pokemon *tmp = arr[i];
		arr[i] = arr[menor];
		arr[menor] = tmp;
	}
}

//Imprime por pantalla todos los pokemones disponibles de forma ordenada por nombre
void mostrar_por_nombre(void *tp1_, void *aux)
{
	if (!tp1_)
		return;

	tp1_t *tp1 = tp1_;
	size_t cantidad = tp1_cantidad(tp1);
	aux_mostrar_nombre_t aux_pokemones = crear_aux(cantidad);
	tp1_con_cada_pokemon(tp1, copiar_pokemon, &aux_pokemones);
	ordenar_pokemon_por_nombre(aux_pokemones.pokemones_ordenados_nombre,
				   aux_pokemones.cantidad_pok);
	printf(ANSI_RESET_SCREEN);
	for (size_t j = 0; j < aux_pokemones.cantidad_pok; j++) {
		imprimir_pokemon(aux_pokemones.pokemones_ordenados_nombre[j]);
	}
	free(aux_pokemones.pokemones_ordenados_nombre);
	bucle_volver();
}

//Ejecuta las opciones de un menu mostrar y retorna true si el usuario quiere volver
bool procesar_entrada_mostrar(menu_t *menu, void *tp1, void *aux)
{
	char *linea = leer_linea(stdin);
	bool salir = false;

	if (strcmp(linea, "N") == 0) {
		salir = !menu_ejecutar_opcion(menu, linea, tp1, linea);
	} else if (strcmp(linea, "I") == 0) {
		salir = !menu_ejecutar_opcion(menu, linea, tp1, linea);
	} else if (strcmp(linea, "A") == 0) {
		salir = menu_ejecutar_opcion(menu, linea, NULL, NULL);
	}

	free(linea);
	return salir;
}

//Crea un menu mostrar, agrega opciones, las ejecuta segun el usuario y lo destruye cuando el usuario
//salga del menu.
void mostrar_pokemon(void *program, void *aux)
{
	printf(ANSI_RESET_SCREEN);
	menu_t *menu_mostrar = crear_menu(TITULO_MOSTRAR, CANT_OP_SUBMENUS);
	if (!menu_mostrar) {
		printf("Hubo un error en crear el menu de busqueda\n");
		return;
	}

	programa_t *programa = program;
	if (!menu_agregar(menu_mostrar, "BUSCAR POR ID", "I", mostrar_por_id) ||
	    !menu_agregar(menu_mostrar, "BUSCAR POR NOMBRE", "N",
			  mostrar_por_nombre) ||
	    !menu_agregar(menu_mostrar, "VOLVER", "A", volver_menu)) {
		destruir_menu(menu_mostrar);
		printf("Hubo un error al iniciar el menu de busqueda.\n");
		return;
	}

	bool salir = false;
	do {
		imprimir_menu(menu_mostrar, programa->pokedex != NULL,
			      programa->estilo);
		salir = procesar_entrada_mostrar(menu_mostrar,
						 programa->pokedex, NULL);
	} while (!salir);

	destruir_menu(menu_mostrar);
}

//Ejecuta las opciones del menu principal, retorna true si el usuario quiere salir del programa
bool procesar_entrada_principal(programa_t *programa)
{
	char *linea = leer_linea(stdin);
	bool salir = false;
	bool es_con_semilla = false;
	if (strcmp(linea, "C") == 0) {
		salir = !menu_ejecutar_opcion(programa->menu_principal, linea,
					      programa, NULL);
	} else if (strcmp(linea, "Q") == 0) {
		salir = menu_ejecutar_opcion(programa->menu_principal, linea,
					     programa, NULL);
	} else if (strcmp(linea, "B") == 0) {
		salir = !menu_ejecutar_opcion(programa->menu_principal, linea,
					      programa, NULL);
	} else if (strcmp(linea, "M") == 0) {
		salir = !menu_ejecutar_opcion(programa->menu_principal, linea,
					      programa, NULL);
	} else if (strcmp(linea, "J") == 0) {
		salir = !menu_ejecutar_opcion(programa->menu_principal, linea,
					      programa->pokedex,
					      &es_con_semilla);
	} else if (strcmp(linea, "S") == 0) {
		es_con_semilla = true;
		salir = !menu_ejecutar_opcion(programa->menu_principal, linea,
					      programa->pokedex,
					      &es_con_semilla);
	} else if (strcmp(linea, "E") == 0) {
		salir = !menu_ejecutar_opcion(programa->menu_principal, linea,
					      programa, NULL);
	}

	free(linea);
	return salir;
}

//Muestra el juego y procesa la jugada ingresada por el usuario
void gestionar_turno(juego_t *juego)
{
	printf(ANSI_RESET_SCREEN);
	printf("%s", POKEMEMORY);
	mostrar_juego(juego);
	char *linea = leer_linea(stdin);
	if (!linea)
		return;
	int numero = atoi(linea);
	free(linea);

	enum resultado_jugada res = realizar_jugada(juego, numero);
	if (res == SEGUNDA_CARTA_LISTA) {
		printf(ANSI_RESET_SCREEN);
		printf("%s", POKEMEMORY);
		mostrar_juego(juego);
		printf("Espera! memoriza las cartas.\n");
		sleep(3);
		procesar_jugada(juego);
	} else if (res == JUGADA_INVALIDA) {
		printf("Ingresa un numero de carta valido. \n");
	}
}

//Inicia el juego, lo ejecuta, lo procesa hasta que termine y lo destruye.
void jugar_cartas(void *tp1, void *aux)
{
	if (!tp1)
		return;

	tp1_t *pokedex = tp1;
	bool *es_con_semilla = aux;
	printf(ANSI_RESET_SCREEN);
	printf("%s", POKEMEMORY);
	juego_t *juego;
	if (*es_con_semilla) {
		printf("Primero ingrese un numero para usarlo como semilla: \n");
		char *linea = leer_linea(stdin);
		if (!linea)
			return;
		juego = iniciar_juego(pokedex, (unsigned int)atoi(linea));
		free(linea);
	} else {
		juego = iniciar_juego(pokedex, (unsigned int)time(NULL));
	}

	if (!juego) {
		printf("Hubo un fallo en iniciar el juego\n");
		return;
	}

	do {
		gestionar_turno(juego);
	} while (estado_juego(juego) == JUGANDO);
	printf(ANSI_RESET_SCREEN);
	mostrar_ganador(juego);
	destruir_juego(juego);
	bucle_volver();
}

//Cambia los estilos del menu de manera circular
void cambiar_estilo_menu(void *program, void *aux)
{
	programa_t *programa = program;
	if (programa->estilo == ESTILO2)
		programa->estilo = DEFAULT;
	else
		programa->estilo++;
}

int main(int argc, char *argv[])
{
	if (argc > 2) {
		printf("Puedes ingresar solo el nombre del archivo con este formato: %s mi_archivo_pokemon.csv\n",
		       argv[0]);
		return -1;
	}
	programa_t programa = { 0 };
	if (argc == 2)
		programa.pokedex = tp1_leer_archivo(argv[IDX_ARCH]);
	else
		programa.pokedex = NULL;

	programa.menu_principal = crear_menu(POKEMEMORY, CANT_OP_MAIN);
	if (!programa.menu_principal) {
		printf("Ups, hubo un fallo al crear el menu principal.\n");
		return -1;
	}

	if (!menu_agregar(programa.menu_principal, "CARGAR ARCHIVO", "C",
			  cargar_archivo_op) ||
	    !menu_agregar(programa.menu_principal, "BUSCAR POKEMON", "B",
			  buscar_pokemon) ||
	    !menu_agregar(programa.menu_principal, "SALIR", "Q",
			  salir_programa) ||
	    !menu_agregar(programa.menu_principal, "MOSTRAR POKEMONES", "M",
			  mostrar_pokemon) ||
	    !menu_agregar(programa.menu_principal, "JUGAR", "J",
			  jugar_cartas) ||
	    !menu_agregar(programa.menu_principal, "JUGAR CON SEMILLA", "S",
			  jugar_cartas) ||
	    !menu_agregar(programa.menu_principal, "CAMBIAR ESTILO", "E",
			  cambiar_estilo_menu)) {
		destruir_menu(programa.menu_principal);
		tp1_destruir(programa.pokedex);
		printf("Ups, hubo un fallo al iniciar el menu principal.\n");
		return -1;
	}

	bool salir = false;
	do {
		imprimir_menu(programa.menu_principal, programa.pokedex != NULL,
			      programa.estilo);
		salir = procesar_entrada_principal(&programa);
	} while (!salir);

	return 0;
}
