/*
 * This file is a part of "CCRipt" Embeddable Custom 'C' Like Scripting Language Interpreter
 */

/**
 * @file stdlib_impl.c
 * @brief Implementace dodatecnich standardnich 'C' funkci pro pouziti ve skriptech.
 *
 * @version 1b0
 * @date 26.06.2022
 *
 * @author Denis Colesnicov <eugustus@gmail.com>
 *
 * @copyright MIT License
 *
 */

#include "stdlib_impl.h"

#include "ccript/cc_configs.h"
#include "ccript/cc_parser.h"
#include "ccript/cc_stdlib.h"
#include "ccript/cc_types.h"
#include "ccript/cc_var_ext.h"
#include "ccript/common.h"
#include "ccript/cc_var.h"

#include <stdint.h>
#include <float.h>
#include <time.h>
#include <unistd.h>

static int64_t millis()
{
	struct timespec now;
	timespec_get(&now, TIME_UTC);
	return ((int64_t) now.tv_sec) * 1000 + ((int64_t) now.tv_nsec) / 1000000;
}

var_s* stdlib_millis(cc_parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args)
{
	var_s *var = VarCreate("@", 1, CC_TYPE_LONG, _parser->depth);

	if (var == NULL)
	{
		return NULL;
	}

	if (!VarValueSetLong(_parser, var, millis()))
	{
		VarDestroy(var);
		return NULL;
	}

	return var;
}

static void sleep_us(long microseconds)
{
	struct timespec ts;
	ts.tv_sec = microseconds / 1000000l;
	ts.tv_nsec = (microseconds % 1000000l) * 1000;
	nanosleep(&ts, NULL);
}

var_s* stdlib_sleep(cc_parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args)
{

	long t = 0;

	if (_vars_count != 1)
	{
		parseSetError(_parser, CC_CODE_LOGIC);
		CC_PRINT("ERROR: wrong number of arguments!\n");
		return NULL;
	}

	else if (_vars[0]->type == CC_TYPE_LONG)
	{
		if (!VarValueGetLong(_parser, _vars[0], &t))
		{
			return NULL;
		}
	}

	else if (_vars[0]->type == CC_TYPE_INT)
	{
		int tt= 0;
		if (!VarValueGetInt(_parser, _vars[0], &tt))
		{
			return NULL;
		}
		t = tt;
	}

	else
	{
		CC_PRINT("ERROR: wrong type of argument '%d'!\n", _vars[0]->type);
		parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
		return NULL;
	}


	sleep_us(t * 1000);

	return NULL;
}

