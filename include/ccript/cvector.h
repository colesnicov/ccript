/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file cvector.h
 * @brief Definice dynamickeho kontejneru pro ulozeni dat.
 *
 * @version 1b0
 * @date 26.06.2022
 *
 * @author Denis Colesnicov <eugustus@gmail.com>
 *
 * @copyright MIT License
 *
 * @details <i>Original: https://eddmann.com/posts/implementing-a-dynamic-vector-array-in-c/ and modified.</i>
 *
 */

#pragma once

#include "ccript/cc_configs.h"
#include <stdbool.h>

/**
 * @def CVECTOR_ASSERT
 * @brief Assert funkce pro kontrolu stavu parseru.
 *
 * @note Kdyz neni treba, CC_ASSERT(expr) nahradit za (0)..
 *
 */
#define CVECTOR_ASSERT(expr)	CONFIG_CC_ASSERT(expr)

/**
 * @def CVECTOR_MALLOC
 * @brief Makro pro alokaci nove pameti.
 *
 */
#define CVECTOR_MALLOC(s)		CONFIG_CC_MALLOC(s)

/**
 * @def CVECTOR_REALLOC
 * @brief  Makro pro alokaci dodatecne pameti.
 *
 */
#define CVECTOR_REALLOC(c, s)	CONFIG_CC_REALLOC(c,s)

/**
 * @def CVECTOR_FREE
 * @brief Makro pro uvolneni pameti.
 *
 */
#define CVECTOR_FREE(p)			CONFIG_CC_FREE(p)

/**
 * @def CONFIG_CVECTOR_DEBUG_ON
 * @brief Vypisovat dodatecne informace?
 *
 */
#define CONFIG_CVECTOR_DEBUG_ON		0

/**
 * @def CONFIG_CVECTOR_INC_CAPACITY
 * @brief O kolik navysovat kapacitu kontejneru?
 *
 * @see CONFIG_CVECTOR_INC_OPERATOR
 *
 */
#define CONFIG_CVECTOR_INC_CAPACITY	2

/**
 * @def CONFIG_CVECTOR_INC_OPERATOR
 * @brief Operator pro navyseni velikosti kontejneru.
 *
 * @see CONFIG_CVECTOR_INC_CAPACITY
 *
 * @note Pozor, nenasobuje se ale scita se: total <b>+</b> CONFIG_CVECTOR_INC_CAPACITY...
 *
 */
#define CONFIG_CVECTOR_INC_OPERATOR	+

/**
 * @def CONFIG_CVECTOR_INIT_CAPACITY
 * @brief Vychozi velikost kontejneru.
 *
 */
#define CONFIG_CVECTOR_INIT_CAPACITY	4

/**
 * @fn void cvector_fn_dealloc(void* _item)
 * @brief Prototyp funkce pro uvolneni prostredku.
 *
 * @param _item Ukazatel na data k uvolneni
 *
 * @fn void (*cvector_fn_dealloc)(void*)
 * @brief Typ prototypu funkce pro uvolneni prostredku.
 */
typedef void (*cvector_fn_dealloc)(void*);

/**
 * @struct cvector_
 * @brief Struktura pro praci s kontejnerem.
 *
 * @typedef cvector_ cvector_s
 * @brief Typ kontejneru
 * @see cvector
 *
 */
typedef struct cvector_ {
	/**
	 * @var void **items
	 * @brief Pole ukazatelu na data
	 */
	void **items;
	/**
	 * @var size_t capacity
	 * @brief Celkova 'jiz rezervovana' velikost kontejneru.
	 */
	size_t capacity;
	/**
	 * @var size_t total
	 * @brief Kolik prvku uz v kontejneru je ulozeno.
	 */
	size_t total;

} cvector_s;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @fn bool cvector_init(cvector_s*)
 * @brief Inicializuje prostredky pro kontejner.
 *
 * @note Call only once.
 *
 * @see CONFIG_CVECTOR_INIT_CAPACITY
 *
 * @param v Ukazatel na kontejner.
 *
 * @return FALSE jestli neni dostatek volne pameti, jinak TRUE.
 */
bool cvector_init(cvector_s *v);

