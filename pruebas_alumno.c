#include "pa2m.h"
#include "src/baraja.h"
#include "src/menu.h"
#include <string.h>

const int MAX_VEC = 5;

//--------TDA BARAJA---------

int comparador_numeros(void *num_a, void *num_b)
{
	int *a = num_a;
	int *b = num_b;

	if (*a == *b) {
		return 0;
	}
	return -1;
}

baraja_t *crear_baraja_completa(size_t capacidad)
{
	baraja_t *baraja = crear_baraja(capacidad);
	if (!baraja)
		return NULL;

	for (size_t i = 0; i < capacidad; i++) {
		int *numero = malloc(sizeof(int));
		if (!numero) {
			destruir_baraja_todo(baraja, free);
			return NULL;
		}

		*numero = (int)i;
		if (!baraja_insertar(baraja, numero)) {
			free(numero);
			destruir_baraja_todo(baraja, free);
			return NULL;
		}
	}

	return baraja;
}

baraja_t *crear_baraja_6_elemetos_intercalados()
{
	baraja_t *baraja = crear_baraja(6);
	if (!baraja)
		return NULL;

	for (size_t i = 0; i < 6; i++) {
		int *numero = malloc(sizeof(int));
		if (!numero) {
			destruir_baraja_todo(baraja, free);
			return NULL;
		}

		*numero = (int)i;
		if (!baraja_insertar(baraja, numero)) {
			free(numero);
			destruir_baraja_todo(baraja, free);
			return NULL;
		}
	}

	baraja_boca_arriba_elemento(baraja, 1);
	baraja_boca_arriba_elemento(baraja, 4);
	void *eliminado1 = baraja_eliminar(baraja, 2);
	void *eliminado2 = baraja_eliminar(baraja, 5);
	free(eliminado1);
	free(eliminado2);

	return baraja;
}

void se_crea_baraja_exitosamente_con_capacidad_valida()
{
	size_t capacidad = 5;
	baraja_t *baraja = crear_baraja(capacidad);
	pa2m_afirmar(baraja != NULL,
		     "Se creo exitosamente una baraja con capacidad valida.");
	pa2m_afirmar(cantidad_baraja(baraja) == 0, "Su cantidad es de cero.");
	destruir_baraja(baraja);
}

void no_se_crea_baraja_dada_capacidad_invalida()
{
	size_t capacidad = 0;
	baraja_t *baraja = crear_baraja(capacidad);
	pa2m_afirmar(!baraja, "No se creo una baraja dada capacidad invalida.");
}

void baraja_recien_creada_se_agrega_un_elemento()
{
	baraja_t *baraja = crear_baraja(5);
	int dato_1 = 1;
	size_t cantidad_inicial = cantidad_baraja(baraja);
	pa2m_afirmar(baraja != NULL, "Se creo exitosamente una baraja.");
	pa2m_afirmar(baraja_insertar(baraja, &dato_1), "Se agrego elemento.");
	pa2m_afirmar(cantidad_baraja(baraja) == cantidad_inicial + 1,
		     "Su cantidad subio en uno.");
	pa2m_afirmar(baraja_estado_de_elemento(baraja, 0) == 1,
		     "Su estado es BOCA_ABAJO");
	pa2m_afirmar(baraja_boca_arriba_elemento(baraja, 0),
		     "Se descubre el elemento.");
	pa2m_afirmar(baraja_obtener(baraja, 0) == &dato_1,
		     "Se obtiene el elemento y es el recien agregado.");

	destruir_baraja(baraja);
}

void se_inserta_elementos_hasta_el_maximo()
{
	size_t capacidad = 5;
	baraja_t *baraja = crear_baraja(capacidad);
	int numeros[capacidad];
	int no_insertado = 99999;

	pa2m_afirmar(baraja != NULL, "Se creo exitosamente una baraja.");

	for (size_t i = 0; i < capacidad; i++) {
		numeros[i] = (int)i;
		pa2m_afirmar(baraja_insertar(baraja, &numeros[i]),
			     "Se insertó elemento");
		baraja_boca_arriba_elemento(baraja, i);
		pa2m_afirmar(baraja_obtener(baraja, i) == &numeros[i],
			     "El elemento insertado es el esperado.");
	}

	pa2m_afirmar(cantidad_baraja(baraja) == capacidad,
		     "La baraja llegó a su máxima capacidad.");
	pa2m_afirmar(
		!baraja_insertar(baraja, &no_insertado),
		"No se pudo insertar elemento ya que la baraja esta llena.");
	pa2m_afirmar(cantidad_baraja(baraja) == capacidad,
		     "No se aumento su cantidad.");
	pa2m_afirmar(baraja_posicion(baraja, &no_insertado,
				     comparador_numeros) == -1,
		     "Se busca la posicion y no se encuentra.");

	destruir_baraja(baraja);
}

void baraja_insertar_devuelve_false_dado_puntero_null()
{
	int dato = 1;
	pa2m_afirmar(!baraja_insertar(NULL, &dato),
		     "baraja_insertar devuelve false con una baraja nula.");
}

void se_inserta_en_un_elemento_eliminado_anteriormente()
{
	baraja_t *baraja = crear_baraja_completa(5);
	if (!baraja) {
		return;
	}
	int *dato_agregar = malloc(sizeof(int));
	if (!dato_agregar) {
		destruir_baraja_todo(baraja, free);
		return;
	}
	*dato_agregar = 9999;

	pa2m_afirmar(baraja != NULL,
		     "Se creo una baraja completamente llena con 5 elementos.");

	void *dato_eliminado = baraja_eliminar(baraja, 2);
	pa2m_afirmar(dato_eliminado != NULL, "Se elimino un elemento.");
	pa2m_afirmar(baraja_insertar(baraja, dato_agregar),
		     "Se inserta un elemento.");
	baraja_boca_arriba_elemento(baraja, 2);
	void *aux = baraja_obtener(baraja, 2);
	pa2m_afirmar(
		aux == dato_agregar,
		"El elemento se inserto en la posicion donde se elimino uno anteriormente.(obtenido: %p, esperado: %p)",
		aux, dato_agregar);

	free(dato_eliminado);
	destruir_baraja_todo(baraja, free);
}

void se_eliminan_exitosamente_todos_los_elementos_de_una_baraja_llena()
{
	baraja_t *baraja = crear_baraja_completa(5);
	if (!baraja) {
		return;
	}

	pa2m_afirmar(baraja != NULL,
		     "Se creo una baraja completamente llena con 5 elementos.");
	size_t cantidad = cantidad_baraja(baraja);
	for (size_t i = 0; i < 5; i++) {
		int *eliminado = baraja_eliminar(baraja, i);
		pa2m_afirmar(
			*eliminado == i,
			"Se elimino el elemento de la posicion %zu y es el esperado.",
			i);
		pa2m_afirmar(cantidad_baraja(baraja) == cantidad - 1,
			     "Su cantidad bajo en uno.");
		cantidad = cantidad_baraja(baraja);
		free(eliminado);
	}
	pa2m_afirmar(cantidad_baraja(baraja) == 0, "La baraja se quedo vacia");
	destruir_baraja(baraja);
}

void baraja_con_un_elemento_boca_abajo_se_verifica_su_estado()
{
	baraja_t *baraja = crear_baraja_completa(1);

	pa2m_afirmar(
		baraja_estado_de_elemento(baraja, 0) == BOCA_ABAJO,
		"Un espacio de la baraja con estado BOCA_ABAJO, baraja_estado_de_elemento devuelve el estado correspondiente.");

	destruir_baraja_todo(baraja, free);
}

void baraja_con_un_elemento_boca_arriba_se_verifica_su_estado()
{
	baraja_t *baraja = crear_baraja_completa(1);

	baraja_boca_arriba_elemento(baraja, 0);
	pa2m_afirmar(
		baraja_estado_de_elemento(baraja, 0) == BOCA_ARRIBA,
		"Un espacio de la baraja con estado BOCA_ARRIBA, baraja_estado_de_elemento devuelve el estado correspondiente.");

	destruir_baraja_todo(baraja, free);
}

void baraja_con_un_espacio_vacio_se_verifica_su_estado()
{
	baraja_t *baraja = crear_baraja(1);

	pa2m_afirmar(
		baraja_estado_de_elemento(baraja, 0) == VACIA,
		"Un espacio de la baraja con estado VACIA, baraja_estado_de_elemento devuelve el estado correspondiente.");

	destruir_baraja(baraja);
}

void baraja_con_elementos_se_verifica_estado_de_una_posicion_invalida()
{
	baraja_t *baraja = crear_baraja_completa(3);

	pa2m_afirmar(
		baraja_estado_de_elemento(baraja, 10) == -1,
		"Se quiere ver el estado de una posicion inexistente, baraja_estado_de_elemento devuelve -1.");

	destruir_baraja_todo(baraja, free);
}

void baraja_con_elementos_con_estado_intercalado_se_verifican_estados()
{
	baraja_t *baraja = crear_baraja_6_elemetos_intercalados();

	pa2m_afirmar(baraja_estado_de_elemento(baraja, 0) == BOCA_ABAJO,
		     "El estado en esta posicion es BOCA_ABAJO.");
	pa2m_afirmar(baraja_estado_de_elemento(baraja, 1) == BOCA_ARRIBA,
		     "El estado en esta posicion es BOCA_ARRIBA.");
	pa2m_afirmar(baraja_estado_de_elemento(baraja, 2) == VACIA,
		     "El estado en esta posicion es VACIA.");
	pa2m_afirmar(baraja_estado_de_elemento(baraja, 3) == BOCA_ABAJO,
		     "El estado en esta posicion es BOCA_ABAJO.");
	pa2m_afirmar(baraja_estado_de_elemento(baraja, 4) == BOCA_ARRIBA,
		     "El estado en esta posicion es BOCA_ARRIBA.");
	pa2m_afirmar(baraja_estado_de_elemento(baraja, 5) == VACIA,
		     "El estado en esta posicion es VACIA.");
	pa2m_afirmar(baraja_estado_de_elemento(baraja, 6) == -1,
		     "El estado en esta posicion inexistente da -1.");

	destruir_baraja_todo(baraja, free);
}

void baraja_estado_de_elemento_devuelve_error_dada_baraja_nula()
{
	pa2m_afirmar(baraja_estado_de_elemento(NULL, 3) == -1,
		     "baraja_estado_de_elemento devuelve -1 dada baraja nula.");
}

void baraja_vacia_la_cantidad_da_cero()
{
	baraja_t *baraja = crear_baraja(10);

	pa2m_afirmar(cantidad_baraja(baraja) == 0,
		     "Dada una baraja vacia, su cantidad es cero.");

	destruir_baraja(baraja);
}

void baraja_llena_se_verifica_cantidad()
{
	size_t cantidad = 10;
	baraja_t *baraja = crear_baraja_completa(cantidad);

	pa2m_afirmar(cantidad_baraja(baraja) == cantidad,
		     "Dada una baraja llena, su cantidad es la esperada.");

	destruir_baraja_todo(baraja, free);
}

void baraja_con_elementos_de_diferentes_estados_se_verifica_cantidad()
{
	baraja_t *baraja = crear_baraja_6_elemetos_intercalados();

	pa2m_afirmar(
		cantidad_baraja(baraja) == 4,
		"Dada una baraja media llena, su cantidad es la esperada.");

	destruir_baraja_todo(baraja, free);
}

void cantidad_baraja_devuelve_cero_dada_baraja_nula()
{
	pa2m_afirmar(cantidad_baraja(NULL) == 0,
		     "cantidad_baraja devuelve 0, dada baraja nula.");
}

void baraja_con_un_elemento_boca_abajo_se_cambia_boca_arriba()
{
	baraja_t *baraja = crear_baraja_completa(1);

	pa2m_afirmar(baraja_boca_arriba_elemento(baraja, 0),
		     "Se cambia el estado BOCA_ABAJO a BOCA_ARRIBA.");
	pa2m_afirmar(baraja_estado_de_elemento(baraja, 0) == BOCA_ARRIBA,
		     "Se verifica su estado y esta BOCA_ARRIBA.");

	destruir_baraja_todo(baraja, free);
}

void baraja_con_un_elemento_boca_arriba_se_cambia_boca_abajo()
{
	baraja_t *baraja = crear_baraja_completa(1);

	baraja_boca_arriba_elemento(baraja, 0);
	pa2m_afirmar(baraja_boca_abajo_elemento(baraja, 0),
		     "Se cambia el estado BOCA_ARRIBA a BOCA_ABAJO.");
	pa2m_afirmar(baraja_estado_de_elemento(baraja, 0) == BOCA_ABAJO,
		     "Se verifica su estado y esta BOCA_ABAJO.");

	destruir_baraja_todo(baraja, free);
}

void baraja_con_un_elemento_boca_abajo_no_se_puede_cambiar_a_boca_abajo()
{
	baraja_t *baraja = crear_baraja_completa(1);

	pa2m_afirmar(
		!baraja_boca_abajo_elemento(baraja, 0),
		"No se puede cambiar a estado BOCA_ABAJO un elemento que ya esta BOCA_ABAJO.");
	pa2m_afirmar(baraja_estado_de_elemento(baraja, 0) == BOCA_ABAJO,
		     "Se verifica su estado y sigue BOCA_ABAJO.");

	destruir_baraja_todo(baraja, free);
}

void baraja_con_un_elemento_boca_arriba_no_se_puede_cambiar_a_boca_arriba()
{
	baraja_t *baraja = crear_baraja_completa(1);

	baraja_boca_arriba_elemento(baraja, 0);
	pa2m_afirmar(
		!baraja_boca_arriba_elemento(baraja, 0),
		"No se puede cambiar a estado BOCA_ARRIBA un elemento que ya esta BOCA_ARRIBA.");
	pa2m_afirmar(baraja_estado_de_elemento(baraja, 0) == BOCA_ARRIBA,
		     "Se verifica su estado y sigue BOCA_ARRIBA.");

	destruir_baraja_todo(baraja, free);
}

void baraja_vacia_se_intenta_cambiar_de_estado_pero_no_se_puede()
{
	baraja_t *baraja = crear_baraja(1);

	pa2m_afirmar(
		!baraja_boca_abajo_elemento(baraja, 0),
		"No se puede poner BOCA_ARRIBA a una posicion que esta VACIA.");
	pa2m_afirmar(
		!baraja_boca_arriba_elemento(baraja, 0),
		"No se puede poner BOCA_ABAJO a una posicion que esta VACIA.");

	destruir_baraja(baraja);
}

void se_intenta_cambiar_estado_con_una_posicion_inexistente()
{
	baraja_t *baraja = crear_baraja(4);

	pa2m_afirmar(
		!baraja_boca_abajo_elemento(baraja, 29),
		"No se puede poner BOCA_ARRIBA a una posicion inexistente.");
	pa2m_afirmar(
		!baraja_boca_arriba_elemento(baraja, 33),
		"No se puede poner BOCA_ABAJO a una posicion inexistente.");

	destruir_baraja(baraja);
}

void baraja_boca_arriba_elemento_da_false_dada_baraja_nula()
{
	pa2m_afirmar(!baraja_boca_arriba_elemento(NULL, 2),
		     "baraja_boca_arriba_elemento da false dada baraja nula.");
}

void baraja_boca_abajo_elemento_da_false_dada_baraja_nula()
{
	pa2m_afirmar(!baraja_boca_abajo_elemento(NULL, 2),
		     "baraja_boca_abajo_elemento da false dada baraja nula.");
}

void baraja_con_un_elemento_boca_abajo_obtener_da_null()
{
	baraja_t *baraja = crear_baraja_completa(1);

	pa2m_afirmar(
		baraja_obtener(baraja, 0) == NULL,
		"No se puede obtener el dato si la posicion esta con estado BOCA_ABAJO");

	destruir_baraja_todo(baraja, free);
}

void baraja_con_un_elemento_boca_arriba_obtener_da_el_esperado()
{
	baraja_t *baraja = crear_baraja_completa(1);

	baraja_boca_arriba_elemento(baraja, 0);
	int *numero0 = baraja_obtener(baraja, 0);
	pa2m_afirmar(
		*numero0 == 0,
		"Se obtiene el dato con estado BOCA_ARRIBA y es el esperado.");

	destruir_baraja_todo(baraja, free);
}

void baraja_con_un_elemento_vacio_obtener_da_null()
{
	baraja_t *baraja = crear_baraja(1);

	pa2m_afirmar(
		baraja_obtener(baraja, 0) == NULL,
		"No se puede obtener dato de una posicion VACIA y da NULL.");

	destruir_baraja_todo(baraja, free);
}

void se_intenta_obtener_dato_de_una_posicion_inexistente()
{
	baraja_t *baraja = crear_baraja(5);

	pa2m_afirmar(
		baraja_obtener(baraja, 99) == NULL,
		"No se puede obtener dato de posicion inexistente y da NULL.");

	destruir_baraja_todo(baraja, free);
}

void baraja_obtener_devuelve_null_dada_baraja_nula()
{
	pa2m_afirmar(baraja_obtener(NULL, 99) == NULL,
		     "baraja_obtener da NULL dada baraja nula.");
}

void baraja_intercalada_se_buscan_posicion_todos_los_elementos_y_son_correctos()
{
	baraja_t *baraja = crear_baraja_6_elemetos_intercalados();
	int num0 = 0;
	int num1 = 1;
	int num3 = 3;
	int num4 = 4;

	pa2m_afirmar(
		baraja_posicion(baraja, &num0, comparador_numeros) == 0,
		"La posicion del numero 0 es correcta.(Marcado como BOCA_ABAJO)");
	pa2m_afirmar(
		baraja_posicion(baraja, &num1, comparador_numeros) == 1,
		"La posicion del numero 1 es correcta.(Marcado como BOCA_ARRIBA");
	pa2m_afirmar(
		baraja_posicion(baraja, &num3, comparador_numeros) == 3,
		"La posicion del numero 3 es correcta.(Marcado como BOCA_ABAJO)");
	pa2m_afirmar(
		baraja_posicion(baraja, &num4, comparador_numeros) == 4,
		"La posicion del numero 4 es correcta.(Marcado como BOCA_ARRIBA)");

	destruir_baraja_todo(baraja, free);
}

void baraja_valida_se_busca_elemento_no_existente()
{
	baraja_t *baraja = crear_baraja_completa(3);
	int no_existe = 999;

	pa2m_afirmar(baraja_posicion(baraja, &no_existe, comparador_numeros) ==
			     -1,
		     "Se busca posicion de un dato inexistente y da -1.");

	destruir_baraja_todo(baraja, free);
}

void baraja_posicion_retorna_error_dado_punteros_null()
{
	baraja_t *baraja = crear_baraja(1);
	int num = 3;

	pa2m_afirmar(baraja_posicion(NULL, &num, comparador_numeros) == -1,
		     "baraja_posicion da -1 dada baraja nula.");
	pa2m_afirmar(baraja_posicion(baraja, &num, NULL) == -1,
		     "baraja_posicion da -1 dado comparador nulo.");

	destruir_baraja(baraja);
}

void baraja_con_elemento_null_se_obtiene()
{
	baraja_t *baraja = crear_baraja(1);

	pa2m_afirmar(baraja_insertar(baraja, NULL),
		     "Se puede insertar un dato NULL.");
	pa2m_afirmar(baraja_obtener(baraja, 0) == NULL,
		     "El elemento insertado es NULL.");

	destruir_baraja(baraja);
}

void baraja_completa_boca_abajo_se_mezcla()
{
	baraja_t *baraja = crear_baraja_completa(5);
	size_t cant_antes = cantidad_baraja(baraja);

	pa2m_afirmar(
		baraja_mezclar(baraja, 123),
		"Se pudo mezclar una baraja con todos elementos BOCA_ABAJO.");

	size_t cant_despues = cantidad_baraja(baraja);
	pa2m_afirmar(cant_despues == cant_antes,
		     "Su cantidad sigue siendo la misma.");

	int cant_boca_abajo = 0;
	for (int i = 0; i < cant_despues; i++) {
		if (baraja_estado_de_elemento(baraja, (size_t)i) ==
		    BOCA_ABAJO) {
			cant_boca_abajo++;
		}
	}
	pa2m_afirmar(cant_boca_abajo = 5, "Todos siguen boca abajo.");

	destruir_baraja_todo(baraja, free);
}

void baraja_completa_boca_arriba_se_mezcla()
{
	baraja_t *baraja = crear_baraja_completa(4);
	size_t cant_antes = cantidad_baraja(baraja);
	for (size_t i = 0; i < 4; i++) {
		baraja_boca_arriba_elemento(baraja, i);
	}

	pa2m_afirmar(
		baraja_mezclar(baraja, 34),
		"Se pudo mezclar una baraja con todos elementos BOCA_ARRIBA.");
	size_t cant_despues = cantidad_baraja(baraja);
	pa2m_afirmar(cant_despues == cant_antes,
		     "Su cantidad sigue siendo la misma.");

	int cant_boca_arriba = 0;
	int num0 = 0;
	int num1 = 0;
	int num2 = 0;
	int num3 = 0;

	for (size_t i = 0; i < cant_despues; i++) {
		if (baraja_estado_de_elemento(baraja, i) == BOCA_ARRIBA) {
			cant_boca_arriba++;
		}

		int *num = baraja_obtener(baraja, i);
		switch (*num) {
		case 0:
			num0++;
			break;
		case 1:
			num1++;
			break;
		case 2:
			num2++;
			break;
		case 3:
			num3++;
			break;
		}
	}

	pa2m_afirmar(cant_boca_arriba == 4, "Todos siguen boca arriba.");
	pa2m_afirmar(num0 + num1 + num2 + num3 == 4,
		     "Los elementos siguen estando en la baraja.");

	destruir_baraja_todo(baraja, free);
}

void baraja_intercalada_se_mezcla()
{
	baraja_t *baraja = crear_baraja_6_elemetos_intercalados();
	size_t cant_antes = cantidad_baraja(baraja);

	pa2m_afirmar(
		baraja_mezclar(baraja, 34),
		"Se pudo mezclar una baraja con elementos BOCA_ARRIBA y BOCA_ABAJO, y posiciones vacias.");
	size_t cant_despues = cantidad_baraja(baraja);
	pa2m_afirmar(
		cant_despues == cant_antes,
		"Su cantidad sigue siendo la misma.(antes:%zu, despues: %zu)",
		cant_antes, cant_despues);

	int cant_boca_arriba = 0;
	int cant_boca_abajo = 0;
	int cant_vacios = 0;
	int num0 = 0;
	int num1 = 0;
	int num3 = 0;
	int num4 = 0;

	for (size_t i = 0; i < 6; i++) {
		baraja_estado_de_elemento(baraja, i);
		if (baraja_estado_de_elemento(baraja, i) == VACIA) {
			cant_vacios++;
		} else {
			if (baraja_estado_de_elemento(baraja, i) ==
			    BOCA_ARRIBA) {
				cant_boca_arriba++;

			} else if (baraja_estado_de_elemento(baraja, i) ==
				   BOCA_ABAJO) {
				cant_boca_abajo++;
			}

			baraja_boca_arriba_elemento(baraja, i);
			int *num = baraja_obtener(baraja, i);
			switch (*num) {
			case 0:
				num0++;
				break;
			case 1:
				num1++;
				break;
			case 3:
				num3++;
				break;
			case 4:
				num4++;
				break;
			}
		}
	}
	pa2m_afirmar(cant_boca_arriba == 2,
		     "La cantidad de elementos BOCA_ARRIBA es la misma.");
	pa2m_afirmar(cant_boca_abajo == 2,
		     "La cantidad de elementos BOCA_ABAJO es la misma.");
	pa2m_afirmar(cant_vacios == 2,
		     "La cantidad de posiciones VACIO es la misma.");
	pa2m_afirmar(num0 + num1 + num3 + num4 == 4,
		     "Los elementos siguen estando en la baraja.");

	destruir_baraja_todo(baraja, free);
}

void se_intenta_eliminar_con_posicion_inexistente_y_no_se_puede()
{
	baraja_t *baraja = crear_baraja(1);
	pa2m_afirmar(!baraja_eliminar(baraja, 32),
		     "No se puede eliminar nada en una posicion inexistente.");
	destruir_baraja(baraja);
}

bool sumar_elementos(void *elemento, void *extra)
{
	int *num = elemento;
	int *contador = extra;
	*contador += *num;
	return true;
}

void se_iteran_solo_elementos_boca_arriba()
{
	baraja_t *baraja = crear_baraja_6_elemetos_intercalados();
	int contador = 0;

	pa2m_afirmar(baraja_iterador(baraja, BARAJA_RECORRER_BOCA_ARRIBA,
				     sumar_elementos, &contador) == 2,
		     "Se iteran solo elementos BOCA_ARRIBA");
	pa2m_afirmar(contador == 5, "La funcion se realizo correctamente.");

	destruir_baraja_todo(baraja, free);
}

void se_iteran_solo_elementos_boca_abajo()
{
	baraja_t *baraja = crear_baraja_6_elemetos_intercalados();
	int contador = 0;

	pa2m_afirmar(baraja_iterador(baraja, BARAJA_RECORRER_BOCA_ABAJO,
				     sumar_elementos, &contador) == 2,
		     "Se iteran solo elementos BOCA_ABAJO");
	pa2m_afirmar(contador == 3, "La funcion se realizo correctamente.");

	destruir_baraja_todo(baraja, free);
}

void se_iteran_todos_los_elementos()
{
	baraja_t *baraja = crear_baraja_6_elemetos_intercalados();
	int contador = 0;

	pa2m_afirmar(baraja_iterador(baraja, BARAJA_RECORRER_TODAS,
				     sumar_elementos, &contador) == 4,
		     "Se iteran todos los elementos (no vacios).");
	pa2m_afirmar(contador == 8, "La funcion se realizo correctamente.");

	destruir_baraja_todo(baraja, free);
}

void baraja_vacia_no_se_itera()
{
	baraja_t *baraja = crear_baraja(5);

	pa2m_afirmar(baraja_iterador(baraja, BARAJA_RECORRER_TODAS,
				     sumar_elementos, NULL) == 0,
		     "No se itera ningun elemento en una baraja vacia.");

	destruir_baraja(baraja);
}

bool seguir(void *elemnto, void *aux)
{
	return true;
}

void se_puede_iterar_con_extra_nulo()
{
	baraja_t *baraja = crear_baraja_6_elemetos_intercalados();

	pa2m_afirmar(baraja_iterador(baraja, BARAJA_RECORRER_TODAS, seguir,
				     NULL) == 4,
		     "Se iteran todos los elementos (no vacios).");

	destruir_baraja_todo(baraja, free);
}

void baraja_iterador_no_se_itera_dado_punteros_null()
{
	baraja_t *baraja = crear_baraja(5);

	pa2m_afirmar(baraja_iterador(NULL, BARAJA_RECORRER_BOCA_ABAJO,
				     sumar_elementos, NULL) == 0,
		     "baraja_iterador devuelve cero dada baraja nula.");
	pa2m_afirmar(baraja_iterador(baraja, BARAJA_RECORRER_BOCA_ABAJO, NULL,
				     NULL) == 0,
		     "baraja_iterador devuelve cero dada funcion nula.");

	destruir_baraja(baraja);
}

bool sumar_hasta_2(void *num_, void *cont)
{
	int *num = num_;
	int *contador = cont;
	*contador += *num;

	if (*contador >= 2) {
		return false;
	}
	return true;
}

void se_itera_hasta_un_false()
{
	baraja_t *baraja = crear_baraja_completa(5);
	int cont = 0;

	pa2m_afirmar(baraja_iterador(baraja, BARAJA_RECORRER_TODAS,
				     sumar_hasta_2, &cont) == 3,
		     "Se itera hasta que dio false.");
	pa2m_afirmar(cont == 3,
		     "La funcion se aplico a los elementos esperados.");

	destruir_baraja_todo(baraja, free);
}

void baraja_mezclar_devuelve_false_dada_puntero_nul()
{
	pa2m_afirmar(!baraja_mezclar(NULL, 12),
		     "baraja_mezclar devuelve false dada baraja nula.");
}

void baraja_eliminar_devuelve_null_dado_puntero_null()
{
	pa2m_afirmar(!baraja_eliminar(NULL, 12),
		     "baraja_eliminar devuelve false dada baraja nula.");
}

bool iterador_devuelve_true(void *dato, void *extra)
{
	return true;
}

void baraja_pruebas_conjunto()
{
	baraja_t *baraja = crear_baraja(10);
	pa2m_afirmar(baraja != NULL, "Crear baraja con capacidad 10");
	int valores[12];
	for (int i = 0; i < 12; i++)
		valores[i] = i * 10;

	for (int i = 0; i < 10; i++) {
		pa2m_afirmar(baraja_insertar(baraja, &valores[i]),
			     "Insertar elemento en baraja");
	}
	pa2m_afirmar(!baraja_insertar(baraja, &valores[10]),
		     "No insertar si baraja llena");
	pa2m_afirmar(!baraja_insertar(baraja, &valores[11]),
		     "No insertar si baraja llena");

	for (size_t i = 0; i < 5; i++) {
		pa2m_afirmar(baraja_boca_arriba_elemento(baraja, i),
			     "Poner elemento BOCA_ARRIBA");
		pa2m_afirmar(baraja_estado_de_elemento(baraja, i) ==
				     BOCA_ARRIBA,
			     "Estado BOCA_ARRIBA correcto");
	}

	for (size_t i = 0; i < 10; i++) {
		void *elem = baraja_obtener(baraja, i);
		if (i < 5)
			pa2m_afirmar(elem == &valores[i],
				     "Obtener elemento BOCA_ARRIBA correcto");
		else
			pa2m_afirmar(
				elem == NULL,
				"Obtener elemento BOCA_ABAJO devuelve NULL");
	}

	void *elim = baraja_eliminar(baraja, 3);
	pa2m_afirmar(elim == &valores[3], "Eliminar elemento 3 correctamente");
	pa2m_afirmar(baraja_estado_de_elemento(baraja, 3) == VACIA,
		     "Elemento 3 ahora VACIA");

	elim = baraja_eliminar(baraja, 8);
	pa2m_afirmar(elim == &valores[8], "Eliminar elemento 8 correctamente");
	pa2m_afirmar(baraja_estado_de_elemento(baraja, 8) == VACIA,
		     "Elemento 8 ahora VACIA");

	int extra1 = 111, extra2 = 222;
	pa2m_afirmar(baraja_insertar(baraja, &extra1),
		     "Insertar elemento en primer VACIO");
	pa2m_afirmar(baraja_insertar(baraja, &extra2),
		     "Insertar elemento en segundo VACIO");
	pa2m_afirmar(baraja_posicion(baraja, &valores[0], comparador_numeros) ==
			     0,
		     "Buscar elemento existente 0");
	pa2m_afirmar(baraja_posicion(baraja, &extra1, comparador_numeros) != -1,
		     "Buscar elemento extra1");
	int inexistente = 999;
	pa2m_afirmar(baraja_posicion(baraja, &inexistente,
				     comparador_numeros) == -1,
		     "Buscar elemento inexistente devuelve -1");
	pa2m_afirmar(baraja_mezclar(baraja, 123), "Mezclar baraja con semilla");

	size_t recorridos = baraja_iterador(baraja, BARAJA_RECORRER_TODAS,
					    iterador_devuelve_true, NULL);
	pa2m_afirmar(recorridos == 10,
		     "Iterador recorre todos los elementos no VACIOS");
	for (size_t i = 0; i < 10; i++) {
		baraja_eliminar(baraja, i);
	}
	pa2m_afirmar(cantidad_baraja(baraja) == 0,
		     "Cantidad después de eliminar todos es 0");

	destruir_baraja(baraja);
}

//---------TDA MENU---------

const char *MENU_TITULO = "Menú principal";

const char *NOM_OP0 = "opcion 0";

const char *NOM_OP1 = "opcion 1";

const char *NOM_OP2 = "opcion 2";

void se_crea_un_menu_exitosamente()
{
	menu_t *menu = crear_menu("Opciones", 10);

	pa2m_afirmar(menu != NULL, "Se crea un menu exitosamente.");
	char *nombre = menu_nombre(menu);
	pa2m_afirmar(strcmp("Opciones", nombre) == 0,
		     "El nombre guardado en el menú es el mismo.");
	pa2m_afirmar(menu_cantidad(menu) == 0,
		     "La cantidad es cero de un menu recien creado.");

	destruir_menu(menu);
}

void se_crea_menu_con_nombre_vacio()
{
	menu_t *menu = crear_menu("", 10);

	pa2m_afirmar(menu != NULL,
		     "Se crea un menu exitosamente con nombre vacio.");
	char *nombre = menu_nombre(menu);
	pa2m_afirmar(strcmp("", nombre) == 0,
		     "El nombre guardado en el menú es el mismo.");
	pa2m_afirmar(menu_cantidad(menu) == 0,
		     "La cantidad es cero de un menu recien creado.");

	destruir_menu(menu);
}

void crear_menu_devuelve_null_dado_nombre_nulo()
{
	pa2m_afirmar(crear_menu(NULL, 10) == NULL,
		     "crear_menu devuelve NULL dado nombre nulo.");
}

void menu_vacio_su_cantidad_es_cero()
{
	menu_t *menu = crear_menu("Cero", 5);

	pa2m_afirmar(menu_cantidad(menu) == 0,
		     "Dado un menu vacio su cantidad es cero.");

	destruir_menu(menu);
}

void sumar(void *a, void *b)
{
	int *a_ = a;
	int *b_ = b;
	*a_ += *b_;
}

void menu_vacio_se_agregan_opcion()
{
	menu_t *menu = crear_menu("Numero", 5);

	pa2m_afirmar(menu_agregar(menu, "Numero cinco", "C", sumar),
		     "Se agrego exitosamente una opcion en el menu.");
	size_t cant = menu_cantidad(menu);
	pa2m_afirmar(cant == 1, "Su cantidad subio en uno(ob: %zu, es: %zu)",
		     cant, 1);

	destruir_menu(menu);
}

void menu_con_opciones_se_agregan_más()
{
	menu_t *menu = crear_menu("Opciones", 10);
	menu_agregar(menu, "Uno", "A", sumar);
	menu_agregar(menu, "Dos", "B", sumar);
	menu_agregar(menu, "Tres", "C", sumar);
	pa2m_afirmar(menu_agregar(menu, "Cuatro", "D", sumar),
		     "Se agrego correctamente la opcion con tecla D");

	pa2m_afirmar(menu_agregar(menu, "Cinco", "E", sumar),
		     "Se agrego correctamente la opcion con tecla E");

	size_t cant = menu_cantidad(menu);
	pa2m_afirmar(
		cant == 5,
		"La cantidad total de opciones es correcta (ob: %zu, esp: %zu)",
		cant, 5);

	destruir_menu(menu);
}

menu_t *crear_menu_3_op()
{
	menu_t *menu = crear_menu("Hola", 10);
	menu_agregar(menu, NOM_OP1, "a", sumar);
	menu_agregar(menu, NOM_OP2, "b", sumar);
	menu_agregar(menu, NOM_OP0, "c", sumar);
	return menu;
}

void menu_con_opciones_su_cantidad_es_correcta()
{
	menu_t *menu = crear_menu_3_op();

	pa2m_afirmar(
		menu_cantidad(menu) == 3,
		"Dado menu con tres opciones, su cantidad es la esperada.");

	destruir_menu(menu);
}

void menu_cantidad_devuelve_cero_dada_menu_nulo()
{
	pa2m_afirmar(menu_cantidad(NULL) == 0,
		     "menu_cantidad devuelve 0 dado menu nulo.");
}

void se_agrega_opcion_con_nombre_vacio()
{
	menu_t *menu = crear_menu("Numero", 5);

	pa2m_afirmar(
		menu_agregar(menu, "", "C", sumar),
		"Se agrego exitosamente una opcion en el menu con nombre vacio.");
	size_t cant = menu_cantidad(menu);
	pa2m_afirmar(cant == 1, "Su cantidad subio en uno(ob: %zu, es: %zu)",
		     cant, 1);

	destruir_menu(menu);
}

void no_se_agrega_opcion_tecla_vacia()
{
	menu_t *menu = crear_menu("Numero", 5);

	pa2m_afirmar(!menu_agregar(menu, "HOLA", "", sumar),
		     "No se agrega una opcion en el menu con tecla vacia.");
	size_t cant = menu_cantidad(menu);
	pa2m_afirmar(cant == 0, "Su cantidad subio no subio");

	destruir_menu(menu);
}

void no_se_agrega_opcion_mas_de_un_caracter()
{
	menu_t *menu = crear_menu("Numero", 5);

	pa2m_afirmar(
		!menu_agregar(menu, "HOLA", "no dormi nada", sumar),
		"No se agrega una opcion en el menu con tecla con mas de un caracter.");
	size_t cant = menu_cantidad(menu);
	pa2m_afirmar(cant == 0, "Su cantidad subio no subio");

	destruir_menu(menu);
}

void menu_agregar_devuelve_false_dado_punteros_null()
{
	menu_t *menu = crear_menu("Numero", 5);

	pa2m_afirmar(!menu_agregar(NULL, "ojalaestebien", "A", sumar),
		     "menu_agregar devuelve false dado menu nulo.");
	pa2m_afirmar(!menu_agregar(menu, NULL, "A", sumar),
		     "menu_agregar devuelve false dado nombre nulo.");
	pa2m_afirmar(!menu_agregar(menu, "ojalaestebien", NULL, sumar),
		     "menu_agregar devuelve false dado tecla nulo.");
	pa2m_afirmar(!menu_agregar(menu, "ojalaestebien", "A", NULL),
		     "menu_agregar devuelve false dado funcion nula.");

	destruir_menu(menu);
}

void baraja_con_elementos_se_ejecutan()
{
	menu_t *menu = crear_menu_3_op();
	int uno = 1;
	int dos = 2;

	pa2m_afirmar(menu_ejecutar_opcion(menu, "a", &uno, &dos),
		     "Se ejecuto la primera opcion del menu.");
	pa2m_afirmar(uno == 3, "La ejecucion hizo lo esperado.");
	pa2m_afirmar(menu_ejecutar_opcion(menu, "b", &uno, &dos),
		     "Se ejecuto la segunda opcion del menu.");
	pa2m_afirmar(uno == 5, "La ejecucion hizo lo esperado.");
	pa2m_afirmar(menu_ejecutar_opcion(menu, "c", &uno, &dos),
		     "Se ejecuto la tercera opcion del menu.");
	pa2m_afirmar(uno == 7, "La ejecucion hizo lo esperado.");

	destruir_menu(menu);
}

void no_se_ejecuta_dada_tecla_con_mas_de_un_caracter()
{
	menu_t *menu = crear_menu_3_op();
	pa2m_afirmar(
		!menu_ejecutar_opcion(menu, "PIPIPI", NULL, NULL),
		"No se puede ejecutar si la tecla tienen mas de un caracter.");

	destruir_menu(menu);
}

void no_se_ejecuta_dada_tecla_inexistente()
{
	menu_t *menu = crear_menu_3_op();
	pa2m_afirmar(!menu_ejecutar_opcion(menu, "z", NULL, NULL),
		     "No se puede ejecutar ina operacion inexistente.");

	destruir_menu(menu);
}

void no_se_ejecuta_dada_tecla_vacia()
{
	menu_t *menu = crear_menu_3_op();
	pa2m_afirmar(!menu_ejecutar_opcion(menu, "", NULL, NULL),
		     "No se puede ejecutar si la tecla esta vacia.");

	destruir_menu(menu);
}

void menu_ejecutar_opcion_devuelve_false_dado_punteros_null()
{
	menu_t *menu = crear_menu("Numero", 5);
	pa2m_afirmar(!menu_ejecutar_opcion(NULL, "a", NULL, NULL),
		     "menu_ejecutar_opcion devuelve false dado menu nulo.");
	pa2m_afirmar(!menu_ejecutar_opcion(menu, NULL, NULL, NULL),
		     "menu_ejecutar_opcion devuelve false dada tecla nula.");

	destruir_menu(menu);
}

void se_eliminan_todos_los_elementos_de_menu()
{
	menu_t *menu = crear_menu_3_op();
	size_t cant_actual = menu_cantidad(menu);

	pa2m_afirmar(menu_eliminar_opcion(menu, "a"),
		     "Se pudo eliminar una opcion del menu.");
	pa2m_afirmar(menu_cantidad(menu) == cant_actual - 1,
		     "Su cantidad bajo en uno.");
	cant_actual = menu_cantidad(menu);
	pa2m_afirmar(menu_eliminar_opcion(menu, "b"),
		     "Se pudo eliminar una opcion del menu.");
	pa2m_afirmar(menu_cantidad(menu) == cant_actual - 1,
		     "Su cantidad bajo en uno.");
	cant_actual = menu_cantidad(menu);
	pa2m_afirmar(menu_eliminar_opcion(menu, "c"),
		     "Se pudo eliminar una opcion del menu.");
	pa2m_afirmar(menu_cantidad(menu) == cant_actual - 1,
		     "Su cantidad bajo en uno.");

	destruir_menu(menu);
}

void no_se_elimina_dada_tecla_invalida()
{
	menu_t *menu = crear_menu_3_op();
	size_t cant_actual = menu_cantidad(menu);

	pa2m_afirmar(!menu_eliminar_opcion(menu, ""),
		     "No se pudo eliminar una opcion dada tecla vacia.");
	pa2m_afirmar(menu_cantidad(menu) == cant_actual,
		     "Su cantidad no bajo.");
	cant_actual = menu_cantidad(menu);
	pa2m_afirmar(!menu_eliminar_opcion(menu, "asdas"),
		     "No se pudo eliminar una opcion dada tecla larga.");
	pa2m_afirmar(menu_cantidad(menu) == cant_actual,
		     "Su cantidad no bajo.");

	destruir_menu(menu);
}

void menu_eliminar_opcion_devuelve_false_dado_punteros_null()
{
	menu_t *menu = crear_menu_3_op();

	pa2m_afirmar(!menu_eliminar_opcion(NULL, "a"),
		     "menu_eliminar_opcion devuelve false dado menu nulo.");
	pa2m_afirmar(!menu_eliminar_opcion(menu, NULL),
		     "menu_eliminar_opcion devuelve false dada tecla nula.");

	destruir_menu(menu);
}

bool contador_op(char *letra, char *nombre, void *dato)
{
	int *contador = dato;
	(*contador)++;
	return true;
}

void menu_iterar_devuelve_cero_dado_punteros_null()
{
	menu_t *menu = crear_menu_3_op();

	pa2m_afirmar(menu_iterar(NULL, contador_op, NULL) == 0,
		     "menu_iterar devuelve cero dado menu nulo.");
	pa2m_afirmar(menu_iterar(menu, NULL, NULL) == 0,
		     "menu_iterar devuelve cero dada funcion nula.");

	destruir_menu(menu);
}

void se_itera_todas_las_op()
{
	menu_t *menu = crear_menu_3_op();
	int contador = 0;
	pa2m_afirmar(
		menu_iterar(menu, contador_op, &contador) == 3,
		"Menu_iterar devuelve la cantidad esperada de elementos iterador.");
	pa2m_afirmar(contador == 3,
		     "Se recorrieron la cantidad de operaciones esperadas.");

	destruir_menu(menu);
}

bool iterador_dummy(char *tecla, char *nombre, void *aux)
{
	if (aux)
		(*(int *)aux)++;
	return true;
}

void menu_pruebas_conjunto()
{
	menu_t *menu = crear_menu("Menu Principal", 3);
	pa2m_afirmar(menu != NULL, "Crear menu es exitoso con capacidad 3");

	int aux_valor = 0;
	pa2m_afirmar(menu_agregar(menu, "Opcion 1", "a", sumar),
		     "Agregar opcion a");
	pa2m_afirmar(menu_agregar(menu, "Opcion 2", "b", sumar),
		     "Agregar opcion b");
	pa2m_afirmar(menu_agregar(menu, "Opcion 3", "c", sumar),
		     "Agregar opcion c");
	pa2m_afirmar(menu_agregar(menu, "Opcion 4", "d", sumar),
		     "Agregar opcion d");
	pa2m_afirmar(menu_agregar(menu, "Opcion 5", "e", sumar),
		     "Agregar opcion e");
	pa2m_afirmar(menu_cantidad(menu) == 5, "Cantidad de opciones es 5");
	pa2m_afirmar(strcmp(menu_nombre(menu), "Menu Principal") == 0,
		     "Nombre del menu correcto");
	pa2m_afirmar(menu_ejecutar_opcion(menu, "a", &aux_valor, &(int){ 2 }),
		     "Ejecutar opcion a suma 2");
	pa2m_afirmar(aux_valor == 2, "Verificar valor después de ejecutar a");
	pa2m_afirmar(menu_ejecutar_opcion(menu, "c", &aux_valor, &(int){ 3 }),
		     "Ejecutar opcion c suma 3");
	pa2m_afirmar(aux_valor == 5, "Verificar valor después de ejecutar c");
	pa2m_afirmar(!menu_ejecutar_opcion(menu, "z", &aux_valor, &(int){ 1 }),
		     "Ejecutar opcion inexistente devuelve false");
	pa2m_afirmar(menu_eliminar_opcion(menu, "b"), "Eliminar opcion b");
	pa2m_afirmar(!menu_eliminar_opcion(menu, "b"),
		     "Eliminar opcion b de nuevo devuelve false");
	pa2m_afirmar(menu_cantidad(menu) == 4, "Cantidad de opciones ahora 4");

	int contador = 0;
	size_t recorridos = menu_iterar(menu, iterador_dummy, &contador);
	pa2m_afirmar(recorridos == 4, "Iterador recorre 4 opciones");
	pa2m_afirmar(contador == 4, "Contador incrementado por iterador");
	pa2m_afirmar(menu_agregar(menu, "Opcion 6", "f", sumar),
		     "Agregar opcion f");
	pa2m_afirmar(menu_agregar(menu, "Opcion 7", "g", sumar),
		     "Agregar opcion g");
	pa2m_afirmar(menu_ejecutar_opcion(menu, "f", &aux_valor, &(int){ 10 }),
		     "Ejecutar opcion f suma 10");
	pa2m_afirmar(menu_ejecutar_opcion(menu, "g", &aux_valor, &(int){ 5 }),
		     "Ejecutar opcion g suma 5");
	pa2m_afirmar(aux_valor == 20,
		     "Verificar valor acumulado después de ejecutar f y g");

	char teclas[] = { 'a', 'c', 'd', 'e', 'f', 'g' };
	for (int i = 0; i < 6; i++)
		menu_eliminar_opcion(menu, (char[]){ teclas[i], '\0' });
	pa2m_afirmar(menu_cantidad(menu) == 0,
		     "Cantidad de opciones después de eliminar todas es 0");

	destruir_menu(menu);
}

