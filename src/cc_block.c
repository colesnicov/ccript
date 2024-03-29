/*
 * This file is a part of "CCRipt" Embeddable Custom 'C' Like Scripting Language Interpreter
 */

/**
 * @file cc_block.c
 * @brief Implementace funkci pro praci s funkcnimi bloky.
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
#include "ccript/cc_block.h"
#include "ccript/cc_function.h"
#include "ccript/cc_parser.h"
#include "ccript/cc_types.h"
#include "ccript/cc_var_ext.h"
#include "ccript/cc_var.h"
#include <stdint.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

var_s* blockCall(cc_parser_s *_parser, cc_block_s *_block)
{

	var_s *args[CC_FUNC_NUMS_ARGS];
	uint8_t args_count = 0;

	file_bufferNext(_parser->buffer);
	file_bufferSkipSpace(_parser->buffer);

	if (!parseBlockCallArguments(_parser, _block, (var_s**) args, &args_count))
	{
		CC_BLOCK_DEBUG("ERROR: arguments error 1.\n");
		garbageFuncArguments(args, args_count);

		return NULL;
	}

	if (_block->args_count != args_count)
	{
		// kontrola jestli byl predan spravny pocet argumentu
		CC_BLOCK_DEBUG("ERROR: bad number of arguments.\n");
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		garbageFuncArguments(args, args_count);

		return NULL;
	}

	size_t pos = parseGetPos(_parser);

	_parser->buffer->offset = _block->pos_start;
	file_bufferReload(_parser->buffer);
	cvector_s *vars = _parser->vars;
	_parser->vars = NULL;

	if (!VarInit(_parser))
	{
		_parser->vars = vars;
		garbageFuncArguments(args, args_count);
		return NULL;
	}

	for (int i = 0; i < vars->total; i++)
	{
		var_s *_var = VarCopy(_parser, vars->items[i]);
		if (!VarStore(_parser, _var, false))
		{
			VarDestroy(_var);
			VarDeinit(_parser);
			_parser->vars = vars;
			garbageFuncArguments(args, args_count);
			return NULL;
		}
	}

	for (int i = 0; i < args_count; i++)
	{
		// kontrola jestli predane argumenty jsou spravneho typu.
		if (_block->args[i]->type != args[i]->type)
		{
			CC_BLOCK_DEBUG("ERROR: bad arg '%d' for function '%s'.\n", i, _block->name);
			parseSetErrorPos(_parser, pos);
			parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
			VarDeinit(_parser);
			_parser->vars = vars;
			garbageFuncArguments(args, args_count);
			return NULL;

		}

		args[i]->scope = _parser->depth;

		var_s *_var = VarCopy(_parser, args[i]);
		if (!VarStore(_parser, _var, true))
		{
			VarDestroy(_var);
			VarDeinit(_parser);
			_parser->vars = vars;
			garbageFuncArguments(args, args_count);
			return NULL;
		}
	}

	var_s *ret_var = parseBlock(_parser, '}');

	VarDeinit(_parser);
	garbageFuncArguments(args, args_count);
	_parser->vars = vars;
	vars = NULL;

	_parser->buffer->offset = pos;
	file_bufferReload(_parser->buffer);

	if (_parser->error == CC_CODE_RETURN)
	{
		return ret_var;
	}

	VarDestroy(ret_var);

	return NULL;
}

void blockDeinit(cc_parser_s *_parser)
{

	cc_block_s *block;

	if (_parser->blocks == NULL)
	{
		CC_BLOCK_DEBUG("DEBUG: destroy blocks 'uninitialized'.\n");
		return;
	}

	CC_BLOCK_DEBUG("DEBUG: destroy blocks '%d'.\n", cvector_total(_parser->blocks));

	for (uint8_t i = 0; i < cvector_total(_parser->blocks); i++)
	{
		block = cvector_get(_parser->blocks, i);

		if (block != NULL)
		{
			CC_BLOCK_DEBUG("DEBUG: destroy block '%s'.\n", block->name);
			garbageBlockDefArgs(block->args, &(block->args_count));
			CC_FREE(block->args);
			block->args = NULL;
			CC_FREE(block->name);
			block->name = NULL;
			CC_FREE(block);
			block = NULL;

		}

	}

	cvector_deinit(_parser->blocks);
	CC_FREE(_parser->blocks);
	_parser->blocks = NULL;

}

cc_block_s* blockGet(cc_parser_s *_parser, const char *_name, size_t _name_len)
{

	cc_block_s *block = NULL;

	for (uint8_t i = 0; i < cvector_total(_parser->blocks); i++)
	{
		block = cvector_get(_parser->blocks, i);

		if (_name_len == strlen(block->name) && strncmp(_name, block->name, _name_len) == 0)
		{
			return block;
		}
	}

	return NULL;
}

bool blockInit(cc_parser_s *_parser)
{
	CC_ASSERT(_parser->blocks == NULL && "CONTAINER FOR BLOCKS ALERADY INITIALIZED");

	_parser->blocks = (cvector_s*) CC_MALLOC(sizeof(cvector_s));
	if (_parser->blocks == NULL)
	{

		parseSetError(_parser, CC_CODE_NOT_MEM);
		CC_BLOCK_DEBUG("ERROR: no memmory for blocks!\n");

		return false;
	}
	memset(_parser->blocks, 0, sizeof(cvector_s));

	if (!cvector_init(_parser->blocks))
	{
		parseSetError(_parser, CC_CODE_NOT_MEM);
		CC_BLOCK_DEBUG("ERROR: no memmory for functions!\n");

		CC_FREE(_parser->blocks);
		_parser->funcs = NULL;

		return false;
	}

	return true;
}

void dumpBlockArgs(cc_block_args_s **_args, uint8_t _args_count)
{
	CC_BLOCK_DEBUG("DEBUG: dump block arguments start: '%d'.\n", _args_count);
	for (uint8_t i = 0; i < _args_count; i++)
	{
		CC_BLOCK_DEBUG("DEBUG: dump block argument: '%d':'%s'.\n", i,
				((cc_block_args_s* )_args[i])->name);
	}CC_BLOCK_DEBUG("DEBUG: dump block arguments end.\n");
}

void garbageBlockDefArgs(cc_block_args_s **_args, uint8_t *_args_count)
{
	CC_BLOCK_DEBUG("DEBUG: destroy block arguments: '%d'.\n", *_args_count);
	for (uint8_t i = 0; i < (*_args_count); i++)
	{
		if (_args[i] != NULL)
		{
			CC_BLOCK_DEBUG("DEBUG: destroy block argument: '%d':'%s'.\n", i, _args[i]->name);
			CC_FREE(_args[i]->name);
			_args[i]->name = NULL;
			CC_FREE(_args[i]);
			_args[i] = NULL;
		}
	}

	*_args_count = 0;
}

bool parseBlockArguments(cc_parser_s *_parser, cc_block_args_s **_args, uint8_t *_args_count)
{
	char ch;

	size_t var_type_len = 0;
	uint8_t index = 0;

	while (FILEBUFFER_OK == file_bufferValid(_parser->buffer))
	{

		parseSkipNewLine(_parser);

		file_bufferGet(_parser->buffer, &ch);

		if (!isalpha(ch))
		{
			CC_BLOCK_DEBUG("ERROR: unexpected symbol '%c'.\n", ch);
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;
		}

		char var_type[CC_KEYWORD_LEN] = {
				'\0' };

		if (!parseIdentifier(_parser, var_type, &var_type_len))
		{
			return false;
		}

		if (var_type_len == 0)
		{
			parseSetErrorPos(_parser, parseGetPos(_parser));
			parseSetError(_parser, CC_CODE_KEYWORD);
			CC_BLOCK_DEBUG("ERROR: empty identifier\n");
			return false;
		}

		cc_block_args_s *var = NULL;
		var = (cc_block_args_s*) CC_MALLOC(sizeof(cc_block_args_s));
		if (var == NULL)
		{
			parseSetErrorPos(_parser, parseGetPos(_parser));
			parseSetError(_parser, CC_CODE_NOT_MEM);
			CC_BLOCK_DEBUG("ERROR: not enought memmory for block args\n");
			return NULL;
		}

		memset(var, 0, sizeof(cc_block_args_s));

		if (var_type_len == 4 && strncmp(var_type, "bool", var_type_len) == 0)
		{
			var->type = CC_TYPE_BOOL;
		}

		else if (var_type_len == 3 && strncmp(var_type, "int", var_type_len) == 0)
		{
			var->type = CC_TYPE_INT;
		}

		else if (var_type_len == 4 && strncmp(var_type, "long", var_type_len) == 0)
		{
			var->type = CC_TYPE_LONG;
		}

		else if (var_type_len == 5 && strncmp(var_type, "float", var_type_len) == 0)
		{
			var->type = CC_TYPE_FLOAT;
		}

		else if (var_type_len == 4 && strncmp(var_type, "char", var_type_len) == 0)
		{
			var->type = CC_TYPE_BOOL;
		}

		else if (var_type_len == 6 && strncmp(var_type, "string", var_type_len) == 0)
		{
			var->type = CC_TYPE_BOOL;
		}

		else
		{
			parseSetError(_parser, CC_CODE_TYPE_UNKNOWN);
			CC_BLOCK_DEBUG("ERROR: unknown type\n");
			CC_FREE(var);
			return false;
		}

		file_bufferSkipSpace(_parser->buffer);
		file_bufferGet(_parser->buffer, &ch);

		if (!isalpha(ch))
		{
			CC_BLOCK_DEBUG("ERROR: unexpected symbol '%c'.\n", ch);
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			CC_FREE(var);
			return false;
		}

		char var_name[CC_KEYWORD_LEN] = {
				'\0' };
		size_t var_name_len = 0;

		if (!parseIdentifier(_parser, var_name, &var_name_len))
		{
			CC_FREE(var);
			return false;
		}

		if (var_name_len == 0)
		{
			parseSetErrorPos(_parser, parseGetPos(_parser));
			parseSetError(_parser, CC_CODE_KEYWORD);
			CC_BLOCK_DEBUG("ERROR: empty identifier\n");
			CC_FREE(var);
			return false;
		}

		var->name = (char*) CC_MALLOC((var_name_len + 1) * sizeof(char));
		if (var->name == NULL)
		{
			parseSetError(_parser, CC_CODE_NOT_MEM);
			CC_BLOCK_DEBUG("ERROR: not enough memmory for block arguments.\n");
			CC_FREE(var);
			return false;
		}
		memset(var->name, 0, (var_name_len + 1) * sizeof(char));
		memmove(var->name, var_name, var_name_len * sizeof(char));

		var->name_len = var_name_len;
		_args[index] = var;

		index++;

		file_bufferSkipSpace(_parser->buffer);
		file_bufferGet(_parser->buffer, &ch);

		if (ch == ',')
		{
			// dalsi argument
			file_bufferNext(_parser->buffer);

			continue;
		}

		else if (ch == ')')
		{
			*_args_count = index;
			// konec
			return true;
		}

		else
		{
			CC_BLOCK_DEBUG("ERROR: unexpected symbol '%c'.\n", ch);
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;
		}

	}

	return false;
}

bool parseBlockCallArguments(cc_parser_s *_parser, cc_block_s *_block, var_s **_args,
		uint8_t *_args_count)
{
	char ch;

	size_t value_len = 0;
	while (FILEBUFFER_OK == file_bufferValid(_parser->buffer))
	{

		parseSkipNewLine(_parser);

		file_bufferGet(_parser->buffer, &ch);

		if (ch == ')')
		{
			// konec

			return true;
		}

		else if (ch == ',')
		{
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

			var_s *_var = VarCreate(_block->args[*_args_count]->name,
					_block->args[*_args_count]->name_len, CC_TYPE_CHAR, _parser->depth);
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
			*_args_count += 1;

			file_bufferNext(_parser->buffer);

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

			var_s *_var = VarCreate(_block->args[*_args_count]->name,
					_block->args[*_args_count]->name_len, CC_TYPE_STRING, _parser->depth);
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

		}

		else if (isdigit(ch) || ch == '-')
		{
			// cislo

			char value[CC_VALUE_NUMERIC_LEN + 1] = {
					'\0' };

			bool is_float = false;
			if (!parseValueFloat(_parser, value, &value_len, &is_float))
			{
				return false;
			}

			if (value_len == 0)
			{
				parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
				CC_BLOCK_DEBUG("ERROR: empty int/float value!\n");
				return false;

			}

			var_s *_var = NULL;

			if (is_float)
			{
				_var = VarCreate(_block->args[*_args_count]->name,
						_block->args[*_args_count]->name_len, CC_TYPE_FLOAT, _parser->depth);

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
				_var = VarCreate(_block->args[*_args_count]->name,
						_block->args[*_args_count]->name_len, CC_TYPE_INT, _parser->depth);

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
				CC_BLOCK_DEBUG("ERROR: empty identifier\n");
				return false;
			}

			var_s *var = NULL;

			if (var_name_len == 4 && strncmp(var_name, "true", var_name_len) == 0)
			{
				var = VarCreate(_block->args[*_args_count]->name,
						_block->args[*_args_count]->name_len, CC_TYPE_BOOL, _parser->depth);

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
				var = VarCreate(_block->args[*_args_count]->name,
						_block->args[*_args_count]->name_len, CC_TYPE_BOOL, _parser->depth);

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
					CC_BLOCK_DEBUG("ERROR: not implemented '%c'!\n", ch);
					parseSetErrorPos(_parser, parseGetPos(_parser));
					parseSetError(_parser, CC_CODE_BAD_SYMBOL);
					return false;
				}

				else if (ch == ')' || ch == ',')
				{
					// promenna

					var_s *_var = VarGet(_parser, var_name, var_name_len);
					if (_var == NULL)
					{
						parseSetError(_parser, CC_CODE_VAR_NOT_DEFINED);
						CC_BLOCK_DEBUG("ERROR: undefined variable '%s'!\n", var_name);
						return false;
					}

					if (!_var->valid)
					{
						assert(0);
						parseSetError(_parser, CC_CODE_VAR_NOT_ASSIGNED);
						CC_BLOCK_DEBUG("ERROR: variable '%s' is uninitialized2!\n", var_name);
						return false;
					}

					var = VarCreate(_block->args[*_args_count]->name,
							_block->args[*_args_count]->name_len, _var->type, _var->scope);

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
							assert(0);
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
						CC_BLOCK_DEBUG("ERROR: not implemented\n");
						parseSetError(_parser, CC_CODE_LOGIC);
						VarDestroy(var);
						return false;
					}

					else
					{
						parseSetError(_parser, CC_CODE_TYPE_UNKNOWN);
						CC_BLOCK_DEBUG("ERROR: unknown type\n");
						VarDestroy(var);
						return false;
					}
				}
				else
				{
					parseSetErrorPos(_parser, parseGetPos(_parser));
					parseSetError(_parser, CC_CODE_BAD_SYMBOL);
					CC_BLOCK_DEBUG("DBEUG: tady asi chyba?'%c'.\n", ch);
					return false;
				}
			}

			_args[*_args_count] = var;
			*_args_count += 1;

		}

		else
		{
			CC_BLOCK_DEBUG("ERROR: unexpected symbol '%c'.\n", ch);
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;
		}

		if (*_args_count > _block->args_count)
		{
			return false;
		}

	}

	return false;
}

bool parseDefineBlock(cc_parser_s *_parser, cc_type_t _type, const char *_block_name,
		size_t _block_name_len)
{

	char ch;

	parseSkipNewLine(_parser);

	file_bufferGet(_parser->buffer, &ch);

	if (ch != '(')
	{
		CC_BLOCK_DEBUG("ERROR: unexpected symbol '%c'.\n", ch);
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;
	}

	file_bufferNext(_parser->buffer);

	cc_block_s *block = (cc_block_s*) CC_MALLOC(sizeof(cc_block_s));
	if (block == NULL)
	{
		parseSetError(_parser, CC_CODE_NOT_MEM);
		CC_BLOCK_DEBUG("ERROR: not enough memmory for block arguments.\n");
		return false;
	}
	memset(block, 0, sizeof(cc_block_s));

	cc_block_args_s *args[CC_FUNC_NUMS_ARGS] = {
			NULL };
	uint8_t args_count = 0;

	if (!parseBlockArguments(_parser, args, &args_count))
	{
		garbageBlockDefArgs(args, &args_count);
		CC_FREE(block);
		block = NULL;

		return false;
	}

	file_bufferNext(_parser->buffer);
	parseSkipNewLine(_parser);

	file_bufferGet(_parser->buffer, &ch);

	if (ch != '{')
	{
		CC_BLOCK_DEBUG("ERROR: unexpected symbol '%c'.\n", ch);
		garbageBlockDefArgs(args, &args_count);
		CC_FREE(block);
		block = NULL;

		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;
	}

	file_bufferNext(_parser->buffer);
	parseSkipNewLine(_parser);

	block->pos_start = parseGetPos(_parser);

	if (!parserSkipBlock(_parser, '{', '}'))
	{
		garbageBlockDefArgs(args, &args_count);
		CC_FREE(block);
		block = NULL;

		return false;
	}

	block->name = (char*) CC_MALLOC(_block_name_len + 1 * sizeof(char));
	if (block->name == NULL)
	{
		CC_BLOCK_DEBUG("ERROR: not enough memmory for block name.\n");
		parseSetError(_parser, CC_CODE_NOT_MEM);
		garbageBlockDefArgs(args, &args_count);
		CC_FREE(block);
		block = NULL;
		return false;
	}
	memset(block->name, 0, _block_name_len + 1 * sizeof(char));
	memmove(block->name, _block_name, _block_name_len);

	block->args = (cc_block_args_s**) CC_MALLOC(sizeof(cc_block_args_s));
	if (block->args == NULL)
	{
		parseSetError(_parser, CC_CODE_NOT_MEM);
		CC_BLOCK_DEBUG("ERROR: not enough memmory for block arguments.\n");
		CC_FREE(block);
		block = NULL;
		return false;
	}
	memset(block->args, 0, sizeof(cc_block_args_s));

	for (int i = 0; i < args_count; i++)
	{

		block->args[i] = args[i];
	}

	block->pos_end = parseGetPos(_parser);
	block->args_count = args_count;

	if (!cvector_add(_parser->blocks, block))
	{
		garbageBlockDefArgs(block->args, (uint8_t*) &block->args_count);
		CC_FREE(block->name);
		block->name = NULL;
		CC_FREE(block);
		block = NULL;

		return false;
	}

	return true;
}

var_s* parseReturnArguments(cc_parser_s *_parser)
{
	char ch;

	size_t value_len = 0;

	parseSkipNewLine(_parser);

	file_bufferGet(_parser->buffer, &ch);

	var_s *_var = NULL;

	if (ch == '\'')
	{
		// pismeno/znak/symbol

		char value = 0;

		if (!ParseValueChar(_parser, &value, &value_len))
		{
			return NULL;
		}

		_var = VarCreate("return", 6, CC_TYPE_CHAR, _parser->depth);
		if (_var == NULL)
		{
			return NULL;
		}

		if (!VarValueSetChar(_parser, _var, value))
		{
			VarDestroy(_var);
			return NULL;
		}

	}

	else if (ch == '"')
	{
		// retezec

		char value[CC_VALUE_STRING_LEN + 1] = {
				'\0' };

		if (!ParseValueString(_parser, value, &value_len))
		{
			return NULL;
		}

		_var = VarCreate("return", 6, CC_TYPE_STRING, _parser->depth);
		if (_var == NULL)
		{
			return NULL;
		}

		if (!VarValueSetString(_parser, _var, value, value_len))
		{
			VarDestroy(_var);
			return NULL;
		}

	}

	else if (isdigit(ch) || ch == '-')
	{
		// cislo

		char value[CC_VALUE_NUMERIC_LEN] = {
				'\0' };

		bool is_float = false;
		if (!parseValueFloat(_parser, value, &value_len, &is_float))
		{
			return NULL;
		}

		if (value_len == 0)
		{
			parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
			CC_BLOCK_DEBUG("ERROR: empty int/float value!\n");
			return NULL;

		}

		if (is_float)
		{

			_var = VarCreate("return", 6, CC_TYPE_FLOAT, _parser->depth);

			if (_var == NULL)
			{
				return NULL;
			}

			if (!VarValueSetFloat(_parser, _var, atof(value)))
			{
				VarDestroy(_var);
				return NULL;
			}

		}
		else
		{

			_var = VarCreate("return", 6, CC_TYPE_INT, _parser->depth);

			if (_var == NULL)
			{
				CC_BLOCK_DEBUG("CHYBA?\n\n");
				return NULL;
			}

			if (!VarValueSetInt(_parser, _var, atoi(value)))
			{
				VarDestroy(_var);
				return NULL;
			}

		}

	}

	else if (isalpha(ch))
	{

		char var_name[CC_KEYWORD_LEN] = {
				'\0' };
		size_t var_name_len = 0;

		if (!parseIdentifier(_parser, var_name, &var_name_len))
		{
			return NULL;
		}

		if (var_name_len == 0)
		{
			parseSetErrorPos(_parser, parseGetPos(_parser));
			parseSetError(_parser, CC_CODE_KEYWORD_EMPTY);
			CC_BLOCK_DEBUG("ERROR: empty identifier\n");
			return NULL;
		}

		if (var_name_len == 4 && strncmp(var_name, "true", var_name_len) == 0)
		{

			_var = VarCreate("return", 6, CC_TYPE_BOOL, _parser->depth);

			if (_var == NULL)
			{
				return NULL;
			}

			if (!VarValueSetBool(_parser, _var, true))
			{
				VarDestroy(_var);
				return NULL;
			}

		}

		else if (var_name_len == 5 && strncmp(var_name, "false", var_name_len) == 0)
		{

			_var = VarCreate("return", 6, CC_TYPE_BOOL, _parser->depth);

			if (_var == NULL)
			{
				return NULL;
			}

			if (!VarValueSetBool(_parser, _var, false))
			{
				VarDestroy(_var);
				return NULL;
			}

		}

		else
		{
			// funkce nebo promenna

			file_bufferGet(_parser->buffer, &ch);

			if (ch == '[')
			{
				CC_BLOCK_DEBUG("ERROR: not implemented '%c'!\n", ch);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				parseSetError(_parser, CC_CODE_NOT_IMPLEMENTED);
				return NULL;
			}

			else if (ch == '(')
			{
				// funkce

				_var = funcCall(_parser, var_name, var_name_len);

				if (_parser->error > CC_CODE_RETURN)
				{
					VarDestroy(_var);
					return NULL;
				}

				if (_var == NULL)
				{

					CC_BLOCK_DEBUG("ERROR: function returns 'NULL'..\n");
					parseSetErrorPos(_parser, parseGetPos(_parser));
					parseSetError(_parser, CC_CODE_FUNC_RET_BAD_TYPE);
					return NULL;
				}

				file_bufferNext(_parser->buffer);
				file_bufferSkipSpace(_parser->buffer);
				file_bufferGet(_parser->buffer, &ch);

				if (!charin(ch, ",)"))
				{
					parseSetErrorPos(_parser, parseGetPos(_parser));
					parseSetError(_parser, CC_CODE_BAD_SYMBOL);
					VarDestroy(_var);
					return NULL;
				}

			}

			else if (ch == ';')
			{
				// promenna

				var_s *var = VarGet(_parser, var_name, var_name_len);
				if (var == NULL)
				{
					parseSetError(_parser, CC_CODE_VAR_NOT_DEFINED);
					CC_BLOCK_DEBUG("ERROR: undefined variable '%s'!\n", var_name);
					return NULL;
				}

				if (!var->valid)
				{
					parseSetError(_parser, CC_CODE_VAR_NOT_ASSIGNED);
					CC_BLOCK_DEBUG("ERROR: variable '%s' is uninitialized3!\n", var_name);
					return NULL;
				}

				_var = VarCreate("return", 6, var->type, var->scope);

				if (_var == NULL)
				{
					return NULL;
				}

				if (_var->type == CC_TYPE_BOOL)
				{
					if (!VarValueSetBool(_parser, _var, *(bool*) var->data))
					{
						VarDestroy(_var);
						return NULL;
					}
				}

				else if (_var->type == CC_TYPE_CHAR)
				{
					if (!VarValueSetChar(_parser, _var, *(char*) var->data))
					{
						VarDestroy(_var);
						return NULL;
					}
				}

				else if (_var->type == CC_TYPE_FLOAT)
				{
					if (!VarValueSetFloat(_parser, _var, *(float*) var->data))
					{
						VarDestroy(_var);
						return NULL;
					}
				}

				else if (_var->type == CC_TYPE_INT)
				{
					if (!VarValueSetInt(_parser, _var, *(int*) var->data))
					{
						VarDestroy(_var);
						return NULL;
					}
				}

				else if (_var->type == CC_TYPE_LONG)
				{
					if (!VarValueSetLong(_parser, _var, *(long*) var->data))
					{
						VarDestroy(_var);
						return NULL;
					}
				}

				else if (_var->type == CC_TYPE_STRING)
				{
					if (!VarValueSetString(_parser, _var, var->data,
							strlen((char*) (_var->data))))
					{
						VarDestroy(_var);
						return NULL;
					}
				}

				else if (_var->type == CC_TYPE_ARRAY)
				{
					CC_BLOCK_DEBUG("ERROR: not implemented\n");
					parseSetError(_parser, CC_CODE_NOT_IMPLEMENTED);
					VarDestroy(_var);
					return NULL;
				}

				else
				{
					parseSetError(_parser, CC_CODE_TYPE_UNKNOWN);
					CC_BLOCK_DEBUG("ERROR: unknown type\n");
					VarDestroy(_var);
					return NULL;
				}

			}

			else
			{
				parseSetErrorPos(_parser, parseGetPos(_parser));
				parseSetError(_parser, CC_CODE_BAD_SYMBOL);
				CC_BLOCK_DEBUG("DBEUG: tady asi chyba?'%c'.\n", ch);

				return NULL;
			}
		}

		file_bufferSkipSpace(_parser->buffer);

	}

	else if (ch == ';')
	{
		return NULL;
	}

	else
	{
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return NULL;
	}

	return _var;
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

