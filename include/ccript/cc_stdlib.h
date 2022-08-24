/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file cc_stdlib.h
 * @brief Definice standardnich 'C' funkci pro volani ze skript.
 * @details Tyto funkce mohou byt 'zaveseny' na parser a posleze pristupne ze skriptu.
 * @see cc_registerFunction()
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

#include <stdint.h>
#include "ccript/cc_types.h"

#ifdef __cplusplus
extern "C" {
#endif

bool cc_stdlib_registrate(parser_s *_parser, void *_args);

var_s* stdlib_sleep(parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args);

var_s* stdlib_millis(parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args);

var_s* stdlib_dump(parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args);

var_s* stdlib_print(parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args);

var_s* stdlib_println(parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args);

var_s* stdlib_strcat(parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args);

var_s* stdlib_strlen(parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args);

var_s* stdlib_cast(parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args);

var_s* stdlib_DebugInfo(parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args);

var_s* stdlib_system(parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args);

var_s* stdlib_exit(parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args);

var_s* stdlib_env(parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args);

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
