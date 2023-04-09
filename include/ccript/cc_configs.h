/*
 * This file is a part of "CCRipt" Embeddable Custom 'C' Like Scripting Language Interpreter
 */

/**
 * @file cc_configs.h
 * @brief Uzivatelske nastaveni.
 * @since 26.06.2022
 *
 * @version 1r1
 * @date 08.04.2023
 *
 *
 * @author Denis Colesnicov <eugustus@gmail.com>
 *
 * @copyright MIT License
 *
 */

#pragma once

#if 1

#define __SF(x) #x
#define __SS(x) __SF(x)
#define CC_LOG_TAG __FILE__ " : " __SS(__LINE__)

#else

#define	CC_LOG_TAG	__FUNCTION__

#endif

#ifdef ESP_PLATFORM

///////////////////////////
//
// Tady vlozit hlavicky funkci ktere jsou pouzity v definicich maker.
//
///////////////////////////

#include <sdkconfig.h>
#include <esp_log.h>
#include <stdio.h>
#include <assert.h>
#include <malloc.h>

#include <freertos/FreeRTOS.h>
#include <freertos/private/portable.h>

/**
 * @def CONFIG_CC_MALLOC
 * @brief Rezervuje pamet.
 *
 */
#define CONFIG_CC_MALLOC		pvPortMalloc

/**
 * @def CONFIG_CC_REALLOC
 * @brief Rezervuje pamet.
 * @brief
 *
 */
#define CONFIG_CC_REALLOC	pvPortRealloc

/**
 * @def CONFIG_CC_FREE
 * @brief Uvolni pamet.
 *
 */
#define CONFIG_CC_FREE		vPortFree

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


#else

///////////////////////////
//
// Tady vlozit hlavicky funkci ktere jsou pouzity v definecich maker.
//
///////////////////////////

#include <stdio.h>
#include <assert.h>
#include <malloc.h>

/**
 * @def CONFIG_CC_MALLOC
 * @brief Rezervuje pamet.
 *
 */
#define CONFIG_CC_MALLOC(s)		malloc((s))

/**
 * @def CONFIG_CC_REALLOC
 * @brief Prerezervuje pamet.
 *
 */
#define CONFIG_CC_REALLOC(p, s)	realloc((p),(s))

/**
 * @def CONFIG_CC_FREE
 * @brief Uvolni pamet.
 *
 */
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
#define CONFIG_CC_FUNC_NUMS_ARGS		8

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
 * @def CONFIG_CC_PRINT_ENV
 * @brief Vypsat promenne prostredi do konzole?
 * @see CONFIG_CC_PRINT
 *
 */
#define CONFIG_CC_PRINT_ENV		1

/**
 * @def CONFIG_CC_FUNC_DEBUG
 * @brief Vypisovat ladici informace o volanich funkci ve skriptech?
 *
 */
#define CONFIG_CC_FUNC_DEBUG	0

/**
 * @def CONFIG_CC_VAR_DEBUG
 * @brief  Vypisovat ladici informace o volanich promennych ve skriptech?
 *
 */
#define CONFIG_CC_VAR_DEBUG		0

/**
 * @def CONFIG_CC_BLOCK_DEBUG
 * @brief  Vypisovat ladici informace o volanich zanorovani bloku ve skriptech?
 *
 */
#define CONFIG_CC_BLOCK_DEBUG	0

/**
 * @def CONFIG_CC_KEYWORD_LEN
 * @brief Velikost zasobniku pro vyraz vcetne ukoncovaci '\0'. nazvy promennych, funkci, ...
 * @details Tolik mista bude rezervovano v pameti RAM pro zasobnik pro vyraz.
 * @details Vysledny vyraz bude o 1 znak kratsi!
 *
 */
#define CONFIG_CC_KEYWORD_LEN	16

/**
 * @def CONFIG_CC_VALUE_LONG_LEN
 * @brief Velikost zasobniku pro cislo vcetne ukoncovaci '\0'. int, long a float.
 * @details Tolik mista bude rezervovano v pameti RAM.
 * @details Vysledny vyraz bude o 1 znak kratsi!
 *
 */
#define CONFIG_CC_VALUE_LONG_LEN	9

/**
 * @def CONFIG_CC_NUMERIC_LEN
 * @brief  Velikost zasobniku pro cislo.
 * @details Tolik mista bude rezervovano v pameti RAM pro zasobnik pro cislo.
 *
 */
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
#define CONFIG_CC_BUFFER_LEN		50

/**
 * @def CONFIG_CC_IF_COND_MATH
 * @brief Podpora matematickych operaci v podminkach.
 * @details Napriklad: `if(1 + 2 == 3)`
 *
 */
#define CONFIG_CC_IF_COND_MATH		1

/**
 * @def CONFIG_CC_ASSIGN_EMPTY
 * @brief Podpora prirazeni prazdneho redezce/znaku.
 *
 */
#define CONFIG_CC_ASSIGN_EMPTY		1

#endif
