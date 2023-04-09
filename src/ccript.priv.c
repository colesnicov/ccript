/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file ccript.priv.c
 * @brief Implementace dodatecnych privatnich funkci pro praci s interpreterem.
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

#include <ccript/cc_parser.h>
#include <ccript/cc_var_ext.h>
#include "ccript/cc_types.h"
#include "ccript/cc_var.h"
#include "cvector/cvector.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

void freeVar(void *item)
{
	VarDestroy((var_s*) item);
}

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

