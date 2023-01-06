/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file cc_configs.h
 * @brief Uzivatelske nastaveni.
 *
 * @version 1b1
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

#include <freertos/FreeRTOS.h>
#include <freertos/private/portable.h>

#include <system/mathio.hpp>

#if 0
#define S1(x) #x
#define S2(x) S1(x)
#define CC_LOG_TAG __FILE__ " : " S2(__LINE__)
#else

#define	CC_LOG_TAG	__FUNCTION__

#endif

#define CONFIG_CC_MALLOC(s)		pvPortMalloc((s))
#define CONFIG_CC_REALLOC(p, s)	pvPortRealloc((p),(s))
#define CONFIG_CC_FREE(p)		vPortFree((p))

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
 * @fixme Pri spatnem poctu predanych funkci parametru dochazi k padu! Vyresit!!
 */
#define CONFIG_CC_FUNC_NUMS_ARGS		5

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
#define CONFIG_CC_PRINT(...)	ESP_LOGI(CC_LOG_TAG, __VA_ARGS__)

/**
 * @def CONFIG_CC_PRINT_COMMENT
 * @brief Vypisovat komentare do konzole?
 * @see CONFIG_CC_PRINT
 *
 */
#define CONFIG_CC_PRINT_COMMENT	0

/**
 * @def CONFIG_CC_FUNC_DEBUG
 * @brief Vypisovat ladici informace o volanich funkci ve skriptech?
 *
 */
#define CONFIG_CC_FUNC_DEBUG	0

#define CONFIG_CC_VAR_DEBUG	0

/**
 * @def CONFIG_CC_KEYWORD_LEN
 * @brief Velikost zasobniku pro vyraz.
 * @details Tolik mista bude rezervovano v pameti RAM pro zasobnik pro vyraz.
 *
 *
 */
#define CONFIG_CC_KEYWORD_LEN	16

#define CONFIG_CC_NUMERIC_LEN	51

/**
 * @def CONFIG_CC_STRING_LEN
 * @brief Velikost zasobniku pro retezec.
 * @details Tolik mista bude rezervovano v pameti RAM pro zasobnik pro retezec.
 *
 */
#define CONFIG_CC_STRING_LEN	51

/**
 * @def CONFIG_CC_COMMENT_LEN
 * @brief Velikost zasobniku pro komentar.
 * @details Tolik mista bude rezervovano v pameti RAM pro zasobnik komentare.
 * @see CONFIG_CC_PRINT_COMMENT
 *
 */
#define CONFIG_CC_COMMENT_LEN	150

/**
 * @def CONFIG_CC_BUFFER_LEN
 * @brief Velikost zasobniku pro skript.
 * @details Tolik mista bude rezervovano v pameti RAM pro zasobnik skriptu nacteneho ze souboru.
 *
 */
#define CONFIG_CC_BUFFER_LEN		5

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

#include "ccript/cc_log.h"
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
#define CONFIG_CC_FUNC_NUMS_ARGS		4

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
#define CONFIG_CC_PRINT(...)	printf("%s: ", CC_LOG_TAG); \
								printf(__VA_ARGS__)

/**
 * @def CONFIG_CC_PRINT_COMMENT
 * @brief Vypisovat komentare do konzole?
 * @see CONFIG_CC_PRINT
 *
 */
#define CONFIG_CC_PRINT_COMMENT	0

/**
 * @def CONFIG_CC_FUNC_DEBUG
 * @brief Vypisovat ladici informace o volanich funkci ve skriptech?
 *
 */
#define CONFIG_CC_FUNC_DEBUG	0

#define CONFIG_CC_VAR_DEBUG		0

#define CONFIG_CC_BLOCK_DEBUG	0

/**
 * @def CONFIG_CC_KEYWORD_LEN
 * @brief Velikost zasobniku pro vyraz vcetne ukoncovaci '\0'. nazvy promennych, funkci, ...
 * @details Tolik mista bude rezervovano v pameti RAM pro zasobnik pro vyraz.
 * @details Vysledny vyraz bude o 1 znak kratsi!
 *
 *
 */
#define CONFIG_CC_KEYWORD_LEN	16

#define CONFIG_CC_NUMERIC_LEN	51

/**
 * @def CONFIG_CC_STRING_LEN
 * @brief Velikost zasobniku pro retezec.
 * @details Tolik mista bude rezervovano v pameti RAM pro zasobnik pro retezec.
 *
 */
#define CONFIG_CC_STRING_LEN	51

/**
 * @def CONFIG_CC_COMMENT_LEN
 * @brief Velikost zasobniku pro komentar.
 * @details Tolik mista bude rezervovano v pameti RAM pro zasobnik komentare.
 * @see CONFIG_CC_PRINT_COMMENT
 *
 */
#define CONFIG_CC_COMMENT_LEN	150

/**
 * @def CONFIG_CC_BUFFER_LEN
 * @brief Velikost zasobniku pro skript.
 * @details Tolik mista bude rezervovano v pameti RAM pro zasobnik skriptu nacteneho ze souboru.
 *
 */
#define CONFIG_CC_BUFFER_LEN		5

#endif
