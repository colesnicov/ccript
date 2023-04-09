/*
 * This file is a part of "CCRipt" Embeddable Custom 'C' Like Scripting Language Interpreter
 */

/**
 * @file cc_stdlib.h
 * @brief Definice standardnich 'C' funkci pro volani ze skript.
 * @details Tyto funkce mohou byt 'zaveseny' na parser a posleze pristupne ze skriptu.
 * @see cc_registerFunction()
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

#include <stdint.h>
#include "ccript/cc_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @fn bool cc_stdlib_registrate(cc_parser_s*, void*)
 * @brief Registruje vsechny 'standardni' funkce.
 *
 * @param _parser Ukazatel na parser.
 * @param _args Ukazatel na uzivatelska data.
 * @details Uzivatelska data budou pristupna v kontextu volane funkce.
 *
 * @return TRUE nebo FALSE pri selhani.
 *
 * @deprecated Odstranit! Programator si vse musi registrovat sam!?
 */
bool cc_stdlib_registrate(cc_parser_s *_parser, void *_args);

/**
 * @fn var_s* stdlib_dump(cc_parser_s*, var_s**, uint8_t, void*)
 * @brief Dumpne informace o promenne.
 *
 * @param _parser Ukazatel na parser.
 * @param _vars Pole ukazatelu na predane argumenty.
 * @param _vars_count Pocet predanych argumenut
 * @param _args Ukazatel na uzivatelska data.
 * @details Uzivatelska data budou pristupna v kontextu volane funkce.
 * @note Vracena promena musi byt odstranena!
 * @see VarDestroy
 *
 * @return Ukazatel na vracenou promennou.
 */
var_s* stdlib_dump(cc_parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args);

/**
 * @fn var_s* stdlib_print(cc_parser_s*, var_s**, uint8_t, void*)
 * @brief Vypise neco do STD.
 *
 * @param _parser Ukazatel na parser.
 * @param _vars Pole ukazatelu na predane argumenty.
 * @param _vars_count Pocet predanych argumenut
 * @param _args Ukazatel na uzivatelska data.
 * @details Uzivatelska data budou pristupna v kontextu volane funkce.
 * @note Vracena promena musi byt odstranena!
 * @see VarDestroy
 *
 * @return Ukazatel na vracenou promennou.
 */
var_s* stdlib_print(cc_parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args);

/**
 * @fn var_s* stdlib_println(cc_parser_s*, var_s**, uint8_t, void*)
 * @brief Vypise neco do STD a ukonci to znakem noveho radku: '\n'.
 *
 * @param _parser Ukazatel na parser.
 * @param _vars Pole ukazatelu na predane argumenty.
 * @param _vars_count Pocet predanych argumenut
 * @param _args Ukazatel na uzivatelska data.
 * @details Uzivatelska data budou pristupna v kontextu volane funkce.
 * @note Vracena promena musi byt odstranena!
 * @see VarDestroy
 * @see CC_PRINT
 *
 * @return Ukazatel na vracenou promennou.
 */
var_s* stdlib_println(cc_parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args);

/**
 * @fn var_s* stdlib_strcat(cc_parser_s*, var_s**, uint8_t, void*)
 * @brief Spoji nekolik retezcu do jednoho.
 *
 * @param _parser Ukazatel na parser.
 * @param _vars Pole ukazatelu na predane argumenty.
 * @param _vars_count Pocet predanych argumenut
 * @param _args Ukazatel na uzivatelska data.
 * @details Uzivatelska data budou pristupna v kontextu volane funkce.
 * @note Vracena promena musi byt odstranena!
 * @see VarDestroy
 * @see CC_FUNC_NUMS_ARGS
 *
 * @return Ukazatel na vracenou promennou.
 */
var_s* stdlib_strcat(cc_parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args);

/**
 * @fn var_s* stdlib_strlen(cc_parser_s*, var_s**, uint8_t, void*)
 * @brief Vrati delku retezce.
 *
 * @param _parser Ukazatel na parser.
 * @param _vars Pole ukazatelu na predane argumenty.
 * @param _vars_count Pocet predanych argumenut
 * @param _args Ukazatel na uzivatelska data.
 * @details Uzivatelska data budou pristupna v kontextu volane funkce.
 * @note Vracena promena musi byt odstranena!
 * @see VarDestroy
 *
 * @return Ukazatel na vracenou promennou.
 */
