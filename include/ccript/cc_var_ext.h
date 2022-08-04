/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file cc_var_ext.h
 * @brief Definice dodatecnych funkci pro praci s promennymi.
 *
 * @version 1b0
 * @date 26.06.2022
 *
 * @author Denis Colesnicov <eugustus@gmail.com>
 *
 * @copyright MIT License
 *
 */

#include <stddef.h>

#include <stdbool.h>

#include "ccript/common.h"
#include "ccript/cc_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @fn void freeVar(void*)
 * @brief Universalni funkce ktera se muze pouzit pro odstraneni promenne.
 *
 * @param item Ukazatel na promennou jako 'void*'
 */
void freeVar(void *item);

/**
 * @fn void VarDump(var_s*)
 * @brief Vypise informace o promenne.
 * @details Vypisuje nazev promenne, jestli je inicializovana, jeji rozsah platnosti a prirazena data.
 *
 * @see CC_PRINT()
 *
 * @param _var Ukazatel na promennou.
 */
void VarDump(var_s *_var);

/**
 * @fn void VarDumpAll(parser_s*)
 * @brief Vypise informace o vsech promennych co jsou momentalne definovany.
 *
 * @param _parser Ukazatel na parser.
 */
void VarDumpAll(parser_s *_parser);

/**
 * @fn const char* VarTypeToString(cc_type_t)
 * @brief Vrati textovou podobu typu promenne.
 *
 * @param _type Typ promenne.
 *
 * @return Textova verze cisla kodu.
 */
const char* VarTypeToString(cc_type_t _type);

/**
 * @fn size_t VarGetSize(var_s*)
 * @brief Vrati kolik promenna zabira mista v pameti.
 *
 * @param _var Ukazatel na promennou.
 *
 * @return Pocet bytu, kolik si vyzaduje promenna.
 */
size_t VarGetSize(var_s *_var);

/**
 * @fn size_t VarGetSizeAll(parser_s*)
 * @brief Vrati kolik mista zabiraji vsechny promenne.
 *
 * @note Do tohoto poctu jsou zapoctene pouze promenne z parser_s.vars
 *
 * fixme Potreba funkci ktera bude umet spocist vsechny promenne, i ty co jsou jako argumenty...
 *
 * @param _parser Ukazatel na promennou.
 *
 * @return Pocet bytu, kolik si vyzadali vsechny promenne.
 */
size_t VarGetSizeAll(parser_s *_parser);

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
