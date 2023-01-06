/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file cc_block.c
 * @brief Implementace funkci pro praci s funkcnimi bloky
 *
 * @version 1b1
 * @date 26.06.2022
 *
 * @author Denis Colesnicov <eugustus@gmail.com>
 *
 * @copyright MIT License
 *
 */

#include "ccript/cc_configs.h"
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

void dumpBlockArgs(cc_block_args_s **_args, uint8_t _args_count)
{
	CC_PRINT("DEBUG: dump block arguments start: '%d'.\n", _args_count);
	for (uint8_t i = 0; i < _args_count; i++)
	{
		CC_PRINT("DEBUG: dump block argument: '%d':'%s'.\n", i,
				((cc_block_args_s* )_args[i])->name);
	}
	CC_PRINT("DEBUG: dump block arguments end.\n");
}

bool blockInit(parser_s *_parser)
{
	CC_ASSERT(_parser->blocks == NULL && "CONTAINER FOR BLOCKS ALERADY INITIALIZED");

	_parser->blocks = (cvector_s*) CONFIG_CC_MALLOC(sizeof(cvector_s));
	if (_parser->blocks == NULL)
	{

		parseSetError(_parser, CC_CODE_NOT_MEM);
		CC_PRINT("ERROR: no memmory for blocks!\n");

		return false;
	}
	memset(_parser->blocks, 0, sizeof(cvector_s));

	if (!cvector_init(_parser->blocks))
	{
		parseSetError(_parser, CC_CODE_NOT_MEM);
		CC_PRINT("ERROR: no memmory for functions!\n");

		CONFIG_CC_FREE(_parser->blocks);
		_parser->funcs = NULL;

		return false;
	}

	return true;
}

void blockDeinit(parser_s *_parser)
{

	CC_ASSERT(_parser != NULL && "PARSER IS NULL!");

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
			blockCleanArguments(block->args, &(block->args_count));
			CONFIG_CC_FREE(block->args);
			block->args = NULL;
			CONFIG_CC_FREE(block->name);
			block->name = NULL;
			CONFIG_CC_FREE(block);
			block = NULL;

		}

	}

	cvector_deinit(_parser->blocks);
	CONFIG_CC_FREE(_parser->blocks);
	_parser->blocks = NULL;

}

var_s* blockCall(parser_s *_parser, cc_block_s *_block, const char *func_name,
		size_t func_name_len)
{

	var_s *var = NULL;
	var_s *args[CC_FUNC_NUMS_ARGS];
	uint8_t args_count = 0;

	file_bufferNext(_parser->buffer);
	file_bufferSkipSpace(_parser->buffer);

	if (!parseFuncArguments(_parser, func_name, func_name_len, (var_s**) args, &args_count))
	{
		CC_PRINT("ERROR: arguments error 1.\n");
		funcClearArguments(args, args_count);

		return NULL;
	}

	// fixme nastavovat kod chyby!
	if (_block->args_count != args_count)
	{
		// kontrola jestli byl predan spravny pocet argumentu
		CC_PRINT("ERROR: bad number of arguments.\n");
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		funcClearArguments(args, args_count);

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
		funcClearArguments(args, args_count);
		return NULL;
	}

	for (int i = 0; i < args_count; i++)
	{
		// kontrola jestli predane argumenty jsou spravneho typu.
		if (_block->args[i]->type != args[i]->type)
		{
			CC_PRINT("ERROR: bad arg '%d' for function '%s'.\n", i, _block->name);
			parseSetErrorPos(_parser, pos);
			parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
			VarDeinit(_parser);
			_parser->vars = vars;
			funcClearArguments(args, args_count);
			return NULL;

		}

		var_s *_var = VarCreate(_block->args[i]->name, _block->args[i]->name_len,
				_block->args[i]->type, _parser->depth);

		VarValueSetInt(_parser, _var, *((int*) args[i]->data));

		if (!VarStore(_parser, _var))
		{
			VarDestroy(_var);
			VarDeinit(_parser);
			_parser->vars = vars;
			funcClearArguments(args, args_count);
			return NULL;
		}
	}

	var_s *ret_var = parseBlock(_parser, '}');

	VarDeinit(_parser);
	_parser->buffer->offset = pos;
	file_bufferReload(_parser->buffer);

	_parser->vars = vars;

	funcClearArguments(args, args_count);

	if (_parser->error == CC_CODE_RETURN)
	{
		return ret_var;
	}

	VarDestroy(ret_var);

	return NULL;
}

bool parseBlockCallArguments(parser_s *_parser, const char *phrase_name, size_t phrase_name_len,
		var_s **_args, uint8_t *_args_count)
{
	char ch;

	size_t value_len = 0;
	while (FILEBUFFER_OK == file_bufferValid(_parser->buffer))
	{

		parseSkipNewLine(_parser);

		file_bufferGet(_parser->buffer, &ch);

		if (ch == ',')
		{
			// dalsi argument

			file_bufferNext(_parser->buffer);
			file_bufferSkipSpace(_parser->buffer);
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

			var_s *_var = VarCreate("_", 1, CC_TYPE_CHAR, _parser->depth);
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
			continue;
		}

		else if (ch == '"')
		{
			// retezec

			char value[CONFIG_CC_STRING_LEN] = {
					'\0' };

			if (!ParseValueString(_parser, value, &value_len))
			{
				return false;
			}

			var_s *_var = VarCreate("_", 1, CC_TYPE_STRING, _parser->depth);
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
				CC_PRINT("ERROR: empty int/float value!\n");
				return false;

			}

			var_s *_var = NULL;

			if (is_float)
			{
				_var = VarCreate("_", 1, CC_TYPE_FLOAT, _parser->depth);

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
				_var = VarCreate("_", 1, CC_TYPE_INT, _parser->depth);

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
				CC_PRINT("ERROR: empty identifier\n");
				return false;
			}

			var_s *var = NULL;

			if (var_name_len == 4 && strncmp(var_name, "true", var_name_len) == 0)
			{
				var = VarCreate("_", 1, CC_TYPE_BOOL, _parser->depth);

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
				var = VarCreate("_", 1, CC_TYPE_BOOL, _parser->depth);

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
					CC_PRINT("ERROR: not implemented '%c'!\n", ch);
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
						CC_PRINT("ERROR: undefined variable '%s'!\n", var_name);
						// fixme nastavit navratove kody chyb!
						return false;
					}

					if (!_var->valid)
					{
						parseSetError(_parser, CC_CODE_VAR_NOT_ASSIGNED);
						CC_PRINT("ERROR: variable '%s' is uninitialized!\n", var_name);
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
						CC_PRINT("ERROR: not implemented\n");
						parseSetError(_parser, CC_CODE_LOGIC);
						VarDestroy(var);
						return false;
					}

					else
					{
						parseSetError(_parser, CC_CODE_TYPE_UNKNOWN);
						CC_PRINT("ERROR: unknown type\n");
						VarDestroy(var);
						return false;
					}
				}
				else
				{
					parseSetErrorPos(_parser, parseGetPos(_parser));
					parseSetError(_parser, CC_CODE_BAD_SYMBOL);
					CC_PRINT("DBEUG: tady asi chyba?'%c'.\n", ch);
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
			CC_PRINT("ERROR: unexpected symbol '%c'.\n", ch);
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;
		}

	}

	return false;
}

cc_block_s* blockGet(parser_s *_parser, const char *_name, size_t _name_len)
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

bool parseDefineBlock(parser_s *_parser, cc_type_t _type, const char *block_name,
		size_t block_name_len)
{

	char ch;

	size_t value_len = 0;

	parseSkipNewLine(_parser);

	file_bufferGet(_parser->buffer, &ch);

	if (ch != '(')
	{
		CC_PRINT("ERROR: unexpected symbol '%c'.\n", ch);
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;
	}

	file_bufferNext(_parser->buffer);

	cc_block_s *block = (cc_block_s*) CONFIG_CC_MALLOC(sizeof(cc_block_s));
	if (block == NULL)
	{
		parseSetError(_parser, CC_CODE_NOT_MEM);
		CC_PRINT("ERROR: not enough memmory for block arguments.\n");
		return false;
	}
	memset(block, 0, sizeof(cc_block_s));

	cc_block_args_s *args[CC_FUNC_NUMS_ARGS] = {
			NULL };
	uint8_t args_count = 0;

	if (!parseBlockArguments(_parser, block_name, block_name_len, args, &args_count))
	{
		blockCleanArguments(args, &args_count);
		CONFIG_CC_FREE(block);
		block = NULL;

		return false;
	}

	file_bufferNext(_parser->buffer);
	parseSkipNewLine(_parser);

	file_bufferGet(_parser->buffer, &ch);

	if (ch != '{')
	{
		CC_PRINT("ERROR: unexpected symbol '%c'.\n", ch);
		blockCleanArguments(args, &args_count);
		CONFIG_CC_FREE(block);
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
		blockCleanArguments(args, &args_count);
		CONFIG_CC_FREE(block);
		block = NULL;

		return false;
	}

	block->name = (char*) CONFIG_CC_MALLOC(block_name_len + 1 * sizeof(char));
	if (block->name == NULL)
	{
		CC_PRINT("ERROR: not enough memmory for block name.\n");
		parseSetError(_parser, CC_CODE_NOT_MEM);
		blockCleanArguments(args, &args_count);
		CONFIG_CC_FREE(block);
		block = NULL;
		return false;
	}
	memset(block->name, 0, block_name_len + 1 * sizeof(char));
	memmove(block->name, block_name, block_name_len);

	block->args = (cc_block_args_s**) CONFIG_CC_MALLOC(sizeof(cc_block_args_s));
	if (block->args == NULL)
	{
		parseSetError(_parser, CC_CODE_NOT_MEM);
		CC_PRINT("ERROR: not enough memmory for block arguments.\n");
		CONFIG_CC_FREE(block);
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
		blockCleanArguments(block->args, (uint8_t*) &block->args_count);
		CONFIG_CC_FREE(block->name);
		block->name = NULL;
		CONFIG_CC_FREE(block);
		block = NULL;

		return false;
	}

	return true;
}

bool parseBlockArguments(parser_s *_parser, const char *block_name, size_t block_name_len,
		cc_block_args_s **_args, uint8_t *_args_count)
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
			CC_PRINT("ERROR: unexpected symbol '%c'.\n", ch);
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
			CC_PRINT("ERROR: empty identifier\n");
			return false;
		}

		cc_block_args_s *var = NULL;
		var = (cc_block_args_s*) CONFIG_CC_MALLOC(sizeof(cc_block_args_s));
		if (var == NULL)
		{
			parseSetError(_parser, CC_CODE_NOT_MEM);
			CC_PRINT("ERROR: not enought memmory for block args '%s'\n", block_name);
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
			CC_PRINT("ERROR: unknown type\n");
			CONFIG_CC_FREE(var);
			return false;
		}

		file_bufferSkipSpace(_parser->buffer);
		file_bufferGet(_parser->buffer, &ch);

		if (!isalpha(ch))
		{
			CC_PRINT("ERROR: unexpected symbol '%c'.\n", ch);
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			CONFIG_CC_FREE(var);
			return false;
		}

		char var_name[CC_KEYWORD_LEN] = {
				'\0' };
		size_t var_name_len = 0;

		if (!parseIdentifier(_parser, var_name, &var_name_len))
		{
			CONFIG_CC_FREE(var);
			return false;
		}

		if (var_name_len == 0)
		{
			parseSetErrorPos(_parser, parseGetPos(_parser));
			parseSetError(_parser, CC_CODE_KEYWORD);
			CC_PRINT("ERROR: empty identifier\n");
			CONFIG_CC_FREE(var);
			return false;
		}

		var->name = (char*) CONFIG_CC_MALLOC((var_name_len + 1) * sizeof(char));
		if (var->name == NULL)
		{
			parseSetError(_parser, CC_CODE_NOT_MEM);
			CC_PRINT("ERROR: not enough memmory for block arguments.\n");
			CONFIG_CC_FREE(var);
			return false;
		}
		memset(var->name, 0, (var_name_len + 1) * sizeof(char));
		memmove(var->name, var_name, var_name_len * sizeof(char));

		var->name_len = var_name_len;
		_args[index] = var;

		index++;
		*_args_count = index;

		file_bufferSkipSpace(_parser->buffer);
		file_bufferGet(_parser->buffer, &ch);

		if (ch == ',')
		{
			// dalsi argument
			file_bufferNext(_parser->buffer);
//			file_bufferSkipSpace(_parser->buffer);

			continue;
		}

		else if (ch == ')')
		{
			// konec
			return true;
		}

		else
		{
			CC_PRINT("ERROR: unexpected symbol '%c'.\n", ch);
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;
		}

	}

	return false;
}

