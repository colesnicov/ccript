/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file cc_configs.h
 * @brief Uzivatelske nastaveni.
 *
 * @version 1b0
 * @date 26.06.2022
 *
 * @author Denis Colesnicov <eugustus@gmail.com>
 *
 * @copyright MIT License
 *
 */

#pragma once

#ifdef ESP_PLATFORM

///////////////////////////
//
// Tady vlozit hlavicky funkci ktere jsou pouzity v definicich maker.
//
///////////////////////////

#include <esp_log.h>
#include <stdio.h>
#include <assert.h>
#include <malloc.h>

#include <system/mathio.hpp>

#define	LOG_TAG	(char*)"CC"

#define CONFIG_CC_MALLOC(s)		malloc((s))
#define CONFIG_CC_REALLOC(p, s)	realloc((p),(s))
#define CONFIG_CC_FREE(p)		free((p))

/**
 * @def CONFIG_CC_INCLUDE_FILEIO
 * @brief INCLUDE s definici funkci pro praci se soubory `struct FILE`, `fopen()`, atd.
 * @details Napriklad <stdio.h>
 *
 */
#define CONFIG_CC_INCLUDE_FILEIO	<stdio.h>

/**
 * @def CONFIG_CC_INCLUDE_SNPRINTF
 * @brief INCLUDE s definici funkce `snprintf`.
 * @details Napriklad <stdio.h>
 *
 */
#define CONFIG_CC_INCLUDE_SNPRINTF	<stdio.h>

/**
 * @def CONFIG_CC_FLOAT_EXP_LEN
 * @brief Delka exponentu cisla s plovouci desetinnou carkou.
 * @details Pouziva se pri prevodu promenne typu `float` na typ `string`
 * 			<code><pre>
 * 			12.345;
 * 			   ^^^
 * 			</pre></code>
 *
 */
#define CONFIG_CC_FLOAT_EXP_LEN		3

/**
 * @def CONFIG_CC_FUNC_NUMS_ARGS
 * @brief Maximalni pocet argumentu ktery funkce muze prijmout.
 *
 */
#define CONFIG_CC_FUNC_NUMS_ARGS		10

/**
 * @def CONFIG_CC_ASSERT
 * @brief ASSERT funkce
 *
 */
#define CONFIG_CC_ASSERT(expr)	assert(expr)

/**
 * @def CONFIG_CC_PRINT
 * @brief Funkce pro vypis do konzole.
 *
 */
//#define CONFIG_CC_PRINT(...)	printf(__VA_ARGS__)
#define CONFIG_CC_PRINT(...)	ESP_LOGI(LOG_TAG, __VA_ARGS__)

/**
 * @def CONFIG_CC_PRINT_COMMENT
 * @brief Vypisovat komentare do konzole?
 * @see CONFIG_CC_PRINT
 *
 */
#define CONFIG_CC_PRINT_COMMENT	1

/**
 * @def CONFIG_CC_FUNC_DEBUG
 * @brief Vypisovat ladici informace o volanich funkci ve skriptech?
 *
 */
#define CONFIG_CC_FUNC_DEBUG	0

/**
 * @def CONFIG_CC_KEYWORD_SIZE_CAPS
 * @brief Velikost zasobniku pro vyraz.
 * @details Tolik mista bude rezervovano v pameti RAM pro zasobnik pro vyraz.
 *
 *
 */
#define CONFIG_CC_KEYWORD_SIZE_CAPS	16

/**
 * @def CONFIG_CC_STRING_SIZE_CAPS
 * @brief Velikost zasobniku pro retezec.
 * @details Tolik mista bude rezervovano v pameti RAM pro zasobnik pro retezec.
 *
 */
#define CONFIG_CC_STRING_SIZE_CAPS	51

/**
 * @def CONFIG_CC_COMMENT_SIZE_CAPS
 * @brief Velikost zasobniku pro komentar.
 * @details Tolik mista bude rezervovano v pameti RAM pro zasobnik komentare.
 * @see CONFIG_CC_PRINT_COMMENT
 *
 */
#define CONFIG_CC_COMMENT_SIZE_CAPS	150

/**
 * @def CONFIG_CC_BUFFER_SIZE_CAPS
 * @brief Velikost zasobniku pro skript.
 * @details Tolik mista bude rezervovano v pameti RAM pro zasobnik skriptu nacteneho ze souboru.
 *
 */
