/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file cc_buffer.h
 * @brief Definice dynamickeho pristupu k obsahu souboru.
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

#include <stdbool.h>

#include "ccript/cc_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @def bufferValid(parser_s*)
 * @brief Overi jestli v zasobniku jsou data, pripadne zajisti jejich doplneni.
 *
 * @see bufferFill()
 *
 * @param _parser Ukazatel na parser.
 *
 * @return TRUE, jestli uz nejsou dalsi data vrati FALSE.
 */
#define bufferValid(_parser) bufferFill(_parser)

/**
 * @fn bool bufferReload(parser_s*)
 * @brief Znova nacte zasobnik ze souboru.
 *
 * @note V pripade selhani nastavuje navratovy kod. @see cc_setError()
 *
 * @param _parser Ukazatel na parser.
 *
 * @return TRUE nebo FALSE kdyz dojde k problemu s praci se souborem.
 */
bool bufferReload(parser_s *_parser);

/**
 * @fn bool bufferFill(parser_s*)
 * @brief Nacte zasobnik ze souboru jestli je potreba.
 *
 * @note V pripade selhani nastavuje navratovy kod. @see bufferReload()
 *
 * @param _parser Ukazatel na parser.
 *
 * @return  TRUE nebo FALSE kdyz dojde k problemu s praci se souborem.
 *
 */
bool bufferFill(parser_s *_parser);

/**
 * @fn bool bufferInit(parser_s*, const char*, size_t)
 * @brief Inicializuje parser.
 *
 * @details Otevre soubor, rezervuje pamet, vynuluje indexy...
 *
 * @note V pripade selhani nastavuje navratovy kod. @see cc_setError()
 *
 * @param _parser Ukazatel na parser.
 * @param _path Cesta k souboru se skriptem.
 * @param _buffer_size Velikost zasobniku pro data.
 *
 * @return  TRUE nebo FALSE kdyz dojde k problemum s praci se souborem nebo nedostatkem pameti.
 *
 * @see bufferReload()
 *
 * @note Kdyz dojde k selhani, dojde k automatickemu uvolneni prostredku. spatne?
 */
bool bufferInit(parser_s *_parser, const char *_path, size_t _buffer_size);

/**
 * @fn void bufferClose(parser_s*)
 * @brief Uvolni prostredky po parseru.
 *
 * @param _parser Ukazatel na parser.
 */
void bufferClose(parser_s *_parser);

/**
 * @fn bool bufferGet(parser_s*, char*)
 * @brief Vrati jeden znak ze zasobniku.
 *
 * @note V pripade selhani nastavuje navratovy kod. @see cc_setError()
 *
 * @param[in] _parser Ukazatel na parser.
 * @param[out] _ch ukazatel na znak do ktereho se to vlozi.
 *
 * @return TRUE, jestli neni mozne ziskat znak tak vrati FALSE.
 */
bool bufferGet(parser_s *_parser, char *_ch);

/**
 * @fn bool bufferNext(parser_s*)
 * @brief Prejde na dalsi znak v zasobniku, pripadne ho doplni.
 * @see bufferFill()
 *
 * @note V pripade selhani nastavuje navratovy kod. @see cc_setError()
 *
 * @param _parser Ukazatel na parser.
 *
 * @return TRUE, FALSE jestli operace selze.
 */
bool bufferNext(parser_s *_parser);

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