void blockCleanArguments(cc_block_args_s **_args, uint8_t *_args_count)
{
	CC_BLOCK_DEBUG("DEBUG: destroy block arguments: '%d'.\n", *_args_count);
	for (uint8_t i = 0; i < (*_args_count); i++)
	{
		if (_args[i] != NULL)
		{
			CC_BLOCK_DEBUG("DEBUG: destroy block argument: '%d':'%s'.\n", i, _args[i]->name);
			CONFIG_CC_FREE(_args[i]->name);
			_args[i]->name = NULL;
			CONFIG_CC_FREE(_args[i]);
			_args[i] = NULL;
		}
	}

	*_args_count = 0;
}

var_s* parseReturnArguments(parser_s *_parser)
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

//		file_bufferNext(_parser->buffer);

	}

	else if (ch == '"')
	{
		// retezec

		char value[CONFIG_CC_STRING_LEN] = {
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
			CC_PRINT("ERROR: empty int/float value!\n");
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
				CC_PRINT("CHYBA?\n\n");
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
			parseSetError(_parser, CC_CODE_KEYWORD);
			CC_PRINT("ERROR: empty identifier\n");
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
				CC_PRINT("ERROR: not implemented '%c'!\n", ch);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				parseSetError(_parser, CC_CODE_BAD_SYMBOL);
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

					CC_PRINT("ERROR: function returns 'NULL'..\n");
					parseSetErrorPos(_parser, parseGetPos(_parser));
					parseSetError(_parser, CC_CODE_FUNC_BAD_TYPE);
					return NULL;
				}

				file_bufferNext(_parser->buffer);
				file_bufferSkipSpace(_parser->buffer);
				file_bufferGet(_parser->buffer, &ch);

				// fixme tady hledat carku ','? kdyz ji odstranim?
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
					CC_PRINT("ERROR: undefined variable '%s'!\n", var_name);
					// fixme nastavit navratove kody chyb!
					return NULL;
				}

				if (!var->valid)
				{
					parseSetError(_parser, CC_CODE_VAR_NOT_ASSIGNED);
					CC_PRINT("ERROR: variable '%s' is uninitialized!\n", var_name);
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
					CC_PRINT("ERROR: not implemented\n");
					parseSetError(_parser, CC_CODE_LOGIC);
					VarDestroy(_var);
					return NULL;
				}

				else
				{
					parseSetError(_parser, CC_CODE_TYPE_UNKNOWN);
					CC_PRINT("ERROR: unknown type\n");
					VarDestroy(_var);
					return NULL;
				}

			}

			else
			{
				parseSetErrorPos(_parser, parseGetPos(_parser));
				parseSetError(_parser, CC_CODE_BAD_SYMBOL);
				CC_PRINT("DBEUG: tady asi chyba?'%c'.\n", ch);

				return NULL;
			}
		}

		file_bufferSkipSpace(_parser->buffer);

	}

	else if (ch == ';')
	{
		// fixme odkomentovat?
//		CC_PRINT(" exit without return value '%c'.\n", ch);
//		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
//		parseSetErrorPos(_parser, parseGetPos(_parser));
		return NULL;
	}

	else
	{
		CC_PRINT("ERROR: unexpected symbol '%c'.\n", ch);
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

