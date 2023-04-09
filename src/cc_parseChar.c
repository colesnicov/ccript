/*
 * This file is a part of "CCRipt" Embeddable Custom 'C' Like Scripting Language Interpreter
 */

/**
 * @file cc_parseChar.c
 * @brief Implementace funkci pro parsovani typu 'CHAR'.
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

bool ParseDefineTypeChar(cc_parser_s *_parser)
{
	// parse name
	// can be var name, function name,
	//
	// najit konec druheho vyrazu
	// overit jestli za druhym vyrazem je znak:
	//  '=' prirazeni promenne
	//  '(' definice funkce
	//  '[' definice pole
	//  nebo chyba?

	file_bufferSkipSpace(_parser->buffer);

	char ch = '\0';

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
		return false;\

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

		char fval = 0;
		if (!parseVarArgsChar(_parser, ';', &fval))
		{
			return false;
		}

		var_s *var = VarCreate(identifier_name, identifier_len, CC_TYPE_CHAR, _parser->depth);

		if (var == NULL)
		{
			return false;
		}

		if (!VarValueSetChar(_parser, var, fval))
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

		var_s *var = VarCreate(identifier_name, identifier_len, CC_TYPE_CHAR, _parser->depth);
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

		return parseDefineBlock(_parser, CC_TYPE_CHAR, identifier_name, identifier_len);
	}
	else
	{
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;
	}

}

bool ParseValueChar(cc_parser_s *_parser, char *_value, size_t *_value_len)
{

	char ch;
	char ch_temp;

	file_bufferGet(_parser->buffer, &ch);

	if (ch == '\'')
	{
		// prvni znak musi byt apostrof (')

		file_bufferNext(_parser->buffer);
		file_bufferGet(_parser->buffer, &ch);

		ch_temp = ch;

		if (ch == '\\')
		{
			// escape sequence?
			// Nebezpecne znaky jsou:
			// ',\ a kombinace jako
			// '\\'
			// '\''

			file_bufferNext(_parser->buffer);
			file_bufferGet(_parser->buffer, &ch);

			if (ch == '\'')
			{
				// apostrof

				file_bufferNext(_parser->buffer);
				file_bufferGet(_parser->buffer, &ch);

				if (ch == '\'')
				{
					// Konec komandy

					_value[0] = '\'';
					*_value_len = 1;

					return true;
				}

				else
				{
					parseSetError(_parser, CC_CODE_BAD_SYMBOL);
					parseSetErrorPos(_parser, parseGetPos(_parser));

					return false;
				}

			}

			else if (ch == '\\')
			{
				// zpetne lomitko (\)

				file_bufferNext(_parser->buffer);
				file_bufferGet(_parser->buffer, &ch);

				if (ch == '\'')
				{
					// Konec komandy

					_value[0] = '\\';
					*_value_len = 1;
					return true;
				}

				else if (ch == ';')
				{
					// Konec komandy

					_value[0] = ch_temp;
					*_value_len = 1;

					return true;
				}

				else
				{
					parseSetError(_parser, CC_CODE_BAD_SYMBOL);
					parseSetErrorPos(_parser, parseGetPos(_parser));

					return false;
				}

			}

			else if (charin(ch, "nrt"))
			{
				// znak novy radek, return nebo tab (\n,\r,\t)

				ch_temp = ch;

				file_bufferNext(_parser->buffer);
				file_bufferGet(_parser->buffer, &ch);

				if (ch == '\'')
				{
					// Konec komandy

					if (ch_temp == 'n')
					{
						_value[0] = '\n';
					}
					else if (ch_temp == 'r')
					{
						_value[0] = '\r';
					}
					else if (ch_temp == 't')
					{
						_value[0] = '\t';
					}

					*_value_len = 1;
					return true;
				}

				else if (ch == ';')
				{
					// Konec komandy

					CC_VAR_DEBUG("ERROR: Todle je asi spatne. musi nejdrive koncit (') "
							"a az potom muze byt ',' nebo ';' nebo ')' a podobne?\n");

					return false;

					_value[0] = ch_temp;
					*_value_len = 1;

					return true;
				}

				else
				{
					parseSetError(_parser, CC_CODE_BAD_SYMBOL);
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

		else
		{
			// znak

			file_bufferNext(_parser->buffer);
			file_bufferGet(_parser->buffer, &ch);

			if (ch == '\'' && ch_temp != '\\')
			{
				// Konec komandy

//fixme Znova projit takove funkce a overit si spravnost logiky. nektere sekce jsou zbytecne!?

				_value[0] = ch_temp;

				*_value_len = 1;
				return true;
			}

			else
			{

				parseSetError(_parser, CC_CODE_BAD_SYMBOL);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;
			}
		}
	}

	else
	{
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;
	}
}

bool parseVarArgsChar(cc_parser_s *_parser, char _symbol_end, char *_value)
{

	size_t value_len = 0;
	char value_name[CC_VALUE_STRING_LEN + 1] = {
			'\0' };

	size_t fval_temp_len = 0;
	char fval_temp = '\0';

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

	file_bufferGet(_parser->buffer, &ch);

	{
		// muze zacinat pouze pismenem a '

		file_bufferGet(_parser->buffer, &ch);

		if (!isalpha(ch) && ch != '\'')
		{
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));

			return false;
		}
	}

	if (ch == '\'')
	{
		if (!ParseValueChar(_parser, &fval_temp, &fval_temp_len))
		{
			return false;
		}

#if 0 == CC_ASSIGN_EMPTY
		if (fval_temp_len == 0)
		{
			parseSetError(_parser, CC_CODE_KEYWORD);
			parseSetErrorPos(_parser, parseGetPos(_parser));

			return false;
		}
#endif

		file_bufferNext(_parser->buffer);

	}

	else if (ch == '(')
	{

		if (!parseVarArgsChar(_parser, ')', &fval_temp))
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
			// volani funkce ...

			var_s *var = funcCall(_parser, value_name, value_len);

			if (_parser->error > CC_CODE_RETURN)
			{
				VarDestroy(var);
				return false;
			}

			if (var == NULL)
			{

				CC_VAR_DEBUG("ERROR: function '%s' return 'null'.\n", value_name);
				parseSetError(_parser, CC_CODE_LOGIC);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;
			}

			file_bufferNext(_parser->buffer);
			file_bufferSkipSpace(_parser->buffer);
			file_bufferGet(_parser->buffer, &ch);

			if (ch != ';')
			{
				parseSetError(_parser, CC_CODE_BAD_SYMBOL);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				VarDestroy(var);
				return false;
			}

			if (var->type == CC_TYPE_CHAR)
			{
				value_len = 1;
				if (!VarValueGetChar(_parser, var, &fval_temp))
				{
					VarDestroy(var);
					return false;
				}
			}

			else
			{

				CC_VAR_DEBUG("ERROR: function '%s' return bad type.\n", value_name);
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

			if (!VarValueGetChar(_parser, var, &fval_temp))
			{
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
		//konec vety

		if (value_len + fval_temp_len > CC_VALUE_STRING_LEN)
		{
			parseSetError(_parser, CC_CODE_STRING_TOO_LONG);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;
		}

		*_value = fval_temp;

		file_bufferNext(_parser->buffer);
		return true;

	}

	else if (ch == '(')
	{
		parseSetError(_parser, CC_CODE_LOGIC);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;

	}

	else if (ch == '[')
	{
		parseSetError(_parser, CC_CODE_LOGIC);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;
	}

	else
	{
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;
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
