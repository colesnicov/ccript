/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file cc_function.h
 * @brief Definice funkci pro praci s funkcemi
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

#include <stddef.h>
#include <stdint.h>

#include "ccript/cc_types.h"
#include "ccript/common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @fn bool funcInit(parser_s*)
 * @brief Inicializuje kontejner pro funkce.
 *
 * @param _parser Ukazatel na parser.
 *
 * @return TRUE, FALSE pri nedostatku pameti.
 */
bool funcInit(parser_s *_parser);

/**
 * @fn void funcDeinit(parser_s*)
 * @brief Deinicializuje kontejner pro funkce.
 *
 * @param _parser Ukazatel na parser.
 */
void funcDeinit(parser_s *_parser);

/**
 * @fn fn_handler_s* funcGet(parser_s*, const char*, size_t)
 * @brief Vrati ukazatel na strukturu udrzujici
 *
 * @param _parser Ukazatel na parser.
 * @param _name Nazev funkce
 * @param _name_len Delka nazvu funkce
 *
 * @return Ukazatel na fn_handler_s nebo NULL jestli nenalezeno
 */
fn_handler_s* funcGet(parser_s *_parser, const char *_name, size_t _name_len);


/**
 * @fn void funcClearArguments(var_s**, uint8_t)
 * @brief Odstrani argumenty po volani funkce.
 * @details Volase az po parseFuncArguments().
 * @see parseFuncArguments()
 *
 * @param args Pole ukazatelu na var_s
 * @param args_count Pocet promennych v poli
 */
void funcClearArguments(var_s **args, uint8_t args_count);

/**
 * @fn var_s* funcCall(parser_s*, const char*, size_t)
 * @brief Zavola funkci.
 * @details Parsuje argumenty funkce a po jejim zavolani uvolni prostredky a vrati vysledek
 *
 * @param _parser Ukazatel na parser.
 * @param func_name Nazev funkce
 * @param func_name_len Delka nazvu funkce
 *
 * @return Ukazatel na var_s nebo NULL.
 */
var_s* funcCall(parser_s *_parser, const char *func_name, size_t func_name_len);

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
