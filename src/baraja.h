#ifndef BARAJA_H_
#define BARAJA_H_
#include <stdlib.h>
#include <stdbool.h>

typedef struct baraja baraja_t;

enum estado {
	VACIA,
	BOCA_ABAJO,
	BOCA_ARRIBA,
};

enum baraja_recorrido {
	BARAJA_RECORRER_TODAS,
	BARAJA_RECORRER_BOCA_ABAJO,
	BARAJA_RECORRER_BOCA_ARRIBA
};

//Crea una baraja con una capacidad mayor a 0, en caso de error devuelve NULL.
baraja_t *crear_baraja(size_t capacidad);

//Devuelve la cantidad de elementos o 0 en caso de error.
size_t cantidad_baraja(baraja_t *baraja);

//Mezcla toda la baraja según la semilla dada.
bool baraja_mezclar(baraja_t *baraja, unsigned int semilla);

// Inserta el dato en la primera posición VACIA con estado BOCA_ABAJO.
// Devuelve true si pudo insertarlo, false si la baraja está llena o algo fallo.
bool baraja_insertar(baraja_t *baraja, void *dato);

//Elimina de la baraja el elemento de esa posicion y lo devuelve, en caso de error da NULL.
void *baraja_eliminar(baraja_t *baraja, size_t posicion);

//Devuelve el elemento de esa posicion si está marcado como BOCA_ARRIBA.
//En caso de estar BOCA_ABAJO, VACIA o no exista la posicion, entonces devuelve NULL.
void *baraja_obtener(baraja_t *baraja, size_t posicion);

//Devuelve el estado de la baraja en esa posicion
// si esta VACIA -> 0
// si esta BOCA_ABAJO -> 1
// si esta BOCA_ARRIBA -> 2
// si la posicion no existe o algun error -> -1
int baraja_estado_de_elemento(baraja_t *baraja, size_t posicion);

//Busca un elemento en la baraja y devuelve la posicion del mismo
// Si no existe el elemento devuelve -1
int baraja_posicion(baraja_t *baraja, void *elemento,
		    int (*comparador)(void *, void *));

//Marca el elemento en esa posicion como BOCA_ARRIBA si es que está BOCA_ABAJO.
//Si lo cambia, se retorna true. Otro caso, retorna false.
bool baraja_boca_arriba_elemento(baraja_t *baraja, size_t posicion);

//Marca el elemento en esa posicion como BOCA_ABAJO si es que está BOCA_ARRIBA.
//Devuelve true en caso de exito, false en caso contrario.
bool baraja_boca_abajo_elemento(baraja_t *baraja, size_t posicion);

//Recorre los elementos de la baraja segun el tipo de recorrido que se desee
// y les aplica la funcion hasta que de false o recorra todos los elementos
//Devuelve la cantidad de elementos recorridos.
size_t baraja_iterador(baraja_t *baraja, enum baraja_recorrido recorrido,
		       bool (*f)(void *, void *), void *extra);

//Libera la memoria reservada para la baraja
void destruir_baraja(baraja_t *baraja);

//Libera la memoria reservada para la baraja y aplica un destructor en cada elemento
void destruir_baraja_todo(baraja_t *baraja, void (*destructor)(void *));

#endif // BARAJA_H_