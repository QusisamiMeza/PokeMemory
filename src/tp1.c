#include "tp1.h"
#include "auxiliares_tp1.h"

struct tp1 {
	struct pokemon *pokemones;
	struct pokemon **pokemones_ordenados_id;
	size_t cantidad_pokemones;
};

tp1_t *tp1_leer_archivo(const char *nombre)
{
	if (!nombre) {
		return NULL;
	}
	FILE *archivo = fopen(nombre, "r");
	if (!archivo) {
		return NULL;
	}

	char *linea = leer_linea(archivo);
	if (linea == NULL) {
		fclose(archivo);
		return NULL;
	}

	tp1_t *nuevo_tp1 = malloc(sizeof(tp1_t));
	if (!nuevo_tp1) {
		return NULL;
	}
	nuevo_tp1->cantidad_pokemones = 0;
	nuevo_tp1->pokemones = NULL;

	while (linea != NULL) {
		struct pokemon *pokemon = parsear_pokemon(linea);
		if (pokemon != NULL) {
			if (tp1_buscar_id(nuevo_tp1, pokemon->id) == NULL) {
				agregar_pokemon(&nuevo_tp1->pokemones,
						&nuevo_tp1->cantidad_pokemones,
						*pokemon);
			}
			free(pokemon->nombre);
			free(pokemon);
		}
		free(linea);
		linea = leer_linea(archivo);
	}
	fclose(archivo);
	nuevo_tp1->pokemones_ordenados_id = asignar_pokemones_ordenados(
		nuevo_tp1->pokemones, nuevo_tp1->cantidad_pokemones,
		es_menor_id);

	return nuevo_tp1;
}

size_t tp1_cantidad(tp1_t *tp1)
{
	if (!tp1) {
		return 0;
	}
	return tp1->cantidad_pokemones;
}

tp1_t *tp1_guardar_archivo(tp1_t *tp1, const char *nombre)
{
	if (!tp1 || !nombre) {
		return NULL;
	}

	FILE *archivo = fopen(nombre, "w");
	if (!archivo) {
		return NULL;
	}
	tp1_con_cada_pokemon(tp1, escribir_pokemones_archivo, archivo);
	fclose(archivo);
	return tp1;
}

struct pokemon *tp1_buscar_id(tp1_t *tp, int id)
{
	if (!tp) {
		return NULL;
	}

	struct pokemon *pok_encontrado = NULL;
	size_t i = 0;
	while (i < tp->cantidad_pokemones && pok_encontrado == NULL) {
		if (tp->pokemones[i].id == id) {
			pok_encontrado = &tp->pokemones[i];
		}
		i++;
	}
	return pok_encontrado;
}

tp1_t *tp1_union(tp1_t *un_tp, tp1_t *otro_tp)
{
	if (!un_tp || !otro_tp ||
	    (un_tp->cantidad_pokemones == 0 &&
	     otro_tp->cantidad_pokemones == 0)) {
		return NULL;
	}

	tp1_t *union_tp1 = calloc(1, sizeof(tp1_t));
	if (!union_tp1) {
		return NULL;
	}
	size_t i = 0, j = 0;

	while (i < un_tp->cantidad_pokemones &&
	       j < otro_tp->cantidad_pokemones) {
		int id_a = un_tp->pokemones_ordenados_id[i]->id;
		int id_b = otro_tp->pokemones_ordenados_id[j]->id;

		if (id_a < id_b) {
			agregar_pokemon(&union_tp1->pokemones,
					&union_tp1->cantidad_pokemones,
					*un_tp->pokemones_ordenados_id[i]);
			i++;
		} else if (id_a > id_b) {
			agregar_pokemon(&union_tp1->pokemones,
					&union_tp1->cantidad_pokemones,
					*otro_tp->pokemones_ordenados_id[j]);
			j++;
		} else {
			agregar_pokemon(&union_tp1->pokemones,
					&union_tp1->cantidad_pokemones,
					*un_tp->pokemones_ordenados_id[i]);
			i++;
			j++;
		}
	}

	while (i < un_tp->cantidad_pokemones) {
		agregar_pokemon(&union_tp1->pokemones,
				&union_tp1->cantidad_pokemones,
				*un_tp->pokemones_ordenados_id[i]);
		i++;
	}

	while (j < otro_tp->cantidad_pokemones) {
		agregar_pokemon(&union_tp1->pokemones,
				&union_tp1->cantidad_pokemones,
				*otro_tp->pokemones_ordenados_id[j]);
		j++;
	}

	union_tp1->pokemones_ordenados_id = malloc(
		sizeof(struct pokemon *) * union_tp1->cantidad_pokemones);
	for (size_t k = 0; k < union_tp1->cantidad_pokemones; k++) {
		union_tp1->pokemones_ordenados_id[k] = &union_tp1->pokemones[k];
	}

	return union_tp1;
}

