/*
 * This file is a part of "CCRipt" Embeddable Custom 'C' Like Scripting Language Interpreter
 */

/**
 * @file cc_function.c
 * @brief Implementace funkci pro praci s funkcemi.
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

#include "ccript/cc_block.h"
#include <emblib/emblib.h>
#include "ccript/cc_function.h"
#include "ccript/cc_parser.h"
#include "ccript/cc_types.h"
#include "ccript/cc_var.h"
#include "ccript/ccript.h"
#include "ccript/common.h"
#include "cvector/cvector.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bool funcInit(cc_parser_s *_parser)
{
	CC_ASSERT(_parser->funcs == NULL && "CONTAINER FOR FUNCTIONS ALERADY INITIALIZED");

	cvector_s *funcs = (cvector_s*) CC_MALLOC(sizeof(cvector_s));
	if (funcs == NULL)
	{

		parseSetError(_parser, CC_CODE_NOT_MEM);
		CC_FUNC_DEBUG("ERROR: not enough memmory for functions!\n");
		_parser->funcs = NULL;

		return false;
	}

	funcs->items = NULL;

	_parser->funcs = funcs;

	if (!cvector_init(_parser->funcs))
	{
		parseSetError(_parser, CC_CODE_NOT_MEM);
		CC_FUNC_DEBUG("ERROR: not enough memmory for functions!");

		CC_FREE(_parser->funcs);
		_parser->funcs = NULL;

		return false;
	}

	return true;
}

void funcDeinit(cc_parser_s *_parser)
{

	CC_ASSERT(_parser != NULL && "PARSER IS NULL!");

	fn_handler_s *handler;

	if (_parser->funcs == NULL)
	{
		return;
	}

	for (uint8_t i = 0; i < cvector_total(_parser->funcs); i++)
	{
		handler = cvector_get(_parser->funcs, i);

		if (handler != NULL)
		{
			CC_FUNC_DEBUG("DEBUG: remove function '%s'.\n", handler->name);
			CC_FREE(handler);
			handler = NULL;
		}

	}

	cvector_deinit(_parser->funcs);
	CC_FREE(_parser->funcs);
	_parser->funcs = NULL;

}

var_s* funcCall(cc_parser_s *_parser, const char *func_name, size_t func_name_len)
{

	var_s *var = NULL;

	cc_block_s *block = blockGet(_parser, func_name, func_name_len);
	if (block != NULL)
	{
		var = blockCall(_parser, block);

		if (cc_errorGetCode(_parser) == CC_CODE_RETURN)
		{
			parseSetError(_parser, CC_CODE_OK);
		}

		return var;
	}
	else
	{

		fn_handler_s *handler = funcGet(_parser, func_name, func_name_len);

		if (handler == NULL)
		{
			parseSetError(_parser, CC_CODE_FUNC_NOT_DEFINED);
			CC_FUNC_DEBUG("ERROR: function '%s' not defined.\n", func_name);
			return NULL;
		}

		var_s *args[CC_FUNC_NUMS_ARGS] = {
				NULL };
		uint8_t args_count = 0;

		file_bufferNext(_parser->buffer);
		file_bufferSkipSpace(_parser->buffer);

		if (!parseFuncArguments(_parser, (var_s**) args, &args_count))
		{
			CC_FUNC_DEBUG("ERROR: func '%s' arguments error.\n", func_name);
			garbageFuncArguments(args, args_count);

			return NULL;
		}

		var = handler->func(_parser, args, args_count, handler->args);

		garbageFuncArguments(args, args_count);

		return var;
	}

}

fn_handler_s* funcGet(cc_parser_s *_parser, const char *_name, size_t _name_len)
{

	fn_handler_s *handler = NULL;

	for (uint8_t i = 0; i < cvector_total(_parser->funcs); i++)
	{
		handler = cvector_get(_parser->funcs, i);
		if (_name_len == strlen(handler->name) && strncmp(_name, handler->name, _name_len) == 0)
		{
			return handler;
		}
	}

	return NULL;
}

bool cc_registerFunction(cc_parser_s *_parser, const char *_name, size_t _name_len,
		cc_fn_prototype _fn, void *_args)
{
	fn_handler_s *handler;

	for (uint8_t i = 0; i < cvector_total(_parser->funcs); i++)
	{
		handler = cvector_get(_parser->funcs, i);

		if (_name_len == strlen(handler->name) && strncmp(_name, handler->name, _name_len) == 0)
		{
			CC_FUNC_DEBUG("ERROR: duplicated function name '%s'!\n", _name);
			parseSetError(_parser, CC_CODE_FUNC_EXISTS);
			return false;
		}
	}

	fn_handler_s *hn = (fn_handler_s*) CC_MALLOC(sizeof(fn_handler_s));
	if (hn == NULL)
	{
		parseSetError(_parser, CC_CODE_NOT_MEM);
		CC_FUNC_DEBUG("ERROR: not enough memmory for function '%s'\n", _name);
		return false;
	}

	hn->name = (char*) _name;
	hn->func = _fn;
	hn->args = _args;
	return cvector_add(_parser->funcs, hn);

}

bool parseFuncArguments(cc_parser_s *_parser, var_s **_args, uint8_t *_args_count)
{
	char ch;

	size_t value_len = 0;
	while (FILEBUFFER_OK == file_bufferValid(_parser->buffer))
	{

		file_bufferSkipSpace(_parser->buffer);

		file_bufferGet(_parser->buffer, &ch);

		if (ch == ',')
		{
			if (CC_FUNC_NUMS_ARGS <= *_args_count)
			{
				return false;
			}
			// dalsi argument

			file_bufferNext(_parser->buffer);

			continue;

		}

		else if (ch == '\'')
		{
			// pismeno/znak/symbol

			char value = 0;

			if (!ParseValueChar(_parser, &value, &value_len))
			{
				return false;
			}

			char buf[3] = {
					'\0' };
			size_t len = 1;

			if (*_args_count == 0)
			{
				memcpy(buf, "0", sizeof(char));
			}

			else
			{
				len = (size_t) itoa_n(*_args_count, buf, 10);
			}

			var_s *_var = VarCreate(buf, len, CC_TYPE_CHAR, _parser->depth);
			if (_var == NULL)
			{
				return false;
			}

			if (!VarValueSetChar(_parser, _var, value))
			{
				VarDestroy(_var);
				return false;
			}

			_args[*_args_count] = _var;
			*_args_count = *_args_count + 1;

			file_bufferNext(_parser->buffer);
			continue;
		}

		else if (ch == '"')
		{
			// retezec

			char value[CC_VALUE_STRING_LEN + 1] = {
					'\0' };

			if (!ParseValueString(_parser, value, &value_len))
			{
				return false;
			}

			char buf[3] = {
					'\0' };
			size_t len = 1;

			if (*_args_count == 0)
			{
				memcpy(buf, "0", sizeof(char));
			}

			else
			{
				len = (size_t) itoa_n(*_args_count, buf, 10);
			}

			var_s *_var = VarCreate(buf, len, CC_TYPE_STRING, _parser->depth);
			if (_var == NULL)
			{
				return false;
			}

			if (!VarValueSetString(_parser, _var, value, value_len))
			{
				VarDestroy(_var);
				return false;
			}

			_args[*_args_count] = _var;
			*_args_count += 1;

			continue;
		}

		else if (isdigit(ch) || ch == '-')
		{
			// cislo

			char value[CC_VALUE_NUMERIC_LEN] = {
					'\0' };

			bool is_float = false;
			if (!parseValueFloat(_parser, value, &value_len, &is_float))
			{
				return false;
			}

			if (value_len == 0)
			{
				parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
				CC_FUNC_DEBUG("ERROR: empty int/float value!\n");
				return false;

			}

			var_s *_var = NULL;

			if (is_float)
			{

				char buf[3] = {
						'\0' };
				size_t len = 1;

				if (*_args_count == 0)
				{
					memcpy(buf, "0", sizeof(char));
				}

				else
				{
					len = (size_t) itoa_n(*_args_count, buf, 10);
				}

				_var = VarCreate(buf, len, CC_TYPE_FLOAT, _parser->depth);

				if (_var == NULL)
				{
					return false;
				}

				if (!VarValueSetFloat(_parser, _var, atof(value)))
				{
					VarDestroy(_var);
					return false;
				}

			}
			else
			{

				char buf[3] = {
						'\0' };
				size_t len = 1;

				if (*_args_count == 0)
				{
					memcpy(buf, "0", sizeof(char));
				}

				else
				{
					len = (size_t) itoa_n(*_args_count, buf, 10);
				}

				_var = VarCreate(buf, len, CC_TYPE_INT, _parser->depth);

				if (_var == NULL)
				{
					return false;
				}

				if (!VarValueSetInt(_parser, _var, atoi(value)))
				{
					VarDestroy(_var);
					return false;
				}

			}

			_args[*_args_count] = _var;
			*_args_count += 1;

			continue;
		}

		else if (isalpha(ch))
		{

			char var_name[CC_KEYWORD_LEN] = {
					'\0' };
			size_t var_name_len = 0;

			if (!parseIdentifier(_parser, var_name, &var_name_len))
			{
				return false;
			}

			if (var_name_len == 0)
			{
				parseSetErrorPos(_parser, parseGetPos(_parser));
				parseSetError(_parser, CC_CODE_KEYWORD);
				CC_FUNC_DEBUG("ERROR: empty identifier\n");
				return false;
			}

			var_s *var = NULL;

			if (var_name_len == 4 && strncmp(var_name, "true", var_name_len) == 0)
			{

				char buf[3] = {
						'\0' };
				size_t len = 1;

				if (*_args_count == 0)
				{
					memcpy(buf, "0", sizeof(char));
				}

				else
				{
					len = (size_t) itoa_n(*_args_count, buf, 10);
				}

				var = VarCreate(buf, len, CC_TYPE_BOOL, _parser->depth);

				if (var == NULL)
				{
					return false;
				}

				if (!VarValueSetBool(_parser, var, true))
				{
					VarDestroy(var);
					return false;
				}

			}

			else if (var_name_len == 5 && strncmp(var_name, "false", var_name_len) == 0)
			{

				char buf[3] = {
						'\0' };
				size_t len = 1;

				if (*_args_count == 0)
				{
					memcpy(buf, "0", sizeof(char));
				}

				else
				{
					len = (size_t) itoa_n(*_args_count, buf, 10);

				}

				var = VarCreate(buf, len, CC_TYPE_BOOL, _parser->depth);

				if (var == NULL)
				{
					return false;
				}

				if (!VarValueSetBool(_parser, var, false))
				{
					VarDestroy(var);
					return false;
				}

			}
			else
			{
				// funkce nebo promenna

				file_bufferGet(_parser->buffer, &ch);

				if (ch == '[')
				{
					CC_FUNC_DEBUG("ERROR: not implemented '%c'!\n", ch);
					parseSetErrorPos(_parser, parseGetPos(_parser));
					parseSetError(_parser, CC_CODE_BAD_SYMBOL);
					return false;
				}

				else if (ch == '(')
				{
					// funkce

					var = funcCall(_parser, var_name, var_name_len);

					if (_parser->error > CC_CODE_RETURN)
					{
						VarDestroy(var);
						return false;
					}

					if (var == NULL)
					{

						CC_FUNC_DEBUG("ERROR: function returns 'NULL'..\n");
						parseSetErrorPos(_parser, parseGetPos(_parser));
						parseSetError(_parser, CC_CODE_FUNC_RET_BAD_TYPE);
						return false;
					}

					file_bufferNext(_parser->buffer);
					file_bufferSkipSpace(_parser->buffer);
					file_bufferGet(_parser->buffer, &ch);

					if (!charin(ch, ",)"))
					{
						parseSetErrorPos(_parser, parseGetPos(_parser));
						parseSetError(_parser, CC_CODE_BAD_SYMBOL);
						VarDestroy(var);
						return false;
					}

				}

				else if (ch == ')' || ch == ',')
				{
					// promenna

					var_s *_var = VarGet(_parser, var_name, var_name_len);
					if (_var == NULL)
					{
						parseSetError(_parser, CC_CODE_VAR_NOT_DEFINED);
						CC_FUNC_DEBUG("ERROR: undefined variable '%s'!\n", var_name);
						return false;
					}

					if (!_var->valid)
					{
						parseSetError(_parser, CC_CODE_VAR_NOT_ASSIGNED);
						CC_FUNC_DEBUG("ERROR: variable '%s' is uninitialized4!\n", var_name);
						return false;
					}

					var = VarCreate(_var->name, strlen(_var->name), _var->type, _var->scope);

					if (var == NULL)
					{
						return false;
					}

					if (_var->type == CC_TYPE_BOOL)
					{
						if (!VarValueSetBool(_parser, var, *(bool*) _var->data))
						{
							VarDestroy(var);
							return false;
						}
					}

					else if (_var->type == CC_TYPE_CHAR)
					{
						if (!VarValueSetChar(_parser, var, *(char*) _var->data))
						{
							VarDestroy(var);
							return false;
						}
					}

					else if (_var->type == CC_TYPE_FLOAT)
					{
						if (!VarValueSetFloat(_parser, var, *(float*) _var->data))
						{
							VarDestroy(var);
							return false;
						}
					}

					else if (_var->type == CC_TYPE_INT)
					{
						if (!VarValueSetInt(_parser, var, *(int*) _var->data))
						{
							VarDestroy(var);
							return false;
						}
					}

					else if (_var->type == CC_TYPE_LONG)
					{
						if (!VarValueSetLong(_parser, var, *(long*) _var->data))
						{
							VarDestroy(var);
							return false;
						}
					}

					else if (_var->type == CC_TYPE_STRING)
					{
						if (!VarValueSetString(_parser, var, _var->data,
								strlen((char*) (_var->data))))
						{
							VarDestroy(var);
							return false;
						}
					}

					else if (_var->type == CC_TYPE_ARRAY)
					{
						CC_FUNC_DEBUG("ERROR: not implemented\n");
						parseSetError(_parser, CC_CODE_LOGIC);
						VarDestroy(var);
						return false;
					}

					else
					{
						parseSetError(_parser, CC_CODE_TYPE_UNKNOWN);
						CC_FUNC_DEBUG("ERROR: unknown type\n");
						VarDestroy(var);
						return false;
					}
				}
				else
				{
					parseSetErrorPos(_parser, parseGetPos(_parser));
					parseSetError(_parser, CC_CODE_BAD_SYMBOL);
					CC_FUNC_DEBUG("DBEUG: tady asi chyba?'%c'.\n", ch);
					return false;
				}
			}

			_args[*_args_count] = var;
			*_args_count += 1;

			file_bufferSkipSpace(_parser->buffer);

			continue;
		}

		else if (ch == ')')
		{
			// konec

			return true;
		}

		else
		{
			CC_FUNC_DEBUG("ERROR: unexpected symbol '%c'.\n", ch);
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;
		}

	}

	return false;
}

void garbageFuncArguments(var_s **args, uint8_t args_count)
{
	CC_FUNC_DEBUG("DEBUG: destroy functions.\n");

	for (uint8_t i = 0; i < args_count; i++)
	{
		VarDestroy(args[i]);
		args[i] = NULL;
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

