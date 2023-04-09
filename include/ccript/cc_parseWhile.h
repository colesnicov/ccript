/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file cc_parseWhile.h
 * @brief Definice funkci pro praci se smyckou WHILE.
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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @fn bool parseWhile(cc_parser_s*)
 * @brief Zpracuje smycku WHILE.
 *
 * @param _parser Ukazatel na parser.
 *
 * @return FALSE kdyz dojde k problemum, jinak TRUE.
 */
var_s* parseWhile(cc_parser_s *_parser);

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
