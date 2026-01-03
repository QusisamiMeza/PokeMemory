#ifndef MENU_H_
#define MENU_H_

#include <stdlib.h>
#include <stdbool.h>

typedef struct menu menu_t;

//Crea un menu con la capacidad determinada (mayor a 0 o 3, si es menor a 3)
//en caso de error, devuelve NULL
menu_t *crear_menu(const char *nombre, size_t capacidad);

//Devuelve true si se agrego una opcion con sus respectivos datos
//En caso de error, devuelve false
bool menu_agregar(menu_t *menu, const char *nombre, char *tecla,
		  void (*f)(void *, void *));

//Devuelve true si se ejecuto dicha opcion con la funcion pasada por parametro
//En caso de error, devuelve false.
bool menu_ejecutar_opcion(menu_t *menu, char *tecla, void *aux1, void *aux2);

//Elimina la opcion del menu asociada a la tecla
//Devuelve el dato guardado en la opcion, en caso de no existir devuelve NULL
bool menu_eliminar_opcion(menu_t *menu, char *tecla);

//Devuelve la cantidad de opciones, en caso de error devuelve 0
size_t menu_cantidad(menu_t *menu);

//Devuelve el nombre del menu, en caso de error NULL
char *menu_nombre(menu_t *menu);

//Aplica la funcion a cada opcion del menu
//La funcion recibe (Letra, nombre, aux)
size_t menu_iterar(menu_t *menu, bool (*f)(char *, char *, void *), void *aux);

//Libera la memoria reservada para menu
void destruir_menu(menu_t *menu);

#endif // MENU_H_