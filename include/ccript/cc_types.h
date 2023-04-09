/*
 * This file is a part of "CCRipt" Embeddable Custom 'C' Like Scripting Language Interpreter
 */

/**
 * @file cc_types.h
 * @brief Definice typu a struktur.
 * @since 26.06.2022
 *
 * @version 1r1
 * @date 08.04.2023
 *
 * @author Denis Colesnicov <eugustus@gmail.com>
 *
 * @copyright MIT License
 *
 */

#pragma once

#include <filebuffer/filebuffer.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "cvector/cvector.h"
#include "ccript/cc_configs.h"

/**
 * @def CC_FUNC_DEBUG
 * @brief Vypise informace o zpracovani funkci na STD.
 * @see CC_PRINT
 *
 */
#if CONFIG_CC_FUNC_DEBUG
#define CC_FUNC_DEBUG(...)	CC_PRINT(__VA_ARGS__)
#else
#define CC_FUNC_DEBUG(...)
#endif

/**
 * @def CC_BLOCK_DEBUG
 * @brief Vypise informace o zpracovani bloku na STD.
 * @see CC_PRINT
 *
 */
#if CONFIG_CC_BLOCK_DEBUG
#define CC_BLOCK_DEBUG(...)	CC_PRINT(__VA_ARGS__)
#else
#define CC_BLOCK_DEBUG(...)
#endif

/**
 * @def CC_VAR_DEBUG
 * @brief Vypise informace o zpracovani promenne na STD.
 * @see CC_PRINT
 *
 */
#if CONFIG_CC_VAR_DEBUG
#define CC_VAR_DEBUG(...)	CC_PRINT(__VA_ARGS__)
#else
#define CC_VAR_DEBUG(...)
#endif

/**
 * @def CC_MALLOC
 * @brief Rezervuje pamet.
 * @see CONFIG_CC_MALLOC
 *
 */
#define CC_MALLOC		CONFIG_CC_MALLOC

/**
 * @def CC_REALLOC
 * @brief Prerezervuje pamet.
 * @see CONFIG_CC_RELLOC
 *
 */
#define CC_REALLOC		CONFIG_CC_RELLOC

/**
 * @def CC_FREE
 * @brief Uvolni pamet.
 * @see CONFIG_CC_FREE
 *
 */
#define CC_FREE		CONFIG_CC_FREE

/**
 * @def CC_COMMENT_LEN
 * @brief Velikost zasobniku pro komentar.
 * @details Tolik mista bude rezervovano v pameti RAM pro zasobnik komentare.
 * @see CC_PRINT_COMMENT
 *
 */
#define CC_COMMENT_LEN			CONFIG_CC_COMMENT_LEN - 1

/**
 * @def CC_PRINT_COMMENT
 * @brief Vypisovat komentare do konzole?
 * @see CC_PRINT
 *
 */
#define CC_PRINT_COMMENT			CONFIG_CC_PRINT_COMMENT

/**
 * @def CC_ASSIGN_EMPTY
 * @brief Podpora prirazeni prazdneho redezce/znaku.
 *
 */
#define CC_ASSIGN_EMPTY			CONFIG_CC_ASSIGN_EMPTY

/**
 * @def CC_IF_COND_MATH
 * @brief Podpora matematickych operaci v podminkach.
 * @details Napriklad: `if(1 + 2 == 3)`
 *
 */
#define CC_IF_COND_MATH			CONFIG_CC_IF_COND_MATH

/**
 * @def CC_FLOAT_EXP_LEN
 * @brief Delka exponentu cisla s plovouci desetinnou carkou.
 * @details Pouziva se pri prevodu promenne typu `float` na typ `string`
 * 			<code><pre>
 * 			12.345;
 * 			   ^^^
 * 			</pre></code>
 *
 */
#define CC_FLOAT_EXP_LEN			CONFIG_CC_FLOAT_EXP_LEN

/**
 * @def CC_BUFFER_LEN
 * @brief Velikost zasobniku pro skript.
 * @details Tolik mista bude rezervovano v pameti RAM pro zasobnik skriptu nacteneho ze souboru.
 *
 */
