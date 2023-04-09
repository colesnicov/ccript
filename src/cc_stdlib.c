/*
 * This file is a part of "CCRipt" Embeddable Custom 'C' Like Scripting Language Interpreter
 */

/**
 * @file cc_stdlib.c
 * @brief Implementace  standardnich 'C' funkci pro volani ze skript.
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

#include "ccript/cc_configs.h"
#include "ccript/cc_parser.h"
#include "ccript/cc_stdlib.h"
#include "ccript/cc_types.h"
#include "ccript/cc_var_ext.h"
#include "ccript/ccript.h"
#include "ccript/common.h"
#include "ccript/cc_var.h"

#include <stdint.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

bool cc_stdlib_registrate(cc_parser_s *_parser, void *_args)
{
	bool ret = true;
	ret &= cc_registerFunction(_parser, "dump", 4, stdlib_dump, _args);
	ret &= cc_registerFunction(_parser, "exit", 5, stdlib_exit, _args);
	ret &= cc_registerFunction(_parser, "cast", 4, stdlib_cast, _args);
	ret &= cc_registerFunction(_parser, "print", 5, stdlib_print, _args);
	ret &= cc_registerFunction(_parser, "println", 7, stdlib_println, _args);
//	ret &= cc_registerFunction(_parser, "system", 6, stdlib_system, _args);
	ret &= cc_registerFunction(_parser, "strlen", 6, stdlib_strlen, _args);
	ret &= cc_registerFunction(_parser, "strcat", 6, stdlib_strcat, _args);
	ret &= cc_registerFunction(_parser, "env", 3, stdlib_env, _args);

	return ret;
}

var_s* stdlib_DebugInfo(cc_parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args)
{

	if (_vars_count == 0)
	{
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		return NULL;
	}

	if (_vars[0]->type != CC_TYPE_STRING)
	{
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		return NULL;
	}

	size_t len = strlen(_vars[0]->data);

	if (len == 0)
	{
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		return false;

	}

	var_s *var = VarCreate("@", 1, CC_TYPE_INT, _parser->depth);

	if (!var)
	{
		return NULL;
	}

	if (len == 12 && strncmp(_vars[0]->data, "parser.depth", len) == 0)
	{

		if (!VarValueSetInt(_parser, var, _parser->depth))
		{
			VarDestroy(var);
			return NULL;
		}

	}

	else if (len == 9 && strncmp(_vars[0]->data, "var.depth", len) == 0)
	{

		if (_vars_count < 2)
		{

			parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);

			VarDestroy(var);
			return NULL;
		}

		if (!VarValueSetInt(_parser, var, _vars[1]->scope))
		{

			VarDestroy(var);
			return NULL;
		}

	}

	else if (len == 8 && strncmp(_vars[0]->data, "var.size", len) == 0)
	{

		if (_vars_count != 2)
		{
			parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);

			VarDestroy(var);
			return NULL;
		}

		int siz = VarGetSize(_vars[1]);

		if (!VarValueSetInt(_parser, var, siz))
		{
			VarDestroy(var);
			return NULL;
		}

	}

	else if (len == 11 && strncmp(_vars[0]->data, "var.sizeAll", len) == 0)
	{

		if (_vars_count > 1)
		{
			parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);

			VarDestroy(var);
			return NULL;
		}

		int siz = VarGetSizeAll(_parser);

		if (!VarValueSetInt(_parser, var, siz))
		{
			VarDestroy(var);
			return NULL;
		}

	}

	else
	{
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);

		VarDestroy(var);
		return NULL;
	}

	return var;
}

var_s* stdlib_exit(cc_parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args)
{
	_parser->error = CC_CODE_ABORT;
	return NULL;
}

var_s* stdlib_system(cc_parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args)
{

	if (_vars_count == 0)
	{
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		return NULL;
	}

	if (_vars[0]->type != CC_TYPE_STRING)
	{
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		return NULL;
	}

	size_t len = strlen(_vars[0]->data);

	if (len == 0)
	{
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		return false;

	}

	else if (len == 5 && strncmp(_vars[0]->data, "abort", len) == 0)
	{

		_parser->error = CC_CODE_ABORT;
		return NULL;
	}

	else if (len == 4 && strncmp(_vars[0]->data, "exit", len) == 0)
	{

		_parser->error = CC_CODE_ABORT;
		return NULL;
	}

	parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
	parseSetErrorPos(_parser, parseGetPos(_parser));

	return NULL;
}

var_s* stdlib_env(cc_parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args)
{

	if (_vars_count != 1)
	{
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		return NULL;
	}

	char str[CC_KEYWORD_LEN + 1] = {
			'\0' };
	size_t len = CC_KEYWORD_LEN;

	if (!VarValueGetString(_parser, _vars[0], str, &len))
	{
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		return NULL;
	}

	for (uint8_t i = 0; i < _parser->env_count; i++)
	{

		if (len != strlen(_parser->env[i].name) || strncmp(str, _parser->env[i].name, len) != 0)
		{
			continue;
		}

		cc_env_s env = _parser->env[i];
//		char buf[20] = {'\0' };

		var_s *var = VarCreate(str, len, env.type, _parser->depth);

		if (_parser->env[i].type == CC_TYPE_INT)
		{
			int *y = (int*) (env.data);
			if (!VarValueSetInt(_parser, var, *y))
			{
				VarDestroy(var);
				return NULL;
			}
		}

		else if (env.type == CC_TYPE_BOOL)
		{

			if (!VarValueSetBool(_parser, var, (bool) (*(bool*) (env).data)))
			{
				VarDestroy(var);
				return NULL;
			}
		}

		else if (env.type == CC_TYPE_LONG)
		{

			if (!VarValueSetLong(_parser, var, (long) (*(long*) (env).data)))
			{
				VarDestroy(var);
				return NULL;
			}
		}

		else if (env.type == CC_TYPE_FLOAT)
		{
			if (!VarValueSetFloat(_parser, var, (float) (*(float*) (env).data)))
			{
				VarDestroy(var);
				return NULL;
			}
		}

		else if (env.type == CC_TYPE_CHAR)
		{

			if (!VarValueSetChar(_parser, var, (char) *(char*) (env).data))
			{
				VarDestroy(var);
				return NULL;
			}
		}

		else if (env.type == CC_TYPE_STRING)
		{

			if (!VarValueSetString(_parser, var, (char*) (env).data, strlen((env).data)))
			{
				VarDestroy(var);
				return NULL;
			}
		}

		return var;
	}

	return NULL;
}

var_s* stdlib_abs(cc_parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args)
{

	if (_vars_count != 1)
	{
		parseSetError(_parser, CC_CODE_LOGIC);
		CC_PRINT("ERROR: wrong number of arguments!\n");
		return NULL;
	}

	var_s *var = NULL;

	if (_vars[0]->type == CC_TYPE_LONG)
	{
		long tt = 0;
		var = VarCreate("@", 1, CC_TYPE_LONG, _parser->depth);

		if (var == NULL)
		{
			return NULL;
		}

		if (!VarValueGetLong(_parser, _vars[0], &tt))
		{
			return NULL;
		}

		if (!VarValueSetLong(_parser, var, labs(tt)))
		{
			VarDestroy(var);
			return NULL;
		}
	}

	else if (_vars[0]->type == CC_TYPE_INT)
	{
		int tt = 0;
		var = VarCreate("@", 1, CC_TYPE_INT, _parser->depth);

		if (var == NULL)
		{
			return NULL;
		}

		if (!VarValueGetInt(_parser, _vars[0], &tt))
		{
			return NULL;
		}

		if (!VarValueSetInt(_parser, var, abs(tt)))
		{
			VarDestroy(var);
			return NULL;
		}

	}

	else if (_vars[0]->type == CC_TYPE_FLOAT)
	{
		float tt = 0;
		var = VarCreate("@", 1, CC_TYPE_FLOAT, _parser->depth);

		if (var == NULL)
		{
			return NULL;
		}

		if (!VarValueGetFloat(_parser, _vars[0], &tt))
		{
			return NULL;
		}

		if (!VarValueSetFloat(_parser, var, fabsf(tt)))
		{
			VarDestroy(var);
			return NULL;
		}

	}

	else
	{
		CC_PRINT("ERROR: wrong type of argument '%d'!\n", _vars[0]->type);
		parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
	}

	return var;
}

var_s* stdlib_cast(cc_parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args)
{

	if (_vars_count == 0)
	{
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		return NULL;
	}

	if (_vars[0]->type != CC_TYPE_STRING)
	{
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		return NULL;
	}

	size_t len = strlen(_vars[0]->data);

	if (len == 0)
	{
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		return false;

	}

	var_s *var = NULL;

	if (len == 3 && strncmp(_vars[0]->data, "int", len) == 0)
	{
		var = VarCastToInt(_parser, _vars[1]);
	}

	if (len == 4 && strncmp(_vars[0]->data, "long", len) == 0)
	{
		var = VarCastToLong(_parser, _vars[1]);
	}

	else if (len == 5 && strncmp(_vars[0]->data, "float", len) == 0)
	{
		var = VarCastToFloat(_parser, _vars[1]);
	}

	else if (len == 4 && strncmp(_vars[0]->data, "char", len) == 0)
	{
		var = VarCastToChar(_parser, _vars[1]);
	}

	else if (len == 6 && strncmp(_vars[0]->data, "string", len) == 0)
	{
		var = VarCastToString(_parser, _vars[1]);
	}

	else if (len == 4 && strncmp(_vars[0]->data, "bool", len) == 0)
	{
		var = VarCastToBool(_parser, _vars[1]);
	}

	return var;
}

var_s* stdlib_strlen(cc_parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args)
{

	if (_vars_count == 0)
	{
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		return NULL;
	}

	if (_vars[0]->type != CC_TYPE_STRING)
	{
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		return NULL;
	}

	size_t len = strlen(_vars[0]->data);

	if (len == 0)
	{
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		return false;

	}

	var_s *var = VarCreate("_", 1, CC_TYPE_INT, _parser->depth);

	if (var == NULL)
	{
		return NULL;
	}

	if (!VarValueSetInt(_parser, var, len))
	{
		VarDestroy(var);
		return NULL;
	}

	return var;
}

var_s* stdlib_strcat(cc_parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args)
{

	if (_vars_count < 2)
	{
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		return NULL;
	}

	size_t len_total = 0;
	char buf[CC_VALUE_STRING_LEN + 1] = {
			'\0' };
	char buf2[CC_VALUE_STRING_LEN + 1] = {
			'\0' };

	size_t len = 0;

	for (uint8_t i = 0; i < _vars_count; i++)
	{

		memset(buf2, '\0', sizeof(char) * CC_VALUE_STRING_LEN);

		if (!_vars[i]->valid)
		{
			parseSetError(_parser, CC_CODE_VAR_NOT_ASSIGNED);
			return NULL;
		}

		if (_vars[i]->type == CC_TYPE_BOOL)
		{
			if (*(bool*) (_vars[i]->data))
			{
				buf2[0] = '1';
			}
			else
			{
				buf2[0] = '0';
			}
			len = 1;
		}
		else if (_vars[i]->type == CC_TYPE_CHAR)
		{
			len = 1;
			buf2[0] = *(char*) _vars[i]->data;
		}
		else if (_vars[i]->type == CC_TYPE_FLOAT)
		{
			len = (size_t) snprintf(buf2, CC_VALUE_STRING_LEN, "%.*f",
			CC_FLOAT_EXP_LEN, *(float*) (_vars[i]->data));
		}
		else if (_vars[i]->type == CC_TYPE_INT)
		{
			len = (size_t) snprintf(buf2, CC_VALUE_STRING_LEN, "%d", *(int*) (_vars[i]->data));
		}
		else if (_vars[i]->type == CC_TYPE_STRING)
		{
			len = strlen(_vars[i]->data);
			memcpy(buf2, _vars[i]->data, len);
		}
		else if (_vars[i]->type == CC_TYPE_ARRAY)
		{
			parseSetError(_parser, CC_CODE_LOGIC);
			return NULL;

		}

		if (len_total + len > CC_VALUE_STRING_LEN)
		{
			return NULL;
		}

		memcpy(buf + len_total, buf2, len);
		len_total += len;

	}

	var_s *var = VarCreate("@", 1, CC_TYPE_STRING, _parser->depth);

	if (var == NULL)
	{
		return NULL;
	}

	if (!VarValueSetString(_parser, var, buf, len_total))
	{
		VarDestroy(var);
		return NULL;
	}

	return var;

}

var_s* stdlib_print(cc_parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args)
{

	if (_vars_count != 1)
	{
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		return NULL;
	}

	char buf[CC_VALUE_STRING_LEN + 1] = {
			'\0' };

	if (_vars[0]->type == CC_TYPE_BOOL)
	{
		snprintf(buf, CC_VALUE_STRING_LEN, "%d", *(bool*) _vars[0]->data);
	}

	else if (_vars[0]->type == CC_TYPE_CHAR)
	{
		snprintf(buf, CC_VALUE_STRING_LEN, "%c", *(char*) _vars[0]->data);
	}

	else if (_vars[0]->type == CC_TYPE_FLOAT)
	{
		snprintf(buf, CC_VALUE_STRING_LEN, "%.*f", CC_FLOAT_EXP_LEN, *(float*) _vars[0]->data);
	}

	else if (_vars[0]->type == CC_TYPE_INT)
	{
		snprintf(buf, CC_VALUE_STRING_LEN, "%d", *(int*) _vars[0]->data);
	}

	else if (_vars[0]->type == CC_TYPE_STRING)
	{
		snprintf(buf, CC_VALUE_STRING_LEN, "%s", (char*) _vars[0]->data);
	}

	CC_PRINT("%s", buf);

	return NULL;
}

var_s* stdlib_println(cc_parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args)
{

	if (_vars_count != 1)
	{
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		return NULL;
	}

	char buf[CC_VALUE_STRING_LEN + 1] = {
			'\0' };

	if (_vars[0]->type == CC_TYPE_BOOL)
	{
		snprintf(buf, CC_VALUE_STRING_LEN, "%d", *(bool*) _vars[0]->data);
	}

	else if (_vars[0]->type == CC_TYPE_CHAR)
	{
		snprintf(buf, CC_VALUE_STRING_LEN, "%c", *(char*) _vars[0]->data);
	}

	else if (_vars[0]->type == CC_TYPE_FLOAT)
	{
		snprintf(buf, CC_VALUE_STRING_LEN, "%.*f", CC_FLOAT_EXP_LEN, *(float*) _vars[0]->data);
	}

	else if (_vars[0]->type == CC_TYPE_INT)
	{
		snprintf(buf, CC_VALUE_STRING_LEN, "%d", *(int*) _vars[0]->data);
	}

	else if (_vars[0]->type == CC_TYPE_STRING)
	{
		snprintf(buf, CC_VALUE_STRING_LEN, "%s", (char*) _vars[0]->data);
	}

	CC_PRINT("%s\n", buf);

	return NULL;
}

var_s* stdlib_dump(cc_parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args)
{

	if (_vars_count != 1)
	{
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		return NULL;
	}

	VarDump(_vars[0]);

	return NULL;
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
