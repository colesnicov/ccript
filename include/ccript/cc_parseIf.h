/*
 * This file is a part of "CCRipt" Embeddable Custom 'C' Like Scripting Language Interpreter
 */

/**
 * @file cc_parseIf.h
 * @brief Definice funkci pro praci s podminkou IF.
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

#include "ccript/cc_types.h"
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @fn bool parseIf(cc_parser_s*)
 * @brief Zpracuje BLOK IF
 *
 * @param _parser Ukazatel na parser
 *
 * @return TRUE jestli bez problemu, jinak FALSE
 * @details Funkce muze selhat z vice duvodu:
 * 			 - vyraz 'BREAK', 'CONTINUE', 'RETURN'
 * 			 - jina nezpracovatelna chyba
 */
var_s* parseIf(cc_parser_s *_parser);

/**
 * @fn bool parseIfArguments(cc_parser_s*, float*, char)
 * @brief Zpracuje podminku IF a vrati vysledek..
 *
 * @param _parser Ukazatel na parser.
 * @param _cond_value Sem se ulozi vysledek vypoctu.
 * @param _end_sym Ukoncovaci znacka.
 *
 * @return TRUE jestli bez problemu, jinak FALSE.
 */
bool parseIfArguments(cc_parser_s *_parser, float *_cond_value, char _end_sym);

/**
 * @fn bool parseIfArg(cc_parser_s*, float*)
 * @brief Zpracuje jeden argument a vrati vysledek jako cislo s plovouci desetinou carkou.
 *
 * @param _parser Ukazatel na parser.
 * @param _arg Sem se ulozi vysledek vypoctu.
 *
 * @return TRUE jestli bez problemu, jinak FALSE.
 */
bool parseIfArg(cc_parser_s *_parser, float *_arg);

/**
 * @fn bool parseIfCond(cc_parser_s*, float*)
 * @brief Zpracuje podminkovy PAR. Par je toto: `a` nebo `a == b`.
 *
 * @param _parser Ukazatel na parser.
 * @param _cond_value Sem se ulozi vysledek vypoctu.
 *
 * @return TRUE jestli bez problemu, jinak FALSE.
 */

bool parseIfCond(cc_parser_s *_parser, float *_cond_value);

/**
 * @fn void garbageIf(var_s**, uint8_t*)
 * @brief Odstrani promenne a pamet po zpracovani podminky IF
 *
 * @details UNUSED?
 *
 * @param args Pole promennych.
 * @param args_count Pocet, kolik je promennych v poli.
 */
void garbageIf(var_s **args, uint8_t *args_count);

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