var_s* stdlib_strlen(cc_parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args);

/**
 * @fn var_s* stdlib_cast(cc_parser_s*, var_s**, uint8_t, void*)
 * @brief Konvertuje promennou na jiny datovy typ.
 *
 * @param _parser Ukazatel na parser.
 * @param _vars Pole ukazatelu na predane argumenty.
 * @param _vars_count Pocet predanych argumenut
 * @param _args Ukazatel na uzivatelska data.
 * @details Uzivatelska data budou pristupna v kontextu volane funkce.
 * @note Vracena promena musi byt odstranena!
 * @see VarDestroy
 *
 * @return Ukazatel na vracenou promennou.
 */
var_s* stdlib_cast(cc_parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args);

/**
 * @fn var_s* stdlib_DebugInfo(cc_parser_s*, var_s**, uint8_t, void*)
 * @brief Vrati informace o na pozadani.
 * @note duplicita s `stdlib_system`!?
 * @note Pouzival jsem ji behem vyvoje. Jinak potrebna neni.
 *
 * @param _parser Ukazatel na parser.
 * @param _vars Pole ukazatelu na predane argumenty.
 * @param _vars_count Pocet predanych argumenut
 * @param _args Ukazatel na uzivatelska data.
 * @details Uzivatelska data budou pristupna v kontextu volane funkce.
 * @note Vracena promena musi byt odstranena!
 * @see VarDestroy
 *
 * @return Ukazatel na vracenou promennou.
 */
var_s* stdlib_DebugInfo(cc_parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args);

/**
 * @fn var_s* stdlib_system(cc_parser_s*, var_s**, uint8_t, void*)
 * @brief Zavola systemovy prikaz ktery nebyl definovan pomoci `cc_registerFunction`
 * @note Pouzival jsem ji behem vyvoje. Jinak potrebna neni.
 *
 * @param _parser Ukazatel na parser.
 * @param _vars Pole ukazatelu na predane argumenty.
 * @param _vars_count Pocet predanych argumenut
 * @param _args Ukazatel na uzivatelska data.
 * @details Uzivatelska data budou pristupna v kontextu volane funkce.
 * @note Vracena promena musi byt odstranena!
 * @see VarDestroy
 * @see cc_registerFunction
 *
 * @return Ukazatel na vracenou promennou.
 */
var_s* stdlib_system(cc_parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args);

/**
 * @fn var_s* stdlib_exit(cc_parser_s*, var_s**, uint8_t, void*)
 * @brief Ukonci beh parseru.
 *
 * @param _parser Ukazatel na parser.
 * @param _vars Pole ukazatelu na predane argumenty.
 * @param _vars_count Pocet predanych argumenut
 * @param _args Ukazatel na uzivatelska data.
 * @details Uzivatelska data budou pristupna v kontextu volane funkce.
 * @note Vracena promena musi byt odstranena!
 * @see VarDestroy
 *
 * @return Ukazatel na vracenou promennou.
 */
var_s* stdlib_exit(cc_parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args);

/**
 * @fn var_s* stdlib_env(cc_parser_s*, var_s**, uint8_t, void*)
 * @brief Ziska promennou 'prosterdi'.
 *
 * @param _parser Ukazatel na parser.
 * @param _vars Pole ukazatelu na predane argumenty.
 * @param _vars_count Pocet predanych argumenut
 * @param _args Ukazatel na uzivatelska data.
 * @details Uzivatelska data budou pristupna v kontextu volane funkce.
 * @note Vracena promena musi byt odstranena!
 * @see VarDestroy
 *
 * @return Ukazatel na vracenou promennou.
 */
var_s* stdlib_env(cc_parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args);


/**
 * @fn var_s* stdlib_abs(cc_parser_s*, var_s**, uint8_t, void*)
 * @brief Z negativniho cisla udela pozitivni.
 *
 * @param _parser Ukazatel na parser.
 * @param _vars Pole ukazatelu na predane argumenty.
 * @param _vars_count Pocet predanych argumenut
 * @param _args Ukazatel na uzivatelska data.
 * @details Uzivatelska data budou pristupna v kontextu volane funkce.
 * @note Vracena promena musi byt odstranena!
 * @see VarDestroy
 *
 * @return Ukazatel na vracenou promennou.
 */
var_s* stdlib_abs(cc_parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args);


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
