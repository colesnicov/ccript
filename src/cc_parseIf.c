/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file cc_parseIf.c
 * @brief Implementace funkci pro parsovani podminky 'IF'.
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

#include <ccript/cc_block.h>
#include <ccript/ccript.h>
#include "ccript/cc_parseIf.h"
#include "ccript/cc_configs.h"
#include "ccript/cc_function.h"
#include "ccript/cc_parser.h"
#include "ccript/cc_types.h"
#include "ccript/cc_var.h"
#include "ccript/common.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/**
 * @var static char *m_excluded_sym
 * @brief Operatory zakazane na zacatku logickych vyrazu.
 */
static char *m_excluded_sym = "=<>&|";

void garbageIf(var_s **args, uint8_t *args_count)
{

	for (uint8_t i = 0; i < *args_count; i++)
	{
		CC_VAR_DEBUG("DEBUG: destroy var: '%s'\n", args[i]->name);
		VarDestroy(args[i]);
	}
	*args_count = 0;
}

bool parseIfArguments(cc_parser_s *_parser, float *_cond_value, char _end_sym)
{

	char ch;

	float cond_result_right;
	float cond_result;

	file_bufferNext(_parser->buffer);
	file_bufferSkipSpace(_parser->buffer);

	while (FILEBUFFER_OK == file_bufferGet(_parser->buffer, &ch))
	{

//		for (uint8_t w = 0; w < 5; w++)
//		{
//			if (ch == m_excluded[w])
//			{
//				parseSetError(_parser, CC_CODE_BAD_SYMBOL);
//				parseSetErrorPos(_parser, parseGetPos(_parser));
//				return false;
//			}
//		}

		if (false == charin(ch, m_excluded_sym))
		{
			if (!parseIfCond(_parser, &cond_result))
			{
				return false;
			}

			file_bufferSkipSpace(_parser->buffer);
			file_bufferGet(_parser->buffer, &ch);
			if (_end_sym == ch)
			{
				// printf("%s: falesne vynoreni (tady [ne]posouvat znak?)\n", CC_LOG_TAG);
			}
		}

		if (ch == '&')
		{

			file_bufferNext(_parser->buffer);
			file_bufferGet(_parser->buffer, &ch);
			if (ch == '&')
			{

				if (!parseIfArguments(_parser, &cond_result_right, _end_sym))
				{
					return false;
				}

				*_cond_value = (float) ((bool) cond_result && (bool) cond_result_right);

				return true;
			}

			else
			{
				parseSetError(_parser, CC_CODE_BAD_SYMBOL);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;
			}
		}

		else if (ch == '|')
		{

			file_bufferNext(_parser->buffer);
			file_bufferGet(_parser->buffer, &ch);
			if (ch == '|')
			{

				if (!parseIfArguments(_parser, &cond_result_right, _end_sym))
				{
					return false;
				}

				*_cond_value = (float) ((bool) cond_result || (bool) cond_result_right);
				return true;
			}

			else
			{
				parseSetError(_parser, CC_CODE_BAD_SYMBOL);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;
			}
		}

		else if (ch == '=')
		{
			file_bufferNext(_parser->buffer);
			file_bufferGet(_parser->buffer, &ch);

			if (ch == '=')
			{
				if (!parseIfArguments(_parser, &cond_result_right, _end_sym))
				{
					return false;
				}

				*_cond_value = (float) (cond_result == cond_result_right);
				return true;
			}
		}

		else if (ch == '!')
		{
			file_bufferNext(_parser->buffer);
			file_bufferGet(_parser->buffer, &ch);

			if (ch == '=')
			{
				if (!parseIfArguments(_parser, &cond_result_right, _end_sym))
				{
					return false;
				}

				*_cond_value = (float) (cond_result != cond_result_right);
				return true;
			}
		}

		else if (ch == '<')
		{
			size_t pos_offset = parseGetPos(_parser);

			file_bufferNext(_parser->buffer);
			file_bufferGet(_parser->buffer, &ch);

			if (ch == '=')
			{
				if (!parseIfArguments(_parser, &cond_result_right, _end_sym))
				{
					return false;
				}

				*_cond_value = (float) (cond_result <= cond_result_right);
				return true;
			}

			_parser->buffer->offset = pos_offset;
			file_bufferReload(_parser->buffer);

			if (!parseIfArguments(_parser, &cond_result_right, _end_sym))
			{
				return false;
			}

			*_cond_value = (float) (cond_result < cond_result_right);
			return true;

		}

		else if (ch == '>')
		{
			size_t pos_offset = parseGetPos(_parser);

			file_bufferNext(_parser->buffer);
			file_bufferGet(_parser->buffer, &ch);

			if (ch == '=')
			{
				if (!parseIfArguments(_parser, &cond_result_right, _end_sym))
				{
					return false;
				}

				*_cond_value = (float) (cond_result >= cond_result_right);
				return true;
			}
			else
			{
				_parser->buffer->offset = pos_offset;
				file_bufferReload(_parser->buffer);

				if (!parseIfArguments(_parser, &cond_result_right, _end_sym))
				{
					return false;
				}

				*_cond_value = (float) (cond_result > cond_result_right);
				return true;
			}

		}

#if CC_IF_COND_MATH
		else if (ch == '-')
		{

			file_bufferNext(_parser->buffer);
			file_bufferSkipSpace(_parser->buffer);
			if (!parseIfCond(_parser, &cond_result_right))
			{
				return false;
			}

			file_bufferSkipSpace(_parser->buffer);
			file_bufferGet(_parser->buffer, &ch);
			if (ch == _end_sym)
			{

				file_bufferNext(_parser->buffer);
				file_bufferSkipSpace(_parser->buffer);
				*_cond_value = cond_result - cond_result_right;
				return true;
			}

			cond_result -= cond_result_right;
			continue;
		}

		else if (ch == '+')
		{
			file_bufferNext(_parser->buffer);
			file_bufferSkipSpace(_parser->buffer);
			if (!parseIfCond(_parser, &cond_result_right))
			{
				return false;
			}

			file_bufferSkipSpace(_parser->buffer);
			file_bufferGet(_parser->buffer, &ch);
			if (ch == _end_sym)
			{

				file_bufferNext(_parser->buffer);
				file_bufferSkipSpace(_parser->buffer);
				*_cond_value = cond_result + cond_result_right;
				return true;
			}

			cond_result += cond_result_right;
			continue;
		}

		else if (ch == '/')
		{
			file_bufferNext(_parser->buffer);
			file_bufferSkipSpace(_parser->buffer);
			if (!parseIfCond(_parser, &cond_result_right))
			{
				return false;
			}

			if (cond_result_right == 0)
			{

				parseSetError(_parser, CC_CODE_ZERO_DIVIDED);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;
			}

			file_bufferSkipSpace(_parser->buffer);
			file_bufferGet(_parser->buffer, &ch);
			if (ch == _end_sym)
			{

				file_bufferNext(_parser->buffer);
				file_bufferSkipSpace(_parser->buffer);
				*_cond_value = cond_result / cond_result_right;
				return true;
			}

			cond_result /= cond_result_right;
			continue;

		}

		else if (ch == '*')
		{
			file_bufferNext(_parser->buffer);
			file_bufferSkipSpace(_parser->buffer);
			if (!parseIfCond(_parser, &cond_result_right))
			{
				return false;
			}
			file_bufferSkipSpace(_parser->buffer);
			file_bufferGet(_parser->buffer, &ch);
			if (ch == _end_sym)
			{

				file_bufferNext(_parser->buffer);
				file_bufferSkipSpace(_parser->buffer);
				*_cond_value = cond_result * cond_result_right;
				return true;
			}

			cond_result *= cond_result_right;
			continue;

		}
#endif

		else if (ch == _end_sym)
		{
			// konec argumentu

			file_bufferNext(_parser->buffer);
			file_bufferSkipSpace(_parser->buffer);

			*_cond_value = cond_result;

			return true;
		}
		else
		{
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;
		}

	}

	parseSetError(_parser, CC_CODE_LOGIC);
	parseSetErrorPos(_parser, parseGetPos(_parser));

	return false;
}

bool parseIfCond(cc_parser_s *_parser, float *_cond_value)
{

	char ch = 0;

	float cond_result;

	bool negatted = false;

	while (FILEBUFFER_OK == file_bufferGet(_parser->buffer, &ch))
	{
		if (ch == '!')
		{

			negatted = true;
			file_bufferNext(_parser->buffer);
			file_bufferSkipSpace(_parser->buffer);

		}

		if (ch == '(')
		{

			// zanoreni
			if (!parseIfArguments(_parser, &cond_result, ')'))
			{

				return false;
			}

		}

		else
		{
			if (!parseIfArg(_parser, &cond_result))
			{
				return false;
			}
		}

		if (negatted)
		{
			if (cond_result)
			{
				*_cond_value = 0.0;
			}
			else
			{
				*_cond_value = 1.0;
			}
		}
		else
		{
			*_cond_value = cond_result;
		}
		return true;
	}

	parseSetError(_parser, CC_CODE_IO);
	parseSetErrorPos(_parser, parseGetPos(_parser));

	return false;
}

bool parseIfArg(cc_parser_s *_parser, float *_arg)
{

	char ch;

	file_bufferGet(_parser->buffer, &ch);

	if (isdigit(ch) || ch == '-')
	{
		char value[CC_VALUE_NUMERIC_LEN + 1] = {
				'\0' };

		size_t value_len = 0;
		bool is_bool = false;

		if (!parseValueFloat(_parser, value, &value_len, &is_bool))
		{
			return false;
		}

		if (value_len == 0)
		{
			parseSetError(_parser, CC_CODE_KEYWORD);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;

		}

		*_arg = (float) atof(value);
		return true;
	}

	else if (isalpha(ch))
	{
		// promenna nebo funkce

		char var_name[CC_KEYWORD_LEN + 1] = {
				'\0' };
		size_t var_name_len = 0;

		parseSetErrorPos(_parser, parseGetPos(_parser));

		if (!parseIdentifier(_parser, var_name, &var_name_len))
		{
			return false;
		}

		if (var_name_len == 0)
		{
			parseSetError(_parser, CC_CODE_KEYWORD);

			return false;
		}

		var_s *_var = NULL;
		bool var_protected = true;

		if (var_name_len == 4 && strncmp(var_name, "true", var_name_len) == 0)
		{

			*_arg = 1.0f;
			return true;
		}

		else if (var_name_len == 5 && strncmp(var_name, "false", var_name_len) == 0)
		{

			*_arg = 0.0f;
			return true;
		}

		// funkce nebo promenna

		file_bufferSkipSpace(_parser->buffer);

		file_bufferGet(_parser->buffer, &ch);

		if (ch == '[')
		{
			// pole ...

			parseSetError(_parser, CC_CODE_NOT_IMPLEMENTED);
			parseSetErrorPos(_parser, parseGetPos(_parser));

			return false;
		}

		else if (ch == '(')
		{
			// volani funkce ...

			_var = funcCall(_parser, var_name, var_name_len);

			if (_parser->error > CC_CODE_RETURN)
			{
				VarDestroy(_var);
				return false;
			}

			file_bufferNext(_parser->buffer);
			file_bufferSkipSpace(_parser->buffer);

			var_protected = false;

		}

		else
		{
			// promenna

			_var = VarGet(_parser, var_name, var_name_len);

			var_protected = true;

		}

		if (_var == NULL)
		{
			return false;
		}

		bool ret = true;

		if (_var->type == (cc_type_t) CC_TYPE_BOOL)
		{
			bool _bool = false;

			if (VarValueGetBool(_parser, _var, &_bool))
			{
				*_arg = (float) _bool;
			}
			else
			{
				ret = false;
			}

		}
		else if (_var->type == CC_TYPE_CHAR)
		{
			char _char = 0;

			if (VarValueGetChar(_parser, _var, &_char))
			{
				*_arg = (float) _char;
			}
			else
			{
				ret = false;
			}
		}
		else if (_var->type == CC_TYPE_FLOAT)
		{

			if (!VarValueGetFloat(_parser, _var, _arg))
			{
				ret = false;
			}
		}

		else if (_var->type == CC_TYPE_INT)
		{
			int _int = 0;

			if (VarValueGetInt(_parser, _var, &_int))
			{
				*_arg = (float) _int;
			}
			else
			{
				ret = false;
			}

		}

		else if (_var->type == CC_TYPE_LONG)
		{
			long _int = 0;

			if (VarValueGetLong(_parser, _var, &_int))
			{
				*_arg = (float) _int;
			}
			else
			{
				ret = false;
			}

		}

		else
		{
			parseSetError(_parser, CC_CODE_ARGS_BAD_TYPE);
			parseSetErrorPos(_parser, parseGetPos(_parser));

			ret = false;
		}

		if (var_protected == false)
		{
			VarDestroy(_var);
		}

		return ret;
	}

	else if (ch == '(')
	{

		return parseIfArguments(_parser, _arg, ')');
	}

	else
	{
		// chyba syntaxe

		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;
	}

}

