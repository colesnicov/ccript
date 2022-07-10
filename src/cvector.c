/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file cvector.c
 * @brief Implementace dynamickeho kontejneru pro ulozeni dat
 *
 * @version 1b0
 * @date 26.06.2022
 *
 * @author Denis Colesnicov <eugustus@gmail.com>
 *
 * @copyright MIT License
 *
 * @details Original: https://eddmann.com/posts/implementing-a-dynamic-vector-array-in-c/ and modified
 */

#include <ccript/cc_configs.h>
#include <ccript/cc_log.h>
#include <stdbool.h>
#include "ccript/cvector.h"
#include <stdlib.h>

bool cvector_init(cvector_s *v) {
	CVECTOR_ASSERT(v->items == NULL && "VECTOR ALREADY INITIALIZED!");

	v->items = CVECTOR_MALLOC(sizeof(void*) * CONFIG_CVECTOR_INIT_CAPACITY);
	if (!v->items) {
		return false;
	}
	v->capacity = CONFIG_CVECTOR_INIT_CAPACITY;
	v->total = 0;

	return true;
}

int cvector_total(cvector_s *v) {
	CVECTOR_ASSERT(v->items != NULL && "VECTOR NOT INITIALIZED!");

	return v->total;
}

bool cvector_resize(cvector_s *v, size_t capacity) {
	CVECTOR_ASSERT(v->items != NULL && "VECTOR NOT INITIALIZED!");

#if CONFIG_CVECTOR_DEBUG_ON & 1
	printf("cvector_resize: %d to %d\n", v->capacity, capacity);
#endif

	void **items = CVECTOR_REALLOC(v->items, sizeof(void*) * capacity);
	if (!items) {
		return false;
	}

	v->items = items;
	v->capacity = capacity;

	return true;
}

bool cvector_add(cvector_s *v, void *item) {
	CVECTOR_ASSERT(v->items != NULL && "VECTOR NOT INITIALIZED!");

	if (v->capacity == v->total) {
		if (!cvector_resize(v, v->capacity + CONFIG_CVECTOR_INC_CAPACITY)) {
			return false;
		}
	}
	v->items[v->total++] = item;

	return true;
}

bool cvector_set(cvector_s *v, int index, void *item) {
	CVECTOR_ASSERT(v->items != NULL && "VECTOR NOT INITIALIZED!");

	if (index >= 0 && index < v->total) {
		v->items[index] = item;
		return true;
	}

	return false;
}

void* cvector_get(cvector_s *v, int index) {
	CVECTOR_ASSERT(v->items != NULL && "VECTOR NOT INITIALIZED!");

	if (index >= 0 && index < v->total) {
		return v->items[index];
	}

	return NULL;
}

bool cvector_delete(cvector_s *v, int index) {
	CVECTOR_ASSERT(v->items != NULL && "VECTOR NOT INITIALIZED!");

	if (index < 0 || index >= v->total) {
		return false;
	}

	v->items[index] = NULL;

	for (int i = index; i < v->total - 1; i++) {
		v->items[i] = v->items[i + 1];
		v->items[i + 1] = NULL;
	}

	v->total--;

	if (v->total > 0 && v->total == v->capacity - (CONFIG_CVECTOR_INC_CAPACITY * 2)) {
		return cvector_resize(v, v->capacity - CONFIG_CVECTOR_INC_CAPACITY);
	}

	return true;
}

bool cvector_delete_dealloc(cvector_s *v, int index, cvector_fn_dealloc fn_free) {
	CVECTOR_ASSERT(v->items != NULL && "VECTOR NOT INITIALIZED!");

	if (index < 0 || index >= v->total) {
		CONFIG_CC_PRINT("dealloc -1[%d]", index);
		return false;
	}

	fn_free(v->items[index]);
	v->items[index] = NULL;

	for (int i = index; i < v->total - 1; i++) {
		v->items[i] = v->items[i + 1];
		v->items[i + 1] = NULL;
	}

	v->total--;

	if (v->total > 0 && v->total == v->capacity - (CONFIG_CVECTOR_INC_CAPACITY * 2)) {
		return cvector_resize(v, v->capacity - CONFIG_CVECTOR_INC_CAPACITY);
	}

	return true;
}

void cvector_deinit(cvector_s *v) {
	CVECTOR_ASSERT(v->items != NULL && "VECTOR NOT INITIALIZED!");

	CVECTOR_FREE(v->items);
	v->items = NULL;
}

void cvector_deinit_dealloc(cvector_s *v, cvector_fn_dealloc fn_free) {
	CVECTOR_ASSERT(v->items != NULL && "VECTOR NOT INITIALIZED!");

	for (int index = 0; index < v->total; index++) {
		fn_free(v->items[index]);
		v->items[index] = NULL;
	}

	CONFIG_CC_FREE(v->items);
}
///
///
///
///
///
///
///
///
///
///
///
///
///
///
///
///
///
///
///
///
///
