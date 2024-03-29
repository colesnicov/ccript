/*
 * This file is a part of "CCRipt" Embeddable Custom 'C' Like Scripting Language Interpreter
 */

/**
 * @file ccript.h
 * @brief Definice verejnych funkci pro praci s interpreterem.
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
#include <stddef.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @fn bool cc_init(cc_parser_s*)
 * @brief Provede inicializaci interpretoru.
 *
 * @param _parser Ukazatel na parser.
 *
 * @return FALSE pri nedostatku pameti, jinak TRUE.
 */
bool cc_init(cc_parser_s *_parser);

/**
 * @fn void cc_deinit(cc_parser_s*)
 * @brief Odstrani promenne, uvolni pamet.
 *
 * @param _parser Ukazatel na parser.
 */
void cc_deinit(cc_parser_s *_parser);

/**
 * @fn bool cc_registerFunction(cc_parser_s*, const char*, size_t, cc_fn_prototype, void*)
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
bool cc_registerFunction(cc_parser_s *_parser, const char *_name, size_t _name_len,
		cc_fn_prototype _fn, void *_args);

/**
 * @fn bool cc_parse(cc_parser_s*, const char*, cc_env_s*, uint8_t)
 * @brief Zpracuje script.
 *
 * @param _parser Ukazatel na parser.
 * @param _path Cesta k souboru.
 * @param _env Pole promennych 'prostredi'.
 * @param _env_count Pocet promennych prostredi.
 *
 * @details	doplnit
 *
 * @return FALSE jestli dojde k chybe, jinak TRUE.
 */
var_s* cc_parse(cc_parser_s *_parser, const char *_path, cc_env_s *_env, uint8_t _env_count);

/**
 * @fn void cc_abort(cc_parser_s*)
 * @brief Zastavi vykonani skriptu.
 *
 * @param _parser Ukazatel na parser.
 *
 */
void cc_abort(cc_parser_s *_parser);

/**
 * @fn bool cc_errorHas(cc_parser_s*)
 * @brief Doslo k chybe?
 *
 * @param _parser Ukazatel na parser.
 *
 * @return TRUE jestli doslo k chybe, jinak FALSE.
 */
bool cc_errorHas(cc_parser_s *_parser);

/**
 * @fn cc_code_t cc_errorGetCode(cc_parser_s*)
 * @brief Vrati stavovy kod chyby.
 *
 * @param _parser Ukazatel na parser.
 *
 * @return Stavovy kod chyby.
 *
 * @see cc_code_t
 */
cc_code_t cc_errorGetCode(cc_parser_s *_parser);

/**
 * @fn size_t cc_errorGetPos(cc_parser_s*)
 * @brief Vrati pozici v souboru na ktere doslo k chybe.
 *
 * @param _parser Ukazatel na parser.
 *
 * @return Pozice v souboru.
 */
size_t cc_errorGetPos(cc_parser_s *_parser);

/**
 * @fn const char* cc_errorToString(cc_code_t)
 * @brief Vrati textovy nazev kodu chyby.
 *
 * @param _err_code Kod chyby.
 *
 * @return Textovy nazev chyby.
 */
const char* cc_errorToString(cc_code_t _err_code);

/**
 * @fn void cc_varDestroy(var_s*)
 * @brief Odstrani promennou.
 * @details Toto je potreba kdyz skripta vraci nejakou hodnotu
 *
 * @param _var Ukazatel na promennou
 */
void cc_varDestroy(var_s *_var);
/**
 * @fn cc_code_t cc_varGetBool(var_s*, bool*)
 * @brief Ziska boolean z promenne vracene skriptou.
 *
 * @param _var Ukazatel na promennou.
 * @param _val Sem bude zapsana hodnota
 *
 * @return CC_CODE_OK nebo jiny v pripade chyby
 */
cc_code_t cc_varGetBool(var_s *_var, bool *_val);

/**
 * @fn cc_code_t cc_varGetInt(var_s*, int*)
 * @brief Ziska 'cislo' z promenne vracene skriptou.
 *
 * @param _var Ukazatel na promennou.
 * @param _val Sem bude zapsana hodnota
 *
 * @return CC_CODE_OK nebo jiny v pripade chyby
 *
 */
cc_code_t cc_varGetInt(var_s *_var, int *_val);

/**
 * @fn cc_code_t cc_varGetLong(var_s*, long*)
 * @brief Ziska 'dlouhe cislo' z promenne vracene skriptou.
 *
 * @param _var Ukazatel na promennou.
 * @param _val Sem bude zapsana hodnota
 *
 * @return CC_CODE_OK nebo jiny v pripade chyby
 *
 */
cc_code_t cc_varGetLong(var_s *_var, long *_val);

/**
 * @fn cc_code_t cc_varGetFloat(var_s*, float*)
 * @brief Ziska 'cislo s desetinou carkou' z promenne vracene skriptou.
 *
 * @param _var Ukazatel na promennou.
 * @param _val Sem bude zapsana hodnota
 *
 * @return CC_CODE_OK nebo jiny v pripade chyby
 *
 */
cc_code_t cc_varGetFloat(var_s *_var, float *_val);

/**
 * @fn cc_code_t cc_varGetChar(var_s*, char*)
 * @brief Ziska 'znak' z promenne vracene skriptou.
 *
 * @param _var Ukazatel na promennou.
 * @param _val Sem bude zapsana hodnota
 *
 * @return CC_CODE_OK nebo jiny v pripade chyby
 *
 */
cc_code_t cc_varGetChar(var_s *_var, char*);

/**
 * @fn cc_code_t cc_varGetString(var_s*, char*, size_t*)
 * @brief Ziska 'retezec' z promenne vracene skriptou.
 *
 * @param _var Ukazatel na promennou.
 * @param _val Sem bude zapsan retezec.
 * @param _len Sem bude delka retezce.
 *
 * @return CC_CODE_OK nebo jiny v pripade chyby
 *
 */
cc_code_t cc_varGetString(var_s *_var, char *_val, size_t *_len);

#ifdef __cplusplus
}
#endif

