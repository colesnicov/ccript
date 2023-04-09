/*
 * This file is a part of "CCRipt" Embeddable Custom 'C' Like Scripting Language Interpreter
 */

/**
 * @file cc_parseString.c
 * @brief Implementace funkci pro parsovani typu 'STRING'.
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

#include <ccript/cc_configs.h>
#include <ccript/cc_function.h>
#include <ccript/cc_parser.h>
#include <ccript/cc_types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "ccript/cc_var.h"

bool ParseDefineTypeString(cc_parser_s *_parser)
{
	char ch = '\0';

	file_bufferSkipSpace(_parser->buffer);

	file_bufferGet(_parser->buffer, &ch);

	if (!isalpha(ch))
	{
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;
	}

	char identifier_name[CC_KEYWORD_LEN + 1] = {
			'\0' };
	size_t identifier_len = 0;

	if (!parseIdentifier(_parser, identifier_name, &identifier_len))
	{
		return false;
	}

	if (identifier_len == 0)
	{
		parseSetError(_parser, CC_CODE_KEYWORD);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;

	}

	file_bufferSkipSpace(_parser->buffer);

	file_bufferGet(_parser->buffer, &ch);

	if (ch == '=')
	{
		// definice a prirazeni promenne
		// ziskej nazev promenne (uz mam: 'keyword_position')
		// over neexistenci promenne
		// ziskej hodnotu prirazeni, over co nasleduje po vyrazu:
		// ';' konec vety
		// 'operator (+-/*)' scitani nekolika cisel
		// '(' volani funkce ktera vraci numeric

		char fval[CC_VALUE_STRING_LEN + 1] = {
				'\0' };
		size_t len = 0;
		if (!parseVarArgsString(_parser, ';', fval, &len))
		{
			return false;
		}

		var_s *var = VarCreate(identifier_name, identifier_len, CC_TYPE_STRING, _parser->depth);

		if (var == NULL)
		{
			return false;
		}

		if (!VarValueSetString(_parser, var, fval, len))
		{
			VarDestroy(var);
			return false;
		}

		if (!VarStore(_parser, var, false))
		{
			VarDestroy(var);
			return false;
		}

		file_bufferNext(_parser->buffer);

		return true;

	}
	else if (ch == ';')
	{
		// definice promenne bez prirazeni

		var_s *var = VarCreate(identifier_name, identifier_len, CC_TYPE_STRING, _parser->depth);
		if (var == NULL)
		{
			return false;
		}

		if (!VarStore(_parser, var, false))
		{
			VarDestroy(var);
			return false;
		}

		file_bufferNext(_parser->buffer);

		return true;

	}

	else if (ch == '(')
	{
		// definice funkce

		return parseDefineBlock(_parser, CC_TYPE_STRING, identifier_name, identifier_len);
	}
	else
	{
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;
	}

}

bool ParseValueString(cc_parser_s *_parser, char *_value, size_t *_value_len)
{

	size_t len = 0;
	char ch;
	char ch_last = 0;

	file_bufferSkipSpace(_parser->buffer);
	file_bufferGet(_parser->buffer, &ch);

	if (ch == '"')
	{

//		memset(_value, '\0', CC_VALUE_STRING_LEN);
		file_bufferNext(_parser->buffer);

		while (FILEBUFFER_OK == file_bufferValid(_parser->buffer))
		{

			if (len >= CC_VALUE_STRING_LEN)
			{
				parseSetError(_parser, CC_CODE_STRING_TOO_LONG);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;
			}

			file_bufferGet(_parser->buffer, &ch);

			if (ch_last == '\\')
			{

				if (ch == '"')
				{
					// Escape zavorka (")

					len--;
					_value[len++] = ch;
					ch_last = 0;
				}

				else if (ch == '\\')
				{
					// Escape lomitko (\)

					len--;
					_value[len++] = ch;
					ch_last = 0;
				}

				else if (charin(ch, "ntr"))
				{
					// Escape lomitko (\)

					_value[len++] = ch;
					ch_last = 0;
				}

				else
				{
					parseSetErrorPos(_parser, parseGetPos(_parser));
					parseSetError(_parser, CC_CODE_BAD_SYMBOL);
					return false;
				}

			}

			else if (ch_last != '\\' && ch == '"')
			{
				// Konec komandy

				file_bufferNext(_parser->buffer);

				*_value_len = len;
				return true;
			}

			else if (ch == '\n' || ch == ';')
			{
				parseSetError(_parser, CC_CODE_BAD_SYMBOL);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;
			}

			else
			{

				_value[len++] = ch;
				ch_last = ch;
			}

			file_bufferNext(_parser->buffer);
		}
		parseSetError(_parser, CC_CODE_LOGIC);
		parseSetErrorPos(_parser, parseGetPos(_parser));

		return 0;

	}
	else if (ch == '\'')
	{
		memset(_value, '\0', CC_VALUE_STRING_LEN);
		return ParseValueChar(_parser, _value, _value_len);

	}
	else
	{
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;
	}

}

bool parseVarArgsString(cc_parser_s *_parser, char _symbol_end, char *_value,
		size_t *_value_len)
{

	size_t value_len = 0;
	char value_name[CC_VALUE_STRING_LEN + 1] = {
			'\0' };

	size_t fval_temp_len = 0;
	char fval_temp[CC_VALUE_STRING_LEN + 1] = {
			'\0' };

	char ch = 0;

	file_bufferNext(_parser->buffer);

	file_bufferGet(_parser->buffer, &ch);
	if (ch == '\n')
	{
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;
	}

	file_bufferSkipSpace(_parser->buffer);

	file_bufferGet(_parser->buffer, &ch);
	if (ch == '\n')
	{
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;
	}

//	file_bufferGet(_parser->buffer, &ch);

	// muze zacinat pouze pismenem, ' a "

	if (!isalpha(ch) && !charin(ch, "'\""))
	{
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));

		return false;
	}

	while (FILEBUFFER_OK == file_bufferValid(_parser->buffer))
	{

		memset(fval_temp, '\0', CC_VALUE_STRING_LEN);

		parseSkipNewLine(_parser);
		file_bufferGet(_parser->buffer, &ch);

		if (ch == '"' || ch == '\'')
		{
			if (!ParseValueString(_parser, fval_temp, &fval_temp_len))
			{
				return false;
			}

			if (fval_temp_len == 0)
			{
				parseSetError(_parser, CC_CODE_KEYWORD);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;
			}
//			file_bufferNext(_parser->buffer);

		}

		else if (ch == '(')
		{
			if (!parseVarArgsString(_parser, ')', fval_temp, &fval_temp_len))
			{
				return false;
			}
		}

		else
		{
			// promenna nebo funkce

			if (!parseIdentifier(_parser, value_name, &value_len))
			{
				return false;
			}

			if (value_len == 0)
			{
				parseSetError(_parser, CC_CODE_KEYWORD);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;

			}

			parseSetErrorPos(_parser, parseGetPos(_parser));
			file_bufferSkipSpace(_parser->buffer);
			file_bufferGet(_parser->buffer, &ch);

			if (ch == '[')
			{
				parseSetError(_parser, CC_CODE_BAD_SYMBOL);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;
			}

			else if (ch == '(')
			{
				// volani funkce
				size_t pos = parseGetPos(_parser);
				var_s *var = funcCall(_parser, value_name, value_len);

				if (_parser->error > CC_CODE_RETURN)
				{
					VarDestroy(var);
					return false;
				}

				if (var == NULL)
				{
					parseSetError(_parser, CC_CODE_LOGIC);
					parseSetErrorPos(_parser, pos);

					CC_VAR_DEBUG("ERROR: function '%s' return 'null'.\n", value_name);
					return false;
				}

				file_bufferNext(_parser->buffer);
				file_bufferSkipSpace(_parser->buffer);
				file_bufferGet(_parser->buffer, &ch);

				if (ch != ';' && !charin(ch, "+"))
				{
					parseSetError(_parser, CC_CODE_BAD_SYMBOL);
					parseSetErrorPos(_parser, parseGetPos(_parser));
					VarDestroy(var);
					return false;
				}

				if (var->type == CC_TYPE_STRING)
				{
					if (!VarValueGetString(_parser, var, fval_temp, &fval_temp_len))
					{
						VarDestroy(var);
						return false;
					}
				}

				else if (var->type == CC_TYPE_CHAR)
				{
					value_len = 1;
					if (!VarValueGetChar(_parser, var, &fval_temp[0]))
					{
						VarDestroy(var);
						return false;
					}
				}

				else
				{
					CC_VAR_DEBUG("ERROR: function '%s' return bad type.\n", value_name);
					parseSetError(_parser, CC_CODE_LOGIC);
					parseSetErrorPos(_parser, parseGetPos(_parser));

					VarDestroy(var);
					return false;
				}

				VarDestroy(var);

			}

			else if (ch == ';')
			{

				var_s *var = VarGet(_parser, value_name, value_len);
				if (var == NULL)
				{
					return false;
				}

				if (var->type == CC_TYPE_CHAR)
				{
					if (!VarValueGetChar(_parser, var, &fval_temp[0]))
					{
						return false;
					}
					fval_temp_len = 1;
				}
				else if (var->type == CC_TYPE_STRING)
				{
					if (!VarValueGetString(_parser, var, fval_temp, &fval_temp_len))
					{
						return false;
					}
				}
				else
				{
					parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
					parseSetErrorPos(_parser, parseGetPos(_parser));
					return false;
				}

			}

			else
			{
				parseSetError(_parser, CC_CODE_BAD_SYMBOL);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;
			}

		}

		file_bufferSkipSpace(_parser->buffer);

		file_bufferGet(_parser->buffer, &ch);

		if (ch == _symbol_end)
		{

			if (value_len + fval_temp_len > CC_VALUE_STRING_LEN)
			{
				parseSetError(_parser, CC_CODE_STRING_TOO_LONG);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;
			}

			memcpy(_value, fval_temp, fval_temp_len);

			*_value_len = fval_temp_len;

			file_bufferNext(_parser->buffer);
			return true;

		}

		else if (ch == '(')
		{
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;

		}

		else if (ch == '[')
		{
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;
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
///
///
///
