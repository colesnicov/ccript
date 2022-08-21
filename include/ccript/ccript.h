/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file ccript.h
 * @brief Definice verejnych funkci pro praci s interpreterem.
 *
 * @version 1b0
 * @date 26.06.2022
 *
 * @author Denis Colesnicov <eugustus@gmail.com>
 *
 * @copyright MIT License
 *
 */

/**
 * @todo toto float1 = float2 na konci chybi strednik ';', vraci spatnou pozici chyby
 * @todo toto float1 = float2+10 na konci chybi strednik ';', vraci spatnou pozici chyby
 * @todo toto int int2 = 22 na konci chybi strednik ';', vraci spatnou chybu
 */

#pragma once

#include "ccript/cc_types.h"
#include <stddef.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @fn bool cc_init(parser_s*, const char*)
 * @brief Provede inicializaci interpretoru.
 *
 * @param _parser Ukazatel na parser.
 *
 * @return FALSE pri nedostatku pameti, jinak TRUE.
 */
bool cc_init(parser_s *_parser);

/**
 * @fn void cc_deinit(parser_s*)
 * @brief Odstrani promenne, uvolni pamet.
 *
 * @param _parser Ukazatel na parser.
 */
void cc_deinit(parser_s *_parser);

/**
 * @fn bool cc_registerFunction(parser_s*, const char*, size_t, cc_fn_prototype)
 * @brief Zaregistruje novou fuknci.
 *
 * @param _parser Ukazatel na parser.
 * @param _name Nazev funkce.
 * @param _name_len Delka nazvu funkce.
 * @param _fn Ukazatel na fn_handler_s.
 * @param _args Ukazatel na externi argument pro funkci
 *
 * @return FALSE jestli neni dost pameti nebo funkce jiz byla registrovana. Jinak TRUE
 */
bool cc_registerFunction(parser_s *_parser, const char *_name, size_t _name_len,
		cc_fn_prototype _fn, void *_args);

/**
 * @fn bool cc_parse(parser_s*, const char*)
 * @brief Zpracuje script.
 *
 * @param _parser Ukazatel na parser.
 * @param _path Cesta k souboru.
 *
 * @details	doplnit
 *
 * @return FALSE jestli dojde k chybe, jinak TRUE.
 */
var_s* cc_parse(parser_s *_parser, const char *_path);

/**
 * @fn void cc_abort(parser_s*)
 * @brief Zastavi vykonani skriptu.
 *
 * @param _parser Ukazatel na parser.
 *
 */
void cc_abort(parser_s *_parser);

/**
 * @fn bool cc_errorHas(parser_s*)
 * @brief Doslo k chybe?
 *
 * @param _parser Ukazatel na parser.
 *
 * @return TRUE jestli doslo k chybe, jinak FALSE.
 */
bool cc_errorHas(parser_s *_parser);

/**
 * @fn cc_code_t cc_errorGetCode(parser_s*)
 * @brief Vrati stavovy kod chyby.
 *
 * @param _parser Ukazatel na parser.
 *
 * @return Stavovy kod chyby.
 *
 * @see cc_code_t
 */
cc_code_t cc_errorGetCode(parser_s *_parser);

/**
 * @fn size_t cc_errorGetPos(parser_s*)
 * @brief Vrati pozici v souboru na ktere doslo k chybe.
 *
 * @param _parser Ukazatel na parser.
 *
 * @return Pozice v souboru.
 */
size_t cc_errorGetPos(parser_s *_parser);

/**
 * @fn const char* cc_errorToString(cc_code_t)
 * @brief Vrati textovy nazev kodu chyby.
 *
 * @param _err_code Kod chyby.
 *
 * @return Textovy nazev chyby.
 */
const char* cc_errorToString(cc_code_t _err_code);

#ifdef __cplusplus
}
#endif