#define CC_BUFFER_LEN			CONFIG_CC_BUFFER_LEN

/**
 * @def CC_PRINT_ENV
 * @brief Vypsat promenne prostredi do konzole?
 * @see CC_PRINT
 *
 */
#define CC_PRINT_ENV			CONFIG_CC_PRINT_ENV

/**
 * @def	CC_KEYWORD_LEN
 * @brief Velikost zasobniku pro nazev promenne/bloku/funkce
 * @details Toto je soucasne maximalni delka nazvu promenne, funkce a vyrazu (while, break, continue, ...)
 * @see CONFIG_CC_KEYWORD_LEN - 1 znak.
 */
#define CC_KEYWORD_LEN 		CONFIG_CC_KEYWORD_LEN - 1

/**
 * @def CC_VALUE_LONG_LEN
 * @brief Velikost zasobniku pro cislo vcetne ukoncovaci '\0'. int, long a float.
 * @details Tolik mista bude rezervovano v pameti RAM.
 * @details Vysledny vyraz bude o 1 znak kratsi!
 *
 */
#define CC_VALUE_LONG_LEN		sizeof(long) * CONFIG_CC_VALUE_LONG_LEN

/**
 * @def CC_VALUE_NUMERIC_LEN
 * @brief Velikost zasobniku pro retezec - 1 znak.
 * @see CONFIG_CC_STRING_LEN
 *
 */
#define CC_VALUE_NUMERIC_LEN			CONFIG_CC_NUMERIC_LEN - 1

/**
 * @def CC_VALUE_STRING_LEN
 * @brief Velikost zasobniku pro retezec - 1 znak.
 * @see CONFIG_CC_STRING_LEN
 *
 */
#define CC_VALUE_STRING_LEN			CONFIG_CC_STRING_LEN - 1

/**
 * @def CC_COMMENT_SIZE
 * @brief Velikost zasobniku pro komentar - 1 znak.
 * @see CONFIG_CC_COMMENT_LEN
 */
#define CC_COMMENT_SIZE		CONFIG_CC_COMMENT_LEN - 1

/**
 * @def CC_BUFFER_SIZE
 * @brief Velikost zasobniku pro skript nacteneho ze souboru - 1 znak.
 * @see CONFIG_CC_BUFFER_LEN
 */
#define CC_BUFFER_SIZE 		CONFIG_CC_BUFFER_LEN - 1

/**
 * @def CC_FLOAT_EXP_LEN
 * @brief Delka exponentu cisla s plovouci desetinnou carkou.
 * @details Pouziva se pri prevodu promenne typu `float` na typ `string`
 * @see CONFIG_CC_FLOAT_EXP_LEN
 */
#define CC_FLOAT_EXP_LEN	CONFIG_CC_FLOAT_EXP_LEN

/**
 * @def CC_FUNC_NUMS_ARGS
 * @brief Maximalni pocet argumentu ktery funkce muze prijmout.
 * @see CONFIG_CC_FUNC_NUMS_ARGS
 *
 */
#define CC_FUNC_NUMS_ARGS	CONFIG_CC_FUNC_NUMS_ARGS

/**
 * @def CC_ASSERT
 * @brief ASSERT funkce.
 * @see CONFIG_CC_ASSERT
 *
 */
#define CC_ASSERT(expr)	CONFIG_CC_ASSERT(expr)

/**
 * @def CC_PRINT
 * @brief Funkce pro vypis do konzole.
 * @see CONFIG_CC_PRINT
 *
 */
#define CC_PRINT(...)	CONFIG_CC_PRINT(__VA_ARGS__)

/**
 * @def PARSER_DEFAULT
 * @brief Pomucka pro snazsi vytvoreni vychozi (neinicializovane) struktury parseru
 * @see cc_init(cc_parser_s*)
 * @see cc_parser_
 *
 */
#define PARSER_DEFAULT()	{0, 0,  0, 0, CC_CODE_OK, NULL, NULL, NULL, NULL, NULL}

/// forward declarations
///
struct var_;
struct cc_parser_;

/**
 * @enum cc_code_
 * @brief Vycet navratovych kodu ktere ovlivnuji praci interpreteru.
 *
 * @typedef cc_code_ cc_code_t
 * @brief Datovy typ pro KOD chyby.
 */
typedef enum cc_code_ {

	/**
	 * @var CC_CODE_OK
	 * @brief Kod chyby ktery signalizuje ze je vse OK.
	 *
	 */
	CC_CODE_OK, /**< Kod chyby ktery signalizuje ze je vse OK. */

	/**
	 * @var CC_CODE_RETURN
	 * @brief Kod chyby ktery signalizuje ze byl nalezen vyraz 'RETURN'
	 *
	 */
	CC_CODE_RETURN, /**< Kod chyby ktery signalizuje ze byl nalezen vyraz 'RETURN' */

	/**
	 * @var CC_CODE_CONTINUE
	 * @brief Kod chyby ktery signalizuje ze byl nalezen vyraz 'CONTINUE'
	 *
	 */
	CC_CODE_CONTINUE, /**< Kod chyby ktery signalizuje ze byl nalezen vyraz 'CONTINUE' */

	/**
	 * @var CC_CODE_BREAK
	 * @brief Kod chyby ktery signalizuje ze byl nalezen vyraz 'BREAK'
	 *
	 */
	CC_CODE_BREAK, /**< Kod chyby ktery signalizuje ze byl nalezen vyraz 'BREAK' */

	/**
	 * @var CC_CODE_ERROR
	 * @brief Kod chyby ktery signalizuje ze doslo k nejake chybe.
	 * @details Pouziva se hlavne k detekci zda k chybe doslo
	 * @see cc_errorHas()
	 *
	 */
	CC_CODE_ERROR, /**< Kod chyby ktery signalizuje ze doslo k nejake chybe. */

	/**
	 * @var CC_CODE_BAD_SYMBOL
	 * @brief Kod chyby ktery signalizuje ze soucasny znak v zasobniku neodpovida ocekavanemu.
	 *
	 */
	CC_CODE_BAD_SYMBOL, /**< Kod chyby ktery signalizuje ze soucasny znak v zasobniku neodpovida ocekavanemu. */

	/**
	 * @var CC_CODE_KEYWORD
	 * @brief Kod chyby ktery signalizuje ze vyraz je spatny.
	 *
	 */
	CC_CODE_KEYWORD, /**< Kod chyby ktery signalizuje ze vyraz je spatny. */

	/**
	 * @var CC_CODE_KEYWORD_EMPTY
	 * @brief Kod chyby ktery signalizuje ze vyraz je prazdny.
	 *
	 */
	CC_CODE_KEYWORD_EMPTY, /**< Kod chyby ktery signalizuje ze vyraz je prazdny. */

	/**
	 * @var CC_CODE_LOGIC
	 * @brief Kod chyby ktery signalizuje logickou chybu.
	 *
	 */
	CC_CODE_LOGIC, /**< Kod chyby ktery signalizuje logickou chybu. */

	/**
	 * @var CC_CODE_NO_FILE
	 //	 * @brief Kod chyby ktery signalizuje ze soubor nenalezen.
	 *
	 */
	CC_CODE_NO_FILE, /**< Kod chyby ktery signalizuje chybu cteni/zapisu souboru. */

	/**
	 * @var CC_CODE_IO
	 * @brief Kod chyby ktery signalizuje chybu cteni/zapisu souboru.
	 *
	 */
	CC_CODE_IO, /**< Kod chyby ktery signalizuje chybu cteni/zapisu zasobniku/souboru. */

	/**
	 * @var CC_CODE_FUNC_NOT_DEFINED
	 * @brief Kod chyby ktery signalizuje o zavolani nedefinovane funkce.
	 *
	 */
	CC_CODE_FUNC_NOT_DEFINED, /**< Kod chyby ktery signalizuje o zavolani nedefinovane funkce. */

	/**
	 * @var CC_CODE_FUNC_ARGS_ERROR
	 * @brief Kod chyby ktery signalizuje o chybe v argumentech funkce
	 *
	 */
	CC_CODE_FUNC_ARGS_ERROR, /**< Kod chyby ktery signalizuje o chybe v argumentech funkce */

	/**
	 * @var CC_CODE_FUNC_EXISTS
	 * @brief Kod chyby ktery signalizuje o registraci funkce se stejnym nazvem.
	 *
	 */
	CC_CODE_FUNC_EXISTS, /**< Kod chyby ktery signalizuje o registraci funkce se stejnym nazvem. */

	/**
	 * @var CC_CODE_FUNC_RET_BAD_TYPE
	 * @brief Kod chyby ktery signalizuje ze funkce vratila stejny typ.
	 *
	 */
	CC_CODE_FUNC_RET_BAD_TYPE, /**< Kod chyby ktery signalizuje ze funkce vratila stejny typ. */

	/**
	 * @var CC_CODE_VAR_NOT_DEFINED
	 * @brief Kod chyby ktery signalizuje pokus o pristup k nedefinovane promenne.
	 *
	 */
	CC_CODE_VAR_NOT_DEFINED, /**< Kod chyby ktery signalizuje pokus o pristup k nedefinovane promenne. */

	/**
	 * @var CC_CODE_VAR_NOT_ASSIGNED
	 * @brief Kod chyby ktery signalizuje pokus o pristup k datum neinicializovane promenne.
	 *
	 */
	CC_CODE_VAR_NOT_ASSIGNED, /**< Kod chyby ktery signalizuje pokus o pristup k datum neinicializovane promenne. */

	/**
	 * @var CC_CODE_VAR_BAD_TYPE
	 * @brief Kod chyby ktery signalizuje pokus o pristup k datum promenne jineho typu.
	 *
	 */
	CC_CODE_VAR_BAD_TYPE, /**< Kod chyby ktery signalizuje pokus o pristup k datum promenne jineho typu. */

	/**
	 * @var CC_CODE_ARGS_BAD_TYPE
	 * @brief Kod chyby ktery signalizuje pokus o pristup k datum argumentu jineho typu.
	 *
	 */
	CC_CODE_ARGS_BAD_TYPE, /**< Kod chyby ktery signalizuje pokus o pristup k datum argumentu jineho typu. */

	/**
	 * @var CC_CODE_VAR_EXISTS
	 * @brief Kod chyby ktery signalizuje duplicitni promennou.
	 *
	 */
	CC_CODE_VAR_EXISTS, /**< Kod chyby ktery signalizuje duplicitni promennou. */

	/**
	 * @var CC_CODE_NOT_MEM
	 * @brief Kod chyby ktery signalizuje nemoznost rezervovat pamet.
	 * @details Proste nedostatek volne pameti RAM.
	 */
	CC_CODE_NOT_MEM, /**< Proste nedostatek volne pameti RAM. */

	/**
	 * @var CC_CODE_NOT_COMMENT
	 * @brief Kod chyby ktery signalizuje pokus o zpracovani bloku ktery neni'komentar'.
	 *
	 */
	CC_CODE_NOT_COMMENT, /**< Kod chyby ktery signalizuje pokus o zpracovani bloku ktery neni'komentar'. */

	/**
	 * @var CC_CODE_NOT_BOOL
	 * @brief Kod chyby ktery signalizuje pokus o operaci nad promennou ktera neni typu 'bool'.
	 *
	 */
	CC_CODE_NOT_BOOL, /**< Kod chyby ktery signalizuje pokus o operaci nad promennou ktera neni typu 'bool'. */

	/**
	 * @var CC_CODE_NOT_INTEGER
	 * @brief Kod chyby ktery signalizuje pokus o operaci nad promennou ktera neni typu 'int'.
	 *
	 */
	CC_CODE_NOT_INTEGER, /**< Kod chyby ktery signalizuje pokus o operaci nad promennou ktera neni typu 'int'. */

	/**
	 * @var CC_CODE_NOT_FLOAT
	 * @brief Kod chyby ktery signalizuje pokus o operaci nad promennou ktera neni typu 'float'.
	 *
	 */
	CC_CODE_NOT_FLOAT, /**< Kod chyby ktery signalizuje pokus o operaci nad promennou ktera neni typu 'float'. */

	/**
	 * @var CC_CODE_NOT_CHAR
	 * @brief Kod chyby ktery signalizuje pokus o operaci nad promennou ktera neni typu 'char'.
	 *
	 */
	CC_CODE_NOT_CHAR, /**< Kod chyby ktery signalizuje pokus o operaci nad promennou ktera neni typu 'char'. */

	/**
	 * @var CC_CODE_NOT_STRING
	 * @brief Kod chyby ktery signalizuje pokus o operaci nad promennou ktera neni typu 'string'.
	 *
	 */
	CC_CODE_NOT_STRING, /**< Kod chyby ktery signalizuje pokus o operaci nad promennou ktera neni typu 'string'. */

	/**
	 * @var CC_CODE_NOT_ARRAY
	 * @brief Kod chyby ktery signalizuje pokus o operaci nad promennou ktera neni typu 'pole'.
	 * @note <b>NOT IMPLEMENTED</b>
	 *
	 */
	CC_CODE_NOT_ARRAY, /**< Kod chyby ktery signalizuje pokus o operaci nad promennou ktera neni typu 'pole'. */

	/**
	 * @var CC_CODE_OUT_OF_LOOP
	 * @brief Kod chyby ktery signalizuje pokus o volani prikazu dostupneho pouze ve smycce.
	 *
	 */
	CC_CODE_OUT_OF_LOOP, /**< Kod chyby ktery signalizuje pokus o volani prikazu dostupneho pouze ve smycce. */

	/**
	 * @var CC_CODE_ABORT
	 * @brief Kod chyby ktery signalizuje vynucene ukonceni zpracovani skriptu
	 *
	 */
	CC_CODE_ABORT, /**< Kod chyby ktery signalizuje vynucene ukonceni zpracovani skriptu */

	/**
	 * @var CC_CODE_TYPE_UNKNOWN
	 * @brief Kod chyby ktery signalizuje neznamy datovy typ
	 *
	 */
	CC_CODE_TYPE_UNKNOWN, /**< Kod chyby ktery signalizuje neznamy datovy typ */

	/**
	 * @var CC_CODE_VALUE_EMPTY
	 * @brief Kod chyby ktery signalizuje ze vyraz je spatny.
	 *
	 */
	CC_CODE_VALUE_EMPTY, /**< Kod chyby ktery signalizuje ze vyraz je spatny. */

	/**
	 * @var CC_CODE_STRING_TOO_LONG
	 * @brief Kod chyby ktery signalizuje delku retezce prekracujici maximalni moznou..
	 *
	 */
	CC_CODE_STRING_TOO_LONG, /**< Kod chyby ktery signalizuje delku retezce prekracujici maximalni moznou.. */

	/**
	 * @var CC_CODE_ZERO_DIVIDED
	 * @brief Kod chyby ktery signalizuje delku retezce prekracujici maximalni moznou..
	 *
	 */
	CC_CODE_ZERO_DIVIDED, /**< Deleni nulou */

	/**
	 * @var CC_CODE_NOT_IMPLEMENTED
	 * @brief Kod chyby ktery signalizuje delku retezce prekracujici maximalni moznou..
	 *
	 */
	CC_CODE_NOT_IMPLEMENTED /**< Kod chyby ze funkce neni implementovana */

} cc_code_t;

/**
 * @enum cc_op_
 * @brief Vycet matematyckych operaci.
 *
 * @typedef cc_op_ cc_op_e
 * @brief Typ logicke/matematycke operace.
 */
typedef enum cc_op_ {

	/**
	 * @var  CC_OP_NONE
	 * @brief Zadna operace.
	 */
	CC_OP_NONE,

	/**
	 * @var  CC_OP_DIV
	 * @brief Matematycka operace 'deleni'.
	 */
	CC_OP_DIV,

	/**
	 * @var  CC_OP_MUL
	 * @brief Matematycka operace 'nasobeni'.
	 */
	CC_OP_MUL,

	/**
	 * @var  CC_OP_SUB
	 * @brief Matematycka operace 'substrakce/odecitani'.
	 */
	CC_OP_SUB,

	/**
	 * @var  CC_OP_SUM
	 * @brief Matematycka operace 'scitani'.
	 */
	CC_OP_SUM,

	/**
	 * @var  CC_OP_AND
	 * @brief Logicka operace 'a take'.
	 */
	CC_OP_AND,

	/**
	 * @var  CC_OP_OR
	 * @brief Logicka operace 'nebo'.
	 */
	CC_OP_OR,

	/**
	 * @var  CC_OP_GREAT
	 * @brief Logicka operace 'vetsi nez'.
	 */
	CC_OP_GREAT,

	/**
	 * @var  CC_OP_LESS
	 * @brief Logicka operace 'menzi nez'.
	 */
	CC_OP_LESS,

	/**
	 * @var  CC_OP_GREAT_EQ
	 * @brief Logicka operace 'vetsi nez nebo rovna se'.
	 */
	CC_OP_GREAT_EQ,

	/**
	 * @var  CC_OP_LESS_EQ
	 * @brief Logicka operace 'mensi nez nebo rovna se'.
	 */
	CC_OP_LESS_EQ
} cc_op_e;

/**
 * @enum cc_type_
 * @brief Vycet typu promennych
 *
 * @typedef cc_type_ cc_type_t
 * @brief Typ promenne
 *
 */
typedef enum cc_type_ {

	/**
	 * @var CC_TYPE_BOOL
	 * @brief Datovy typ pro pravdivostni hodnotu
	 * @details Rozsah hodnot TRUE (1) a FALSE (0)
	 */
	CC_TYPE_BOOL, /**< Datovy typ pro pravdivostni hodnotu */

	/**
	 * @var CC_TYPE_INT
	 * @brief Index datoveho typu pro cele cislo se znamenkem
	 * @details Rozsah hodnot -2,147,483,648 az 2,147,483,647
	 *
	 */
	CC_TYPE_INT, /**< Index datoveho typu pro cele cislo se znamenkem */

	/**
	 * @var CC_TYPE_LONG
	 * @brief Index datoveho typu pro cele cislo se znamenkem
	 * @details Rozsah hodnot -2,147,483,648 az 2,147,483,647
	 *
	 */
	CC_TYPE_LONG, /**< Index datoveho typu pro cele cislo se znamenkem */

	/**
	 * @var CC_TYPE_FLOAT
	 * @brief Index datoveho typu pro cislo s plovouci desetinnou carkou a znamenkem
	 * @details Rozsah hodnot -3.4028235E+38 az 3.4028235E+38
	 *
	 */
	CC_TYPE_FLOAT, /**< Index datoveho typu pro cislo s plovouci desetinnou carkou a znamenkem */

	/**
	 * @var CC_TYPE_CHAR
	 * @brief Index datoveho typu pro cele cislo se znamenkem
	 * @details Rozsah hodnot -128 az 127
	 *
	 */
	CC_TYPE_CHAR, /**< Index datoveho typu pro cel */

	/**
	 * @var CC_TYPE_STRING
	 * @brief Index datoveho typu pro retezec (pole znaku 'CC_TYPE_CHAR')
	 * @details Delka od 0 do CC_VALUE_STRING_LEN
	 * @see CC_VALUE_STRING_LEN
	 *
	 */
	CC_TYPE_STRING, /**< Index datoveho typu pro retezec (pole znaku 'CC_TYPE_CHAR') */

	/**
	 * @var CC_TYPE_ARRAY
	 * @brief Index datoveho typu pro pole
	 * @details Rozsah hodnot 0
	 * @note NOT IMPLEMENTED
	 *
	 */
	CC_TYPE_ARRAY, /**< Index datoveho typu pro pole */

	/**
	 * @var CC_TYPE_MAX
	 * @brief Toto cislo prezentuje mnozstvi datovych typu
	 * @details Pouziva se vnitrne k detekci spatneho typu promenne.
	 * 			Nemusi dojit k definici spatneho typu promenne ale co kdyby...
	 * @note Protoze vycet typu zacina od indexu '1', je skutecny pocet  'CC_TYPE_MAX - 1'!
	 *
	 */
	CC_TYPE_MAX, /**< Toto cislo prezentuje mnozstvi datovych typu */

} cc_type_t;

/**
 * @fn var_s * cc_fn_prototype(cc_parser_s*, var_s**, uint8_t)
 * @brief Prototyp 'C' funkce pro privazani k parseru
 *
 *
 * @param _parser Ukazatel na strukturu parseru
 * @param _vars Pole ukazatelu na struktury promennych
 * @param _vars_count Pocet promennych v poli
 * @param _args Ukazatel na externi argument pro funkci
 *
 * @return	Ukazatel na strukturu promenne
 * @note	Vracena promenna, se musi po ukonceni praci s ni
 * 			odstranit volanim funkce @see varDestroy()
 */
typedef struct var_* (*cc_fn_prototype)(struct cc_parser_ *_parser, struct var_ **_vars,
		uint8_t _vars_count, void *_args);

/**
 * @struct cc_env_
 * @brief Promenna 'prostredi'.
 *
 * @typedef  cc_env_ cc_env_s
 * @brief Typ cc_env_
 * @see cc_env_
 *
 */
typedef struct cc_env_ {

	/**
	 * @var const char *name
	 * @brief Nazev podle promenne.
	 */
	const char *name;

	/**
	 * @var cc_type_t type
	 * @brief Typ dat.
	 */
	cc_type_t type;

	/**
	 * @var void *data
	 * @brief Ukazatel na data.
	 * @details Data se pred pouzitim musi pretypovat podle jejich typu.
	 */
	void *data;
} cc_env_s;

/**
 * @struct fn_handler_
 * @brief Struktura pro udrzeni informaci o dostupne 'C' funkci pro volani ze skript
 *
 * @typedef fn_handler_ fn_handler_s
 * @brief Typ fn_handler_
 * @see fn_handler_
 *
 */
typedef struct fn_handler_ {

	/**
	 * @var const char *name
	 * @brief Nazev funkce
	 */
	char *name;

	/**
	 * @var cc_fn_prototype func
	 * @brief Ukazatel na funkci
	 */
	cc_fn_prototype func;

	/**
	 * @var void* args
	 * @brief Ukazatel na argumenty z venci..
	 */
	void *args;

} fn_handler_s;

/**
 * @struct var_
 * @brief Struktura pro promennou
 *
 * @typedef var_ var_s
 * @brief Typ var_
 * @see var_
 *
 */
typedef struct var_ {

	/**
	 * @var	cc_type_t type
	 * @brief Datovy typ promenne.
	 * @see cc_type_t
	 */
	cc_type_t type;

	/**
	 * @var	bool valid
	 * @brief TRUE jestli promenna byla inicializovana, jinak FALSE.
	 */
	bool valid;

	/**
	 * @var	char *name
	 * @brief Nazev promenne.
	 */
	char *name;

	/**
	 * @var	uint8_t scope
	 * @brief Hloubka zanoreni @see cc_parser_s.depth
	 */
	uint8_t scope;

	/**
	 * @var	void *data
	 * @brief Data
	 */
	void *data;

} var_s;

/**
 * @struct cc_block_args_
 * @brief Definice argumentu pro funkcni blok.
 *
 * @typedef cc_block_args_ cc_block_args_s
 * @brief Typ cc_block_args_
 * @see cc_block_args_
 */
typedef struct cc_block_args_ {

	/**
	 * @var	cc_type_t type
	 * @brief Datovy typ promenne.
	 * @see cc_type_t
	 */
	cc_type_t type;

	/**
	 * @var size_t name_len
	 * @brief Delka nazvu prmonne.
	 */
	size_t name_len;

	/**
	 * @var	char *name
	 * @brief Nazev promenne.
	 */
	char *name;

} cc_block_args_s;

/**
 * @struct cc_block_
 * @brief Struktura definice funkcniho bloku.
 *
 *
 * @typedef cc_block_ cc_block_s
 * @brief Typ cc_block_
 * @see buffer_
 *
 */
typedef struct cc_block_ {

	/**
	 * @var char *name
	 * @brief Nazev.
	 */
	char *name;

	/**
	 * @var size_t name_len
	 * @brief Delka nazvu.
	 */
	size_t name_len;

	/**
	 * @var size_t pos_start
	 * @brief Pozice kde zacina.
	 */
	size_t pos_start;

	/**
	 * @var size_t pos_end
	 * @brief Pozice kde konci.
	 */
	size_t pos_end;

	/**
	 * @var uint8_t args_count
	 * @brief Pocet argumentu ktere prijima.
	 */
	uint8_t args_count;

	/**
	 * @var cc_block_args_s **args
	 * @brief Argumenty ktere prijima.
	 */
	cc_block_args_s **args;

	/**
	 * @var var_s *value
	 * @brief To co vraci.
	 */
	var_s *value;

} cc_block_s;

/**
 * @struct buffer_
 * @brief Struktura zasobniku se skriptem
 *
 * @typedef buffer_ buffer_s
 * @brief Typ buffer_
 * @see buffer_
 *
 */
typedef struct buffer_ {

	/**
	 * @var FILE* fd
	 * @brief Popisovac na otevreny sobor.
	 */
	FILE *fd;

	/**
	 * @var size_t fpos
	 * @brief Pozice v zasobniku.
	 */
	size_t fpos;

	/**
	 * @var size_t offset
	 * @brief Offset zasobniku v souboru. pozice v souboru se rovna offset + fpos.
	 */
	size_t offset;

	/**
	 * @var  long int fsize
	 * @brief Celkova delka dat v souboru.
	 */
	long int fsize;

	/**
	 * @var size_t length
	 * @brief Delka zasobniku nacteneho ze souboru.
	 */
	size_t length;

	/**
	 * @var char *data
	 * @brief Zasobnik nacteny ze souboru.
	 */
	char *data;

} buffer_s;

/**
 * @struct cc_parser_
 * @brief Struktura parseru
 *
 * @typedef cc_parser_ cc_parser_s
 * @brief Typ cc_parser_
 * @see cc_parser_
 *
 */
typedef struct cc_parser_ {

	/**
	 * @var int8_t inLoop
	 * @brief Jestli je vetsi nez 0, nachazim se ve smycce.
	 */
	int8_t inLoop;

	/**
	 * @var uint8_t depth
	 * @brief Hloubka zanoreni.
	 */
	uint8_t depth;

	/**
	 * @var uint8_t env_count
	 * @brief Pocet promennych 'prostredi'.
	 */
	uint8_t env_count;

	/**
	 * @var size_t error_pos
	 * @brief Pozice ve skripte, kde doslo k chybe.
	 */
	size_t error_pos; // pozice kde doslo k potencialni chybe. // vzdy, pred rizikovym mistem musim volat parseSetStop(_parser)

	/**
	 * @var cc_code_t error
	 * @brief Pracovni statovy KOD parseru.
	 * @details Tento kod signalizuje pracovni stav parseru.
	 */
	cc_code_t error;

	/**
	 * @var cvector_s *funcs
	 * @brief Ukazatel na strukturu kontejneru s funkcemi ktere muze volat skript
	 */
	cvector_s *funcs;

	/**
	 * @var cvector_s *blocks
	 * @brief Ukazatel na strukturu kontejneru s funkcemi ktere byli definovany uvnitr skripty
	 */
	cvector_s *blocks;

	/**
	 * @var cvector_s *vars
	 * @brief Ukazatel na strukturu kontejneru s promennymi ktere byli definovany ve skriptu
	 */
	cvector_s *vars;

	/**
	 * @var buffer_s *buffer
	 * @brief Ukazatel na strukturu zasobniku
	 */
	filebuffer_s *buffer;

	/**
	 * @var cc_env_s *env
	 * @brief Promenne 'prostredi'.
	 */
	cc_env_s *env;

} cc_parser_s;

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
///
///
///
///
///
