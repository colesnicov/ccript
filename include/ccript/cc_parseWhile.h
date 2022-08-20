/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file cc_parseWhile.h
 * @brief Definice funkci pro praci se smyckou WHILE.
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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @fn bool parseWhile(parser_s*)
 * @brief Zpracuje smycku WHILE.
 *
 * @param _parser Ukazatel na parser.
 *
 * @return FALSE kdyz dojde k problemum, jinak TRUE.
 */
var_s* parseWhile(parser_s *_parser);

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
