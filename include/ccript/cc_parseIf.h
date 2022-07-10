/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file cc_parseIf.h
 * @brief Definice funkci pro praci s podminkou IF.
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

#include "ccript/cc_types.h"
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @fn bool parseIf(parser_s*)
 * @brief Zpracuje BLOK IF
 *
 * @param _parser Ukazatel na parser
 *
 * @return TRUE jestli bez problemu, jinak FALSE
 * @details Funkce muze selhat z vice duvodu:
 * 			 - vyraz 'BREAK', 'CONTINUE', 'RETURN'
 * 			 - jina nezpracovatelna chyba
 */
bool parseIf(parser_s *_parser);

/**
 * @fn bool parseIfArguments(parser_s*, bool*)
 * @brief Zpracuje podminku IF a vrati vysledek..
 *
 * @param _parser Ukazatel na parser.
 * @param _cond_passed Sem se ulozi vysledek vypoctu.
 *
 * @return TRUE jestli bez problemu, jinak FALSE.
 */
bool parseIfArguments(parser_s *_parser, bool *_cond_passed);

/**
 * @fn bool parseIfArg(parser_s*, float*)
 * @brief Zpracuje jeden argument a vrati vysledek jako cislo s plovouci desetinou carkou.
 *
 * @param _parser Ukazatel na parser.
 * @param _cond_arg Sem se ulozi vysledek vypoctu.
 *
 * @return TRUE jestli bez problemu, jinak FALSE.
 */
bool parseIfArg(parser_s *_parser, float *arg);

/**
 * @fn bool parseIfPair(parser_s*, bool*)
 * @brief Zpracuje podminkovy PAR. Par je toto: a == b.
 *
 * @param _parser Ukazatel na parser.
 * @param _cond_passed Sem se ulozi vysledek vypoctu.
 *
 * @return TRUE jestli bez problemu, jinak FALSE.
 */
bool parseIfPair(parser_s *_parser, bool *_cond_passed);

/**
 * @fn void cleanIf(var_s**, uint8_t*)
 * @brief Odstrani promenne a pamet po zpracovani podminky IF
 *
 * @details UNUSED?
 *
 * @param args Pole promennych.
 * @param args_count Pocet, kolik je promennych v poli.
 */
void cleanIf(var_s **args, uint8_t *args_count);

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