tp1_t *tp1_interseccion(tp1_t *un_tp, tp1_t *otro_tp)
{
	if (!un_tp || !otro_tp || un_tp->cantidad_pokemones == 0 ||
	    otro_tp->cantidad_pokemones == 0) {
		return NULL;
	}
	tp1_t *interseccion_tp1 = calloc(1, sizeof(tp1_t));

	if (!interseccion_tp1) {
		return NULL;
	}
	size_t i = 0, j = 0;

	while (i < un_tp->cantidad_pokemones &&
	       j < otro_tp->cantidad_pokemones) {
		int id_a = un_tp->pokemones_ordenados_id[i]->id;
		int id_b = otro_tp->pokemones_ordenados_id[j]->id;

		if (id_a < id_b) {
			i++;
		} else if (id_a > id_b) {
			j++;
		} else {
			agregar_pokemon(&interseccion_tp1->pokemones,
					&interseccion_tp1->cantidad_pokemones,
					*un_tp->pokemones_ordenados_id[i]);
			i++;
			j++;
		}
	}

	interseccion_tp1->pokemones_ordenados_id =
		malloc(sizeof(struct pokemon *) *
		       interseccion_tp1->cantidad_pokemones);
	for (size_t k = 0; k < interseccion_tp1->cantidad_pokemones; k++) {
		interseccion_tp1->pokemones_ordenados_id[k] =
			&interseccion_tp1->pokemones[k];
	}

	return interseccion_tp1;
}

tp1_t *tp1_diferencia(tp1_t *un_tp, tp1_t *otro_tp)
{
	if (!un_tp || !otro_tp || un_tp->cantidad_pokemones == 0) {
		return NULL;
	}
	tp1_t *diferencia_tp1 = calloc(1, sizeof(tp1_t));

	if (!diferencia_tp1) {
		return NULL;
	}
	size_t i = 0, j = 0;

	while (i < un_tp->cantidad_pokemones &&
	       j < otro_tp->cantidad_pokemones) {
		int id_a = un_tp->pokemones_ordenados_id[i]->id;
		int id_b = otro_tp->pokemones_ordenados_id[j]->id;

		if (id_a < id_b) {
			agregar_pokemon(&diferencia_tp1->pokemones,
					&diferencia_tp1->cantidad_pokemones,
					*un_tp->pokemones_ordenados_id[i]);
			i++;
		} else if (id_a > id_b) {
			j++;
		} else {
			i++;
			j++;
		}
	}

	while (i < un_tp->cantidad_pokemones) {
		agregar_pokemon(&diferencia_tp1->pokemones,
				&diferencia_tp1->cantidad_pokemones,
				*un_tp->pokemones_ordenados_id[i]);
		i++;
	}

	diferencia_tp1->pokemones_ordenados_id = malloc(
		sizeof(struct pokemon *) * diferencia_tp1->cantidad_pokemones);
	for (size_t k = 0; k < diferencia_tp1->cantidad_pokemones; k++) {
		diferencia_tp1->pokemones_ordenados_id[k] =
			&diferencia_tp1->pokemones[k];
	}

	return diferencia_tp1;
}

struct pokemon *tp1_buscar_nombre(tp1_t *tp, const char *nombre)
{
	if (!tp || !nombre) {
		return NULL;
	}
	struct pokemon *pok_encontrado = NULL;
	size_t i = 0;
	while (i < tp->cantidad_pokemones && pok_encontrado == NULL) {
		if (strcmp(tp->pokemones[i].nombre, nombre) == 0) {
			pok_encontrado = &tp->pokemones[i];
		}
		i++;
	}
	return pok_encontrado;
}

size_t tp1_con_cada_pokemon(tp1_t *un_tp, bool (*f)(struct pokemon *, void *),
			    void *extra)
{
	if (!un_tp || !f || un_tp->cantidad_pokemones == 0) {
		return 0;
	}

	size_t cantidad_pok_cambiados = 0;
	bool seguir_f = true;
	while (cantidad_pok_cambiados < un_tp->cantidad_pokemones && seguir_f) {
		seguir_f =
			f(un_tp->pokemones_ordenados_id[cantidad_pok_cambiados],
			  extra);
		cantidad_pok_cambiados++;
	}

	return cantidad_pok_cambiados;
}

void tp1_destruir(tp1_t *tp1)
{
	if (!tp1) {
		return;
	}
	destruir_pokemones(tp1->pokemones, tp1->cantidad_pokemones);
	free(tp1->pokemones_ordenados_id);
	free(tp1);
}