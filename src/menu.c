#include "menu.h"
#include "hash.h"
#include <string.h>
#include <stdio.h>

struct menu {
	char *nombre;
	size_t cantidad;
	hash_t *opciones;
};

typedef struct opciones_menu {
	char *nombre;
	char *tecla;
	void (*f)(void *, void *);
} opciones_menu_t;

static char *copiar_nombre(const char *nom)
{
	char *copia = malloc(strlen(nom) + 1);
	if (!copia)
		return NULL;

	return strcpy(copia, nom);
}

void destruir_opcion(void *opcion)
{
	if (!opcion)
		return;

	opciones_menu_t *op = opcion;
	free(op->nombre);
	free(op->tecla);
	free(op);
}

typedef struct aux_iterar {
	bool (*f)(char *, char *, void *);
	void *aux;
} aux_iterar_t;

menu_t *crear_menu(const char *nombre, size_t capacidad)
{
	if (capacidad == 0 || !nombre)
		return NULL;

	menu_t *menu = malloc(sizeof(menu_t));
	if (!menu)
		return NULL;

	menu->opciones = hash_crear(capacidad);
	if (!menu->opciones) {
		free(menu);
		return NULL;
	}

	menu->nombre = copiar_nombre(nombre);
	if (!menu->nombre) {
		hash_destruir(menu->opciones);
		free(menu);
		return NULL;
	}

	menu->cantidad = 0;
	return menu;
}

bool menu_agregar(menu_t *menu, const char *nombre, char *tecla,
		  void (*f)(void *, void *))
{
	if (!menu || !nombre || !tecla || strlen(tecla) != 1 || !f)
		return false;

	opciones_menu_t *opcion = malloc(sizeof(opciones_menu_t));
	if (!opcion)
		return false;

	opcion->nombre = copiar_nombre(nombre);
	opcion->tecla = copiar_nombre(tecla);
	if (!opcion->nombre || !opcion->tecla) {
		destruir_opcion(opcion);
		return false;
	}

	opcion->f = f;

	if (!hash_insertar(menu->opciones, tecla, opcion, NULL)) {
		destruir_opcion(opcion);
		return false;
	}

	menu->cantidad++;
	return true;
}

bool menu_ejecutar_opcion(menu_t *menu, char *tecla, void *aux1, void *aux2)
{
	if (!menu || !tecla || strlen(tecla) != 1)
		return false;

	opciones_menu_t *opcion = hash_buscar(menu->opciones, tecla);
	if (!opcion)
		return false;

	opcion->f(aux1, aux2);
	return true;
}

bool menu_eliminar_opcion(menu_t *menu, char *tecla)
{
	if (!menu || !tecla || strlen(tecla) != 1)
		return NULL;

	opciones_menu_t *opcion = hash_quitar(menu->opciones, tecla);
	if (!opcion)
		return NULL;

	destruir_opcion(opcion);
	menu->cantidad--;
	return true;
}

size_t menu_cantidad(menu_t *menu)
{
	if (!menu)
		return 0;

	return menu->cantidad;
}

char *menu_nombre(menu_t *menu)
{
	if (!menu)
		return NULL;

	return menu->nombre;
}

bool aplicar_funcion_opcion(char *clave, void *op, void *aux)
{
	opciones_menu_t *opcion = op;
	aux_iterar_t *aux_f = aux;

	return aux_f->f(opcion->tecla, opcion->nombre, aux_f->aux);
}

size_t menu_iterar(menu_t *menu, bool (*f)(char *, char *, void *), void *aux)
{
	if (!menu || !f)
		return 0;
	aux_iterar_t aux_f;
	aux_f.f = f;
	aux_f.aux = aux;

	return hash_iterar(menu->opciones, aplicar_funcion_opcion, &aux_f);
}

void destruir_menu(menu_t *menu)
{
	if (!menu)
		return;

	hash_destruir_todo(menu->opciones, destruir_opcion);
	free(menu->nombre);
	free(menu);
}