#define CONFIG_CC_BUFFER_SIZE_CAPS		5

#else
/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file cc_configs.h
 * @brief Uzivatelske nastaveni.
 *
 * @version 1
 * @date 26.06.2022
 *
 * @author Denis Colesnicov <eugustus@gmail.com>
 *
 * @copyright MIT License
 *
 */

#pragma once

///////////////////////////
//
// Tady vlozit hlavicky funkci ktere jsou pouzity v definecich maker.
//
///////////////////////////

#include <stdio.h>
#include <assert.h>
#include <malloc.h>

#define CONFIG_CC_MALLOC(s)		malloc((s))
#define CONFIG_CC_REALLOC(p, s)	realloc((p),(s))
#define CONFIG_CC_FREE(p)		free((p))

/**
 * @def CONFIG_CC_INCLUDE_FILEIO
 * @brief INCLUDE s definici funkci pro praci se soubory `struct FILE`, `fopen()`, atd.
 * @details Napriklad <stdio.h>
 *
 */
#define CONFIG_CC_INCLUDE_FILEIO	<stdio.h>

/**
 * @def CONFIG_CC_INCLUDE_SNPRINTF
 * @brief INCLUDE s definici funkce `snprintf`.
 * @details Napriklad <stdio.h>
 *
 */
#define CONFIG_CC_INCLUDE_SNPRINTF	<stdio.h>

/**
 * @def CONFIG_CC_FLOAT_EXP_LEN
 * @brief Delka exponentu cisla s plovouci desetinnou carkou.
 * @details Pouziva se pri prevodu promenne typu `float` na typ `string`
 * 			<code><pre>
 * 			12.345;
 * 			   ^^^
 * 			</pre></code>
 *
 */
#define CONFIG_CC_FLOAT_EXP_LEN		3

/**
 * @def CONFIG_CC_FUNC_NUMS_ARGS
 * @brief Maximalni pocet argumentu ktery funkce muze prijmout.
 *
 */
#define CONFIG_CC_FUNC_NUMS_ARGS		10

/**
 * @def CONFIG_CC_ASSERT
 * @brief ASSERT funkce
 *
 */
#define CONFIG_CC_ASSERT(expr)	assert(expr)

/**
 * @def CONFIG_CC_PRINT
 * @brief Funkce pro vypis do konzole.
 *
 */
#define CONFIG_CC_PRINT(...)	printf(__VA_ARGS__)

/**
 * @def CONFIG_CC_PRINT_COMMENT
 * @brief Vypisovat komentare do konzole?
 * @see CONFIG_CC_PRINT
 *
 */
#define CONFIG_CC_PRINT_COMMENT	1

/**
 * @def CONFIG_CC_FUNC_DEBUG
 * @brief Vypisovat ladici informace o volanich funkci ve skriptech?
 *
 */
#define CONFIG_CC_FUNC_DEBUG	0

/**
 * @def CONFIG_CC_KEYWORD_SIZE_CAPS
 * @brief Velikost zasobniku pro vyraz.
 * @details Tolik mista bude rezervovano v pameti RAM pro zasobnik pro vyraz.
 *
 *
 */
#define CONFIG_CC_KEYWORD_SIZE_CAPS	16

/**
 * @def CONFIG_CC_STRING_SIZE_CAPS
 * @brief Velikost zasobniku pro retezec.
 * @details Tolik mista bude rezervovano v pameti RAM pro zasobnik pro retezec.
 *
 */
#define CONFIG_CC_STRING_SIZE_CAPS	51

/**
 * @def CONFIG_CC_COMMENT_SIZE_CAPS
 * @brief Velikost zasobniku pro komentar.
 * @details Tolik mista bude rezervovano v pameti RAM pro zasobnik komentare.
 * @see CONFIG_CC_PRINT_COMMENT
 *
 */
#define CONFIG_CC_COMMENT_SIZE_CAPS	150

/**
 * @def CONFIG_CC_BUFFER_SIZE_CAPS
 * @brief Velikost zasobniku pro skript.
 * @details Tolik mista bude rezervovano v pameti RAM pro zasobnik skriptu nacteneho ze souboru.
 *
 */
#define CONFIG_CC_BUFFER_SIZE_CAPS		5

#endif
