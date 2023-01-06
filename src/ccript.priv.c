/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file ccript.priv.c
 * @brief Implementace dodatecnych privatnich funkci pro praci s interpreterem.
 *
 * @version 1b1
 * @date 26.06.2022
 *
 * @author Denis Colesnicov <eugustus@gmail.com>
 *
 * @copyright MIT License
 *
 */

#include <ccript/cc_parser.h>
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

void parseClearScope(parser_s *_parser)
{
	var_s *var = NULL;
	uint8_t i = 0;

	// tady WHILE proto ze kontejner se nevaliduje?
	while (true)
	{

		if (((cvector_s*) _parser->vars)->total == 0)
		{
			break;
		}

		if (i >= ((cvector_s*) _parser->vars)->total)
		{
			break;
		}

		var = cvector_get((cvector_s*) (_parser->vars), (int) i);
		if (!var)
		{
			CC_PRINT("DEBUG: variable with index '%d' of '%u' is 'null'", i,
					(unsigned )((cvector_s* ) _parser->vars)->total);
			cvector_delete((cvector_s*) _parser->vars, i);

			continue;
		}

		if (_parser->depth < var->scope)
		{

//			CC_PRINT("debug: destroy scoped variable '%s'!\n\n", var->name);

			cvector_delete_dealloc((cvector_s*) _parser->vars, i, freeVar);

			continue;
		}

		i++;

	}
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

