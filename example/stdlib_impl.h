/*
 * This file is a part of "CCRipt" Embeddable Custom 'C' Like Scripting Language Interpreter
 */
/**
 * @file stdlib_impl.h
 * @brief Definice dodatecnich standardnich 'C' funkci pro pouziti ve skriptech.
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

#include <stdint.h>
#include <ccript/cc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

var_s* stdlib_millis(cc_parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args);

var_s* stdlib_sleep(cc_parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args);

#ifdef __cplusplus
}
#endif

