/*
 * This file is a part of "CCRipt" Embeddable Custom 'C' Like Scripting Language Interpreter
 */

/**
 * @file cc_parse.c
 * @brief Implementace funkci pro parsovani skriptu.
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
#include "ccript/cc_var_ext.h"
#include "ccript/cc_function.h"
#include "ccript/cc_parseIf.h"
#include "ccript/cc_parseWhile.h"
#include "ccript/cc_configs.h"
#include "ccript/cc_parser.h"
#include "ccript/cc_types.h"
#include "ccript/cc_var.h"
#include "ccript/common.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#ifdef ESP_PLATFORM
#include <freertos/private/projdefs.h>
#include <freertos/task.h>
#endif

var_s* parseBlock(cc_parser_s *_parser, char _end_char)
{
	/**
	 * @var char keyword_name[CC_KEYWORD_LEN]
	 * @brief Nalezeny prvni vyraz ve vete
	 * @details	Typicky: type(int,string,...), function name, command(if,while,...)
	 *
	 */
	char keyword_name[CC_KEYWORD_LEN + 1] = {
			'\0' };

	char ch = '\0';

	uint8_t scope = _parser->depth;

	var_s *ret_var = NULL;

	while (FILEBUFFER_OK == file_bufferValid(_parser->buffer))
	{
#if defined(ESP_PLATFORM) && (0 < CONFIG_CC_TASK_DELAY)
		vTaskDelay(pdMS_TO_TICKS(CONFIG_CC_TASK_DELAY));
#endif

		memset(keyword_name, '\0', CC_KEYWORD_LEN);
		size_t keyword_len = 0;

		parseSkipNewLine(_parser);

		if (FILEBUFFER_OK != file_bufferGet(_parser->buffer, &ch))
		{
			parseSetErrorPos(_parser, parseGetPos(_parser));
			parseSetError(_parser, CC_CODE_IO);
			return NULL;
		}

		if (isdigit(ch))
		{
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));

			return NULL;
		}

		if (_end_char != 0 && ch == _end_char)
		{
			_parser->depth = scope;
			VarGarbageScope(_parser);
			parseSetError(_parser, CC_CODE_OK);

			return NULL;
		}

		if (ch == '{')
		{
			// Zrejme zanoreny blok

			file_bufferNext(_parser->buffer);

			_parser->depth++;

			ret_var = parseBlock(_parser, '}');
			_parser->depth = scope;
			VarGarbageScope(_parser);

			if (_parser->error == CC_CODE_OK)
			{
				file_bufferNext(_parser->buffer);

				continue;
			}

			else if (_parser->error < CC_CODE_ERROR)
			{
				// break/continue/return

				return ret_var;
			}

			else
			{
				// nejaka chyba

				return false;
			}

		}

		else if (ch == '/')
		{
			// komentar

			if (!parseSkipComment(_parser))
			{
				return false;
			}

			continue;

		}

		else if (ch == '+')
		{
			// inkrementace promenne
			// ++var;

			if (!parseVarIncrementLeft(_parser, keyword_name, keyword_len))
			{
				return NULL;
			}

			file_bufferNext(_parser->buffer);

			continue;

		}

		else if (ch == '-')
		{
			// dekrementace promenne
			// --var;

			if (!parseVarDecrementLeft(_parser, keyword_name, keyword_len))
			{
				return NULL;
			}

			file_bufferNext(_parser->buffer);

			continue;

		}

		else if (isalpha(ch))
		{
			// promenna, funkce

			parseSetErrorPos(_parser, parseGetPos(_parser));

			if (!parseIdentifier(_parser, keyword_name, &keyword_len))
			{
				return NULL;
			}

			if (keyword_len == 0)
			{
				parseSetError(_parser, CC_CODE_KEYWORD);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return NULL;

			}

			file_bufferSkipSpace(_parser->buffer);

			if (keyword_len == 4 && strncmp(keyword_name, "bool", keyword_len) == 0)
			{
				if (ParseDefineTypeBool(_parser, keyword_name))
				{
					continue;
				}
				else
				{

					return NULL;
				}
			}

			else if (keyword_len == 3 && strncmp(keyword_name, "int", keyword_len) == 0)
			{
				if (ParseDefineTypeInt(_parser))
				{
					continue;
				}
				else
				{
					return NULL;
				}
			}

			else if (keyword_len == 4 && strncmp(keyword_name, "long", keyword_len) == 0)
			{
				if (ParseDefineTypeLong(_parser))
				{
					continue;
				}
				else
				{
					return NULL;
				}
			}

			else if (keyword_len == 5 && strncmp(keyword_name, "float", keyword_len) == 0)
			{
				if (ParseDefineTypeFloat(_parser))
				{
					continue;
				}
				else
				{
					return NULL;
				}
			}

			else if (keyword_len == 4 && strncmp(keyword_name, "char", keyword_len) == 0)
			{
				if (ParseDefineTypeChar(_parser))
				{
					continue;
				}
				else
				{
					return NULL;
				}
			}

			else if (keyword_len == 6 && strncmp(keyword_name, "string", keyword_len) == 0)
			{
				if (ParseDefineTypeString(_parser))
				{
					continue;
				}
				else
				{
					return NULL;
				}
			}

			else if (keyword_len == 6 && strncmp(keyword_name, "delete", keyword_len) == 0)
			{

				if (parseVarDelete(_parser, keyword_name))
				{
					continue;
				}
				else
				{
					return NULL;
				}
			}

			else if (keyword_len == 2 && strncmp(keyword_name, "if", keyword_len) == 0)
			{
				_parser->depth++;
				ret_var = parseIf(_parser);

				_parser->depth = scope;
				VarGarbageScope(_parser);
				if (_parser->error == CC_CODE_RETURN)
				{
					// return
					return ret_var;
				}

				VarDestroy(ret_var);

				if (_parser->error == CC_CODE_BREAK)
				{
					// muzu se nachazet ve smycce
					// break

					return NULL;
				}

				else if (_parser->error == CC_CODE_CONTINUE)
				{
					// muzu se nachazet ve smycce
					// continue

					return NULL;
				}

				else if (_parser->error >= CC_CODE_ERROR)
				{
					return NULL;
				}

				continue;

			}

			else if (keyword_len == 5 && strncmp(keyword_name, "while", keyword_len) == 0)
			{
				_parser->inLoop++;

				ret_var = parseWhile(_parser);
				_parser->inLoop--;

				// fixme proc zde neupravuji 'scope'? V pripade 'IF' to delam..
//				_parser->depth = scope;
//				VarGarbageScope(_parser);
				if (_parser->error == CC_CODE_RETURN)
				{
					return ret_var;
				}

				else if (_parser->error >= CC_CODE_ERROR)
				{
					return NULL;
				}

				continue;

			}

			else if (keyword_len == 5 && strncmp(keyword_name, "break", keyword_len) == 0)
			{
				if (!_parser->inLoop)
				{
					parseSetError(_parser, CC_CODE_OUT_OF_LOOP);
					parseSetErrorPos(_parser, parseGetPos(_parser));

					return NULL;
				}

				parseSetError(_parser, CC_CODE_BREAK);

				return NULL;
			}

			else if (keyword_len == 8 && strncmp(keyword_name, "continue", keyword_len) == 0)
			{
				if (!_parser->inLoop)
				{
					parseSetError(_parser, CC_CODE_OUT_OF_LOOP);
					parseSetErrorPos(_parser, parseGetPos(_parser));

					return NULL;
				}

				parseSetError(_parser, CC_CODE_CONTINUE);

				return NULL;
			}

			else if (keyword_len == 6 && strncmp(keyword_name, "return", keyword_len) == 0)
			{

				parseSetError(_parser, CC_CODE_RETURN);
				ret_var = parseReturnArguments(_parser);

				return ret_var;
			}

			else
			{
				// promenna nebo funkce

				file_bufferSkipSpace(_parser->buffer);
				file_bufferGet(_parser->buffer, &ch);

				if (ch == '=')
				{
					// prirazeni promenne

					if (parseVarAssign(_parser, keyword_name, keyword_len))
					{
						continue;
					}
					else
					{
						return NULL;
					}
				}

				else if (ch == '[')
				{
					parseSetError(_parser, CC_CODE_NOT_IMPLEMENTED);
					parseSetErrorPos(_parser, parseGetPos(_parser));
					return NULL;
				}

				else if (ch == '(')
				{
					// volani funkce, nevracim nic

					var_s *var = funcCall(_parser, keyword_name, keyword_len);

					if (var != NULL)
					{
						VarDestroy(var);
					}

					if (_parser->error >= CC_CODE_ERROR)
					{
						return NULL;
					}

					file_bufferNext(_parser->buffer);
					file_bufferSkipSpace(_parser->buffer);
					file_bufferGet(_parser->buffer, &ch);

					if (ch != ';')
					{
						parseSetError(_parser, CC_CODE_BAD_SYMBOL);
						parseSetErrorPos(_parser, parseGetPos(_parser));

						return NULL;
					}

					file_bufferNext(_parser->buffer);

					continue;
				}

				else if (ch == '+')
				{
					// inkrementace promenne
					// var++;

					if (!parseVarIncrementRight(_parser, keyword_name, keyword_len))
					{
						return NULL;
					}

					file_bufferNext(_parser->buffer);

					continue;

				}

				else if (ch == '-')
				{
					// Decrementace promenne
					// var--;

					if (!parseVarDecrementRight(_parser, keyword_name, keyword_len))
					{
						return NULL;
					}

					file_bufferNext(_parser->buffer);

					continue;

				}

				else if (ch == '/')
				{
					// Deleni promenne
					// var /= 20;

					if (!parseVarDivideRight(_parser, keyword_name, keyword_len))
					{
						return NULL;
					}

					file_bufferNext(_parser->buffer);

					continue;

				}

				else if (ch == '*')
				{
					// Nasobeni promenne
					// var *= 20;

					if (!parseVarMultiplyRight(_parser, keyword_name, keyword_len))
					{
						return NULL;
					}

					file_bufferNext(_parser->buffer);

					continue;

				}

				else
				{
					parseSetError(_parser, CC_CODE_LOGIC);
					parseSetErrorPos(_parser, parseGetPos(_parser));
					return NULL;
				}

			}

		}
		else if (_parser->buffer->fsize > _parser->error_pos)
		{
			CC_BLOCK_DEBUG("ERROR: unexpected symbol 2 '%c'\n", ch);
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			return NULL;
		}
		else
		{

			return NULL;
		}
	}

	parseSetError(_parser, CC_CODE_LOGIC);
	parseSetErrorPos(_parser, parseGetPos(_parser));

	return NULL;

}

bool parseVarAssign(cc_parser_s *_parser, char *_var_name, size_t _var_len)
{
	// funkce priradi hodnotu prommene ktera byla definovana jiz drive

	parseSetErrorPos(_parser, parseGetPos(_parser));

	var_s *var = VarGet(_parser, _var_name, _var_len);
	if (var == NULL)
	{
		parseSetError(_parser, CC_CODE_VAR_NOT_DEFINED);
		CC_VAR_DEBUG("ERROR: undefined variable '%s'!", _var_name);

		return false;
	}

	if (var->type == CC_TYPE_BOOL)
	{
		bool ival = 0;
		if (!parseVarArgsBool(_parser, ';', &ival))
		{
			return false;
		}

		if (!VarValueSetBool(_parser, var, ival))
		{
			return false;
		}

//		file_bufferNext(_parser->buffer);

		return true;

	}

	else if (var->type == CC_TYPE_INT)
	{
		int ival = 0;
		if (!parseVarArgsInt(_parser, ';', &ival))
		{
			return false;
		}

		if (!VarValueSetInt(_parser, var, ival))
		{
			return false;
		}

		file_bufferNext(_parser->buffer);

		return true;

	}

	else if (var->type == CC_TYPE_LONG)
	{
		long val = 0;
		if (!parseVarArgsLong(_parser, ';', &val))
		{
			return false;
		}

		if (!VarValueSetLong(_parser, var, val))
		{
			return false;
		}

		file_bufferNext(_parser->buffer);

		return true;

	}

	else if (var->type == CC_TYPE_FLOAT)
	{
		float fval = 0;
		if (!parseVarArgsFloat(_parser, ';', &fval))
		{
			return false;
		}

		if (!VarValueSetFloat(_parser, var, fval))
		{
			return false;
		}

		file_bufferNext(_parser->buffer);

		return true;

	}

	else if (var->type == CC_TYPE_CHAR)
	{
		char cval = 0;
		if (!parseVarArgsChar(_parser, ';', &cval))
		{
			return false;
		}

		if (!VarValueSetChar(_parser, var, cval))
		{
			return false;
		}

		file_bufferNext(_parser->buffer);

		return true;

	}

	if (var->type == CC_TYPE_STRING)
	{
		size_t fval_len = 50;
		char fval[CC_VALUE_STRING_LEN + 1] = {
				'\0' };
		if (!parseVarArgsString(_parser, ';', fval, &fval_len))
		{
			return false;
		}

		if (!VarValueSetString(_parser, var, fval, fval_len))
		{
			return false;
		}

		file_bufferNext(_parser->buffer);

		return true;

	}

	if (var->type == CC_TYPE_ARRAY)
	{
		parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
		return false;
	}

	else
	{
		parseSetError(_parser, CC_CODE_TYPE_UNKNOWN);
		return false;
	}

}

void parseSetErrorPos(cc_parser_s *_parser, size_t _pos)
{
	_parser->error_pos = _pos;
}

size_t parseGetPos(cc_parser_s *_parser)
{
	return ((size_t) _parser->buffer->offset) + _parser->buffer->fpos - _parser->buffer->length;
}
void parseSetError(cc_parser_s *_parser, cc_code_t _error)
{
	_parser->error = _error;
}

bool parseSkipComment(cc_parser_s *_parser)
{
	char ch = 0;

	file_bufferGet(_parser->buffer, &ch);

	if (ch == '/')
	{

		file_bufferNext(_parser->buffer);
		file_bufferGet(_parser->buffer, &ch);

		if (ch == '/')
		{
			// komentar

#if CC_PRINT_COMMENT
			char buf[CC_COMMENT_LEN + 1] = {
					'\0' };
			size_t len = 0;
#endif

			file_bufferNext(_parser->buffer);
			file_bufferSkipSpace(_parser->buffer);

			while (FILEBUFFER_OK == file_bufferValid(_parser->buffer))
			{

				file_bufferGet(_parser->buffer, &ch);

				if (ch == '\n')
				{
					// konec radku

#if CC_PRINT_COMMENT
					CC_PRINT("\\comment: '%s'\n", buf);
					memset(buf, 0, CC_COMMENT_SIZE);
#endif

					return true;
				}

#if CC_PRINT_COMMENT
				if (len >= CC_COMMENT_SIZE)
				{
					CC_PRINT("\\comment: '%s'\n", buf);
					memset(buf, 0, CC_COMMENT_SIZE);
					len = 0;
				}

				buf[len++] = ch;
#endif

				file_bufferNext(_parser->buffer);
			}

		}

	}

	parseSetErrorPos(_parser, parseGetPos(_parser));
	parseSetError(_parser, CC_CODE_NOT_COMMENT);
	return false;

}

void parseSkipNewLine(cc_parser_s *_parser)
{
	char ch = 0;

	while (FILEBUFFER_OK == file_bufferGet(_parser->buffer, &ch))
	{
		if (ch == '\n' || isspace(ch))
		{
			file_bufferNext(_parser->buffer);
		}
		else
		{
			break;
		}
	}
}

bool parseIdentifier(cc_parser_s *_parser, char *_name, size_t *_name_len)
{

	size_t i = 0;
	char ch = 0;

	file_bufferGet(_parser->buffer, &ch);

	while (isalpha(ch) || isdigit(ch) || ch == '_')
	{
		_name[i++] = ch;
		if (FILEBUFFER_OK != file_bufferNext(_parser->buffer)
				|| FILEBUFFER_OK != file_bufferGet(_parser->buffer, &ch))
		{
			*_name_len = 0;
			return false;
		}
	}

	*_name_len = i;
	return true;
}

bool parseVarDelete(cc_parser_s *_parser, char *_var_name)
{

	file_bufferSkipSpace(_parser->buffer);

	char ch = '\0';

	file_bufferGet(_parser->buffer, &ch);

	if (!isalpha(ch))
	{
		parseSetErrorPos(_parser, parseGetPos(_parser));
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		return false;
	}

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
		return false;
	}

	file_bufferSkipSpace(_parser->buffer);

	file_bufferGet(_parser->buffer, &ch);

	if (ch != ';')
	{
		parseSetErrorPos(_parser, parseGetPos(_parser));
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		return false;
	}

	file_bufferNext(_parser->buffer);

	return VarFindAndDestroy(_parser, var_name, var_name_len);
}

bool parserSkipBlock(cc_parser_s *_parser, char _start_char, char _end_char)
{
	char ch = 0;
	uint8_t depth = 1;

	file_bufferNext(_parser->buffer);

	while (FILEBUFFER_OK == file_bufferValid(_parser->buffer))
	{

		file_bufferGet(_parser->buffer, &ch);

//		if (ch == '/')
//		{
//			// komentar/operator
//
//			if (!parseSkipComment(_parser))
//			{
////				return false;
//			}
//
//			continue;
//		}

		if (ch == _start_char)
		{
			depth++;

			file_bufferNext(_parser->buffer);
			continue;
		}

		else if (ch == _end_char)
		{

			file_bufferNext(_parser->buffer);
			if (depth > 1)
			{
				depth--;
				continue;
			}

			else
			{
				return true;
			}

		}
		file_bufferNext(_parser->buffer);

	}

	parseSetError(_parser, CC_CODE_LOGIC);
	parseSetErrorPos(_parser, parseGetPos(_parser));

	return false;
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