/**
 * @fn void cvector_deinit(cvector_s*)
 * @brief Uvolni prostredky po kontejneru.
 *
 * @param v Ukazatel na kontejner
 */
void cvector_deinit(cvector_s *v);

/**
 * @fn void cvector_deinit_dealloc(cvector_s*, cvector_fn_dealloc)
 * @brief Uvolni prostredky po kontejneru a zaroven zavola uzivatelskou
 * 			funkci pro uvolneni prostredku. Tato funkce bude volana pro
 * 			kazdou polozku zvlast. Takto: <code>fn_free(stored_item);</code>.
 *
 * @param v Ukazatel na kontejner.
 * @param fn_free Funkce pro uvolneni prostredku po prvku, napr f-ce 'free()'.
 */
void cvector_deinit_dealloc(cvector_s *v, cvector_fn_dealloc fn_free);

/**
 * @fn int cvector_total(cvector_s*)
 * @brief Vrati pocet ulozenych prvku.
 *
 * @param v Ukazatel na kontejner.
 *
 * @return Pocet ulozenych prvku.
 */
int cvector_total(cvector_s *v);

/**
 * @fn bool cvector_resize(cvector_s*, size_t)
 * @brief Zvetsi velikost uloziste.
 *
 * @note Nove pozadovana velikost musi byt aspon o 1 vetsi nez soucasna!
 *
 * @see CONFIG_CVECTOR_INC_CAPACITY
 *
 * @param v Ukazatel na kontejner.
 * @param capacity Nova velikost kontejneru.
 *
 * @return FALSE jestli neni dostatek volne pameti, jinak TRUE.
 */
bool cvector_resize(cvector_s *v, size_t capacity);

/**
 * @fn bool cvector_add(cvector_s*, void*)
 * @brief Prida prvek do kontejneru.
 *
 * @param v Ukazatel na kontejner.
 * @param item Prvek ke vlozeni.
 *
 * @return FALSE jestli neni dostatek volne pameti, jinak TRUE.
 */
bool cvector_add(cvector_s *v, void *item);

/**
 * @fn bool cvector_set(cvector_s*, int, void*)
 * @brief Vlozi prvek do kontejneru na zadany index.
 *
 * @note Index prvku musi byt v mezich '0' <= index < 'cvector_total()'!
 *
 * @param v Ukazatel na kontejner.
 * @param index Index, kam se ma vlozit prvek.
 * @param item Prvek ke vlozeni.
 *
 * @return FALSE jestli index je mimo rozsah, jinak TRUE.
 */
bool cvector_set(cvector_s *v, int index, void *item);

/**
 * @fn void* cvector_get(cvector_s*, int)
 * @brief Vrati prvek ze zadane pozice.
 *
 * @note Index prvku musi byt v mezich '0' <= index < 'cvector_total()'!
 *
 * @param v Ukazatel na kontejner.
 * @param index index Index pozice na ktere se prvek nachazi.
 *
 * @return Prvek ze zadane pozice, jestli nalezen, jinak NULL.
 */
void* cvector_get(cvector_s *v, int index);

/**
 * @fn bool cvector_delete(cvector_s*, int)
 * @brief Odstrani prvek z kontejneru.
 *
 * @param v Ukazatel na kontejner.
 * @param index Index pozice na ktere se prvek nachazi.
 *
 * @return FALSE jestli neni dostatek volne pameti, jinak TRUE.
 */
bool cvector_delete(cvector_s *v, int index);

/**
 * @fn bool cvector_delete_dealloc(cvector_s*, int, cvector_fn_dealloc)
 * @brief Odstrani prvek z kontejneru a zaroven zavola uzivatelskou
 * 			funkci pro uvolneni prostredku.
 *
 * @param v Ukazatel na kontejner.
 * @param index Index pozice na ktere se prvek nachazi.
 * @param fn_free Funkce pro uvolneni prostredku po prvku, napr f-ce 'free()'.
 *
 * @return FALSE jestli neni dostatek volne pameti, jinak TRUE.
 */
bool cvector_delete_dealloc(cvector_s *v, int index, cvector_fn_dealloc fn_free);

#ifdef __cplusplus
}
#endif

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