var_s* parseIf(cc_parser_s *_parser)
{
	char ch = 0;

	parseSkipNewLine(_parser);

	file_bufferGet(_parser->buffer, &ch);

	if (ch != '(')
	{
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return NULL;
	}

	float cond_passed = 0.0;

	if (!parseIfArguments(_parser, &cond_passed, ')'))
	{
		return NULL;
	}

	parseSkipNewLine(_parser);

	file_bufferGet(_parser->buffer, &ch);

	if (ch != '{')
	{
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return NULL;
	}

	if ((bool) cond_passed)
	{
		file_bufferNext(_parser->buffer);

		var_s *ret_var = parseBlock(_parser, '}');

		if (_parser->error == CC_CODE_RETURN)
		{

			return ret_var;
		}

		VarDestroy(ret_var);

		if (_parser->error == CC_CODE_BREAK)
		{

			return NULL;
		}

		else if (_parser->error == CC_CODE_CONTINUE)
		{

			return NULL;
		}

		else if (_parser->error >= CC_CODE_ERROR)
		{
			return NULL;
		}

		file_bufferNext(_parser->buffer);
		parseSkipNewLine(_parser);

		char _name[CC_VALUE_STRING_LEN + 1] = {
				'\0' };

		size_t pos_offset = 0;
		while (FILEBUFFER_OK == file_bufferValid(_parser->buffer))
		{

			file_bufferGet(_parser->buffer, &ch);

			if (ch == 'e')
			{
				pos_offset = parseGetPos(_parser);

				memset(_name, '\0', sizeof(char) * CC_VALUE_STRING_LEN);

				size_t ll = 0;
				if (!parseIdentifier(_parser, _name, &ll))
				{
					return NULL;
				}

				if (ll == 4 && strncmp(_name, "elif", ll) == 0)
				{
					// elif, preskocit
					parseSkipNewLine(_parser);
					if (!parserSkipBlock(_parser, '(', ')'))
					{
						return NULL;
					}

					parseSkipNewLine(_parser);
					if (!parserSkipBlock(_parser, '{', '}'))
					{
						return NULL;
					}

					parseSkipNewLine(_parser);

					continue;
				}

				else if (ll == 4 && strncmp(_name, "else", ll) == 0)
				{
					// else, preskocit
					parseSkipNewLine(_parser);
					if (!parserSkipBlock(_parser, '{', '}'))
					{
						return NULL;
					}

					parseSkipNewLine(_parser);

					return NULL;
				}

				else
				{
					// else neni. konec
					// vracim buffer do pozice pred hledanim

					_parser->buffer->offset = pos_offset;

					file_bufferReload(_parser->buffer);

					return NULL;

				}
			}

			else
			{
				// else neni. konec
				return NULL;
			}
		}

		parseSetError(_parser, CC_CODE_LOGIC);
		parseSetErrorPos(_parser, parseGetPos(_parser));

		return NULL;
	}
	else
	{
		//	preskocit blok if

		if (!parserSkipBlock(_parser, '{', '}'))
		{
			return NULL;
		}

		parseSkipNewLine(_parser);

		file_bufferGet(_parser->buffer, &ch);

		if (ch == 'e')
		{

			size_t pos_offset = parseGetPos(_parser);
			char _name[51] = {
					'\0' };

			size_t ll = 0;
			if (!parseIdentifier(_parser, _name, &ll))
			{
				return NULL;
			}

			if (ll == 4 && strncmp(_name, "elif", ll) == 0)
			{
				var_s *ret_var = parseIf(_parser);

				if (_parser->error == CC_CODE_RETURN)
				{
					return ret_var;
				}

				VarDestroy(ret_var);

				if (_parser->error == CC_CODE_BREAK)
				{
					if (0 == _parser->inLoop)
					{
						parseSetError(_parser, CC_CODE_OUT_OF_LOOP);
						parseSetErrorPos(_parser, parseGetPos(_parser));
					}

					return NULL;
				}

				else if (_parser->error == CC_CODE_CONTINUE)
				{
					return NULL;
				}

				else if (_parser->error >= CC_CODE_ERROR)
				{
					return NULL;
				}

				return NULL;
			}

			else if (ll == 4 && strncmp(_name, "else", ll) == 0)
			{
				parseSkipNewLine(_parser);

				file_bufferGet(_parser->buffer, &ch);

				if (ch != '{')
				{
					parseSetError(_parser, CC_CODE_BAD_SYMBOL);
					parseSetErrorPos(_parser, parseGetPos(_parser));

					return NULL;
				}

				file_bufferNext(_parser->buffer);

				_parser->depth++;
				var_s *ret_var = parseBlock(_parser, '}');

				if (_parser->error == CC_CODE_RETURN)
				{
					return ret_var;
				}

				VarDestroy(ret_var);

				if (_parser->error == CC_CODE_BREAK)
				{
					if (0 == _parser->inLoop)
					{
						parseSetError(_parser, CC_CODE_OUT_OF_LOOP);
						parseSetErrorPos(_parser, parseGetPos(_parser));
					}

					return NULL;
				}

				else if (_parser->error == CC_CODE_CONTINUE)
				{
					return NULL;
				}

				else if (_parser->error >= CC_CODE_ERROR)
				{
					return NULL;
				}

				file_bufferNext(_parser->buffer);
				parseSkipNewLine(_parser);

				return NULL;

			}
			else
			{
				// neni 'else', vratit buffer do pozice pred hledanim

				_parser->buffer->offset = pos_offset;

				file_bufferReload(_parser->buffer);

				parseSkipNewLine(_parser);

				return NULL;
			}
		}
		else
		{
			// neni else/elif

			return NULL;
		}

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