int main()
{
	pa2m_nuevo_grupo("============== TDA BARAJA ===============");
	pa2m_nuevo_grupo("CREACION:");
	se_crea_baraja_exitosamente_con_capacidad_valida();
	no_se_crea_baraja_dada_capacidad_invalida();

	pa2m_nuevo_grupo("ESTADO DE ELEMENTO:");
	baraja_con_un_elemento_boca_abajo_se_verifica_su_estado();
	baraja_con_un_elemento_boca_arriba_se_verifica_su_estado();
	baraja_con_un_espacio_vacio_se_verifica_su_estado();
	baraja_con_elementos_se_verifica_estado_de_una_posicion_invalida();
	baraja_con_elementos_con_estado_intercalado_se_verifican_estados();

	pa2m_nuevo_grupo("CAMBIAR ESTADO:");
	baraja_con_un_elemento_boca_abajo_se_cambia_boca_arriba();
	baraja_con_un_elemento_boca_arriba_se_cambia_boca_abajo();
	baraja_vacia_se_intenta_cambiar_de_estado_pero_no_se_puede();
	baraja_con_un_elemento_boca_abajo_no_se_puede_cambiar_a_boca_abajo();
	baraja_con_un_elemento_boca_arriba_no_se_puede_cambiar_a_boca_arriba();
	se_intenta_cambiar_estado_con_una_posicion_inexistente();

	pa2m_nuevo_grupo("CANTIDAD:");
	baraja_vacia_la_cantidad_da_cero();
	baraja_llena_se_verifica_cantidad();
	baraja_con_elementos_de_diferentes_estados_se_verifica_cantidad();

	pa2m_nuevo_grupo("OBTENER:");
	baraja_con_un_elemento_boca_abajo_obtener_da_null();
	baraja_con_un_elemento_boca_arriba_obtener_da_el_esperado();
	baraja_con_un_elemento_vacio_obtener_da_null();
	se_intenta_obtener_dato_de_una_posicion_inexistente();

	pa2m_nuevo_grupo("POSICION:");
	baraja_intercalada_se_buscan_posicion_todos_los_elementos_y_son_correctos();
	baraja_valida_se_busca_elemento_no_existente();

	pa2m_nuevo_grupo("INSERTADO:");
	baraja_recien_creada_se_agrega_un_elemento();
	se_inserta_elementos_hasta_el_maximo();
	se_inserta_en_un_elemento_eliminado_anteriormente();
	baraja_con_elemento_null_se_obtiene();

	pa2m_nuevo_grupo("MEZCLA:");
	baraja_completa_boca_abajo_se_mezcla();
	baraja_completa_boca_arriba_se_mezcla();
	baraja_intercalada_se_mezcla();
	se_puede_iterar_con_extra_nulo();

	pa2m_nuevo_grupo("ITERADOR:");
	se_iteran_solo_elementos_boca_arriba();
	se_iteran_solo_elementos_boca_abajo();
	se_iteran_todos_los_elementos();
	baraja_vacia_no_se_itera();
	se_itera_hasta_un_false();

	pa2m_nuevo_grupo("ELIMINACION:");
	se_eliminan_exitosamente_todos_los_elementos_de_una_baraja_llena();
	se_intenta_eliminar_con_posicion_inexistente_y_no_se_puede();

	pa2m_nuevo_grupo("PUNTEROS NULL:");
	baraja_insertar_devuelve_false_dado_puntero_null();
	baraja_estado_de_elemento_devuelve_error_dada_baraja_nula();
	cantidad_baraja_devuelve_cero_dada_baraja_nula();
	baraja_boca_arriba_elemento_da_false_dada_baraja_nula();
	baraja_boca_abajo_elemento_da_false_dada_baraja_nula();
	baraja_obtener_devuelve_null_dada_baraja_nula();
	baraja_posicion_retorna_error_dado_punteros_null();
	baraja_iterador_no_se_itera_dado_punteros_null();
	baraja_mezclar_devuelve_false_dada_puntero_nul();
	baraja_eliminar_devuelve_null_dado_puntero_null();

	pa2m_nuevo_grupo("PRUEBAS EN CONJUNTO:");
	baraja_pruebas_conjunto();

	pa2m_nuevo_grupo("============== TDA MENU ===============");
	pa2m_nuevo_grupo("CREACION:");
	se_crea_un_menu_exitosamente();
	se_crea_menu_con_nombre_vacio();

	pa2m_nuevo_grupo("CANTIDAD:");
	menu_vacio_su_cantidad_es_cero();
	menu_con_opciones_su_cantidad_es_correcta();

	pa2m_nuevo_grupo("AGREGAR:");
	menu_vacio_se_agregan_opcion();
	menu_con_opciones_se_agregan_más();
	se_agrega_opcion_con_nombre_vacio();
	no_se_agrega_opcion_tecla_vacia();
	no_se_agrega_opcion_mas_de_un_caracter();

	pa2m_nuevo_grupo("EJECUTAR:");
	baraja_con_elementos_se_ejecutan();
	no_se_ejecuta_dada_tecla_con_mas_de_un_caracter();
	no_se_ejecuta_dada_tecla_vacia();
	no_se_ejecuta_dada_tecla_inexistente();

	pa2m_nuevo_grupo("ELIMINAR:");
	se_eliminan_todos_los_elementos_de_menu();
	no_se_elimina_dada_tecla_invalida();

	pa2m_nuevo_grupo("ITERAR:");
	se_itera_todas_las_op();

	pa2m_nuevo_grupo("PUNTEROS NULL:");
	crear_menu_devuelve_null_dado_nombre_nulo();
	menu_cantidad_devuelve_cero_dada_menu_nulo();
	menu_agregar_devuelve_false_dado_punteros_null();
	menu_ejecutar_opcion_devuelve_false_dado_punteros_null();
	menu_eliminar_opcion_devuelve_false_dado_punteros_null();
	menu_iterar_devuelve_cero_dado_punteros_null();

	pa2m_nuevo_grupo("PRUEBAS EN CONJUNTO:");
	menu_pruebas_conjunto();

	return pa2m_mostrar_reporte();
}
