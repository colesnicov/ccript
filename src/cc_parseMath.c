/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file cc_parseMath.c
 * @brief Implementace matematickych funkci (var++,--var,var/=,...) pro promenne 'INT' a 'FLOAT'.
 *
 * @version 1b1
 * @date 26.06.2022
 *
 * @author Denis Colesnicov <eugustus@gmail.com>
 *
 * @copyright MIT License
 *
 */

#include <filebuffer/filebuffer.h>
#include "ccript/cc_configs.h"
#include "ccript/cc_function.h"
#include "ccript/cc_parser.h"
#include "ccript/cc_types.h"
#include "ccript/cc_var_ext.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "ccript/cc_var.h"

#define OP_SUM	1
#define OP_SUB	2
#define OP_MUL	3
#define OP_DIV	4

bool parseVarDecrementLeft(parser_s *_parser, char *_var_name, size_t _var_name_len)
{

	// dekrementace promenne?
	// --var;

	char ch = 0;

	file_bufferGet(_parser->buffer, &ch);

	if (ch == '-')
	{

		file_bufferNext(_parser->buffer);
		file_bufferSkipSpace(_parser->buffer);
		file_bufferGet(_parser->buffer, &ch);

		if (ch == '-')
		{
			// Dekrementace promenne:
			// --var;

			file_bufferNext(_parser->buffer);
			file_bufferSkipSpace(_parser->buffer);

			if (!parseIdentifier(_parser, _var_name, &_var_name_len))
			{
				return false;
			}

			if (_var_name_len == 0)
			{
				parseSetError(_parser, CC_CODE_KEYWORD);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;

			}

			file_bufferSkipSpace(_parser->buffer);
			file_bufferGet(_parser->buffer, &ch);

			if (ch != ';')
			{
				parseSetError(_parser, CC_CODE_BAD_SYMBOL);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return 0;
			}

			var_s *var = VarGet(_parser, _var_name, _var_name_len);
			if (var == NULL)
			{
				return 0;
			}

			if (var->type == CC_TYPE_FLOAT)
			{
				float value = 0.0f;
				if (!VarValueGetFloat(_parser, var, &value))
				{
					return 0;
				}

				value -= 1;

				if (!VarValueSetFloat(_parser, var, value))
				{
					return 0;
				}

				return true;

			}

			else if (var->type == CC_TYPE_INT)
			{
				int value = 0;
				if (!VarValueGetInt(_parser, var, &value))
				{
					return 0;
				}

				value -= 1;

				if (!VarValueSetInt(_parser, var, value))
				{
					return 0;
				}

				return true;

			}

			else if (var->type == CC_TYPE_LONG)
			{
				long value = 0;
				if (!VarValueGetLong(_parser, var, &value))
				{
					return 0;
				}

				value -= 1;

				if (!VarValueSetLong(_parser, var, value))
				{
					return 0;
				}

				return true;

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
			parseSetError(_parser, CC_CODE_LOGIC);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return 0;
		}

	}

	parseSetError(_parser, CC_CODE_LOGIC);
	parseSetErrorPos(_parser, parseGetPos(_parser));
	return false;
}

bool parseVarIncrementLeft(parser_s *_parser, char *_var_name, size_t _var_name_len)
{

	// Inkrementace promenne?
	// ++var;?

	char ch = 0;
	file_bufferGet(_parser->buffer, &ch);

	if (ch == '+')
	{

		file_bufferNext(_parser->buffer);
		file_bufferSkipSpace(_parser->buffer);
		file_bufferGet(_parser->buffer, &ch);

		if (ch == '+')
		{
			// Inkrementace promenne:
			// ++var;

			file_bufferNext(_parser->buffer);
			file_bufferSkipSpace(_parser->buffer);

			if (!parseIdentifier(_parser, _var_name, &_var_name_len))
			{
				return false;
			}

			if (_var_name_len == 0)
			{
				parseSetError(_parser, CC_CODE_KEYWORD);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;

			}

			file_bufferSkipSpace(_parser->buffer);
			file_bufferGet(_parser->buffer, &ch);

			if (ch != ';')
			{
				parseSetError(_parser, CC_CODE_BAD_SYMBOL);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return 0;
			}

			var_s *var = VarGet(_parser, _var_name, _var_name_len);
			if (var == NULL)
			{
				return 0;
			}

			if (var->type == CC_TYPE_FLOAT)
			{
				float value = 0.0f;
				if (!VarValueGetFloat(_parser, var, &value))
				{
					return 0;
				}

				value += 1;

				if (!VarValueSetFloat(_parser, var, value))
				{
					return 0;
				}

				return true;

			}

			else if (var->type == CC_TYPE_INT)
			{
				int value = 0;
				if (!VarValueGetInt(_parser, var, &value))
				{
					return 0;
				}

				value += 1;

				if (!VarValueSetInt(_parser, var, value))
				{
					return 0;
				}

				return true;

			}

			else if (var->type == CC_TYPE_LONG)
			{
				long value = 0;
				if (!VarValueGetLong(_parser, var, &value))
				{
					return 0;
				}

				value += 1;

				if (!VarValueSetLong(_parser, var, value))
				{
					return 0;
				}

				return true;

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
			parseSetError(_parser, CC_CODE_LOGIC);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return 0;
		}

	}

	parseSetError(_parser, CC_CODE_LOGIC);
	parseSetErrorPos(_parser, parseGetPos(_parser));
	return false;
}

bool parseVarDecrementRight(parser_s *_parser, char *_var_name, size_t _var_name_len)
{
	char ch = 0;
	file_bufferGet(_parser->buffer, &ch);

	if (ch == '-')
	{
		// Decrementace promenne?
		// var--;

		file_bufferNext(_parser->buffer);
		file_bufferSkipSpace(_parser->buffer);
		file_bufferGet(_parser->buffer, &ch);

		if (ch == '-')
		{
			// Decrementace promenne:
			// var--;

			file_bufferNext(_parser->buffer);
			file_bufferSkipSpace(_parser->buffer);
			file_bufferGet(_parser->buffer, &ch);

			if (ch != ';')
			{
				parseSetError(_parser, CC_CODE_BAD_SYMBOL);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;
			}

			var_s *var = VarGet(_parser, _var_name, _var_name_len);
			if (var == NULL)
			{
				return 0;
			}

			if (var->type == CC_TYPE_FLOAT)
			{
				float value = 0.0f;
				if (!VarValueGetFloat(_parser, var, &value))
				{
					return 0;
				}

				value -= 1;

				if (!VarValueSetFloat(_parser, var, value))
				{
					return 0;
				}

				return true;

			}

			else if (var->type == CC_TYPE_INT)
			{
				int value = 0;
				if (!VarValueGetInt(_parser, var, &value))
				{
					return 0;
				}

				value -= 1;

				if (!VarValueSetInt(_parser, var, value))
				{
					return 0;
				}

				return true;

			}

			else if (var->type == CC_TYPE_LONG)
			{
				long value = 0;
				if (!VarValueGetLong(_parser, var, &value))
				{
					return 0;
				}

				value -= 1;

				if (!VarValueSetLong(_parser, var, value))
				{
					return 0;
				}

				return true;

			}

			else
			{
				parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;
			}

		}
		else if (ch == '=')
		{
			// prirazeni promenne:
			// var -= 20;

			var_s *var = VarGet(_parser, _var_name, _var_name_len);
			if (var == NULL)
			{
				return 0;
			}

			if (var->type == CC_TYPE_FLOAT)
			{
				float ival = 0;
				if (!parseVarArgsFloat(_parser, ';', &ival))
				{
					return 0;
				}

				float value = 0.0f;
				if (!VarValueGetFloat(_parser, var, &value))
				{
					return 0;
				}

				value -= ival;

				if (!VarValueSetFloat(_parser, var, value))
				{
					return 0;
				}

				return true;

			}

			else if (var->type == CC_TYPE_INT)
			{
				int ival = 0;
				if (!parseVarArgsInt(_parser, ';', &ival))
				{
					return 0;
				}

				int value = 0;
				if (!VarValueGetInt(_parser, var, &value))
				{
					return 0;
				}

				value -= ival;

				if (!VarValueSetInt(_parser, var, value))
				{
					return 0;
				}

				return true;

			}

			else if (var->type == CC_TYPE_LONG)
			{

				long ival = 0;
				if (!parseVarArgsLong(_parser, ';', &ival))
				{
					return 0;
				}

				long value = 0;
				if (!VarValueGetLong(_parser, var, &value))
				{
					return 0;
				}

				value -= ival;

				if (!VarValueSetLong(_parser, var, value))
				{
					return 0;
				}

				return true;

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
			return 0;
		}

	}
	parseSetError(_parser, CC_CODE_BAD_SYMBOL);
	parseSetErrorPos(_parser, parseGetPos(_parser));
	return 0;
}

bool parseVarIncrementRight(parser_s *_parser, char *_var_name, size_t _var_name_len)
{
	char ch = 0;
	file_bufferGet(_parser->buffer, &ch);

	if (ch == '+')
	{
		// inkrementace promenne?
		// var++;?

		file_bufferNext(_parser->buffer);
		file_bufferSkipSpace(_parser->buffer);
		file_bufferGet(_parser->buffer, &ch);

		if (ch == '+')
		{
			// Inkrementace promenne:
			// var++;

			file_bufferNext(_parser->buffer);
			file_bufferSkipSpace(_parser->buffer);
			file_bufferGet(_parser->buffer, &ch);

			if (ch != ';')
			{
				parseSetError(_parser, CC_CODE_BAD_SYMBOL);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return 0;
			}

			var_s *var = VarGet(_parser, _var_name, _var_name_len);
			if (var == NULL)
			{
				parseSetError(_parser, CC_CODE_VAR_NOT_DEFINED);

				return 0;
			}

			if (var->type == CC_TYPE_FLOAT)
			{
				float value = 0.0f;
				if (!VarValueGetFloat(_parser, var, &value))
				{
					return 0;
				}

				value += 1;

				if (!VarValueSetFloat(_parser, var, value))
				{
					return 0;
				}

				return true;

			}

			else if (var->type == CC_TYPE_INT)
			{
				int value = 0;
				if (!VarValueGetInt(_parser, var, &value))
				{
					return 0;
				}

				value += 1;

				if (!VarValueSetInt(_parser, var, value))
				{
					return 0;
				}

				return true;

			}

			else if (var->type == CC_TYPE_LONG)
			{
				long value = 0;
				if (!VarValueGetLong(_parser, var, &value))
				{
					return 0;
				}

				value += 1;

				if (!VarValueSetLong(_parser, var, value))
				{
					return 0;
				}

				return true;

			}

			else
			{
				parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;
			}

		}

		else if (ch == '=')
		{
			// prirazeni promenne:
			// var += 20;

			var_s *var = VarGet(_parser, _var_name, _var_name_len);
			if (var == NULL)
			{
				parseSetError(_parser, CC_CODE_VAR_NOT_DEFINED);
				return 0;
			}

			if (var->type == CC_TYPE_FLOAT)
			{
				float ival = 0;
				if (!parseVarArgsFloat(_parser, ';', &ival))
				{
					return 0;
				}

				float value = 0.0f;
				if (!VarValueGetFloat(_parser, var, &value))
				{
					return 0;
				}

				value += ival;

				if (!VarValueSetFloat(_parser, var, value))
				{
					return 0;
				}
			}

			else if (var->type == CC_TYPE_INT)
			{
				int ival = 0;
				if (!parseVarArgsInt(_parser, ';', &ival))
				{
					return 0;
				}

				int value = 0;
				if (!VarValueGetInt(_parser, var, &value))
				{
					return 0;
				}

				value += ival;

				if (!VarValueSetInt(_parser, var, value))
				{
					return 0;
				}
			}

			else if (var->type == CC_TYPE_LONG)
			{
				long ival = 0;
				if (!parseVarArgsLong(_parser, ';', &ival))
				{
					return 0;
				}

				long value = 0;
				if (!VarValueGetLong(_parser, var, &value))
				{
					return 0;
				}

				value += ival;

				if (!VarValueSetLong(_parser, var, value))
				{
					return 0;
				}
			}

			else if (var->type == CC_TYPE_STRING)
			{
				char sval[CONFIG_CC_STRING_LEN] = {
						'\0' };
				size_t sval_len = 0;
				if (!parseVarArgsString(_parser, ';', sval, &sval_len))
				{
					return 0;
				}

				char sval_l[CONFIG_CC_STRING_LEN] = {
						'\0' };
				size_t sval_l_len = 0;
				if (!VarValueGetString(_parser, var, sval_l, &sval_l_len))
				{
					return 0;
				}

				if (sval_len + sval_l_len > CC_VALUE_STRING_LEN)
				{
					parseSetError(_parser, CC_CODE_STRING_TOO_LONG);
					parseSetErrorPos(_parser, parseGetPos(_parser));
					return false;
				}

				memcpy(sval_l + sval_l_len, sval, sval_len);
				sval_l_len += sval_len;

				if (!VarValueSetString(_parser, var, sval_l, sval_l_len))
				{
					return 0;
				}
			}

			else
			{
				parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;
			}

			return true;

		}
		else
		{
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return 0;
		}
	}

	parseSetError(_parser, CC_CODE_BAD_SYMBOL);
	parseSetErrorPos(_parser, parseGetPos(_parser));
	return 0;
}

bool parseVarMultiplyRight(parser_s *_parser, char *_var_name, size_t _var_name_len)
{

	char ch = 0;
	file_bufferGet(_parser->buffer, &ch);

	if (ch == '*')
	{
		// Nasobeni promenne?
		// var *= 20;

		file_bufferNext(_parser->buffer);
		file_bufferSkipSpace(_parser->buffer);
		file_bufferGet(_parser->buffer, &ch);

		if (ch == '=')
		{
			// prirazeni promenne:
			// var *= 20;

			var_s *var = VarGet(_parser, _var_name, _var_name_len);
			if (var == NULL)
			{
				return 0;
			}

			if (var->type == CC_TYPE_FLOAT)
			{
				float ival = 0;
				if (!parseVarArgsFloat(_parser, ';', &ival))
				{
					return 0;
				}

				float value = 0;
				if (!VarValueGetFloat(_parser, var, &value))
				{
					return 0;
				}

				value *= ival;

				if (!VarValueSetFloat(_parser, var, value))
				{
					return 0;
				}
			}

			else if (var->type == CC_TYPE_INT)
			{
				int ival = 0;
				if (!parseVarArgsInt(_parser, ';', &ival))
				{
					return 0;
				}

				int value = 0;
				if (!VarValueGetInt(_parser, var, &value))
				{
					return 0;
				}

				value *= ival;

				if (!VarValueSetInt(_parser, var, value))
				{
					return 0;
				}
			}

			else if (var->type == CC_TYPE_LONG)
			{
				long ival = 0;
				if (!parseVarArgsLong(_parser, ';', &ival))
				{
					return 0;
				}

				long value = 0;
				if (!VarValueGetLong(_parser, var, &value))
				{
					return 0;
				}

				value *= ival;

				if (!VarValueSetLong(_parser, var, value))
				{
					return 0;
				}
			}

			else
			{
				parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;
			}

			return true;

		}
		else
		{
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return 0;
		}

	}

	parseSetError(_parser, CC_CODE_BAD_SYMBOL);
	parseSetErrorPos(_parser, parseGetPos(_parser));
	return 0;
}

bool parseVarDivideRight(parser_s *_parser, char *_var_name, size_t _var_name_len)
{

	char ch = 0;
	file_bufferGet(_parser->buffer, &ch);

	if (ch == '/')
	{
		// Deleni promenne?
		// var /= 20;

		file_bufferNext(_parser->buffer);
		file_bufferSkipSpace(_parser->buffer);
		file_bufferGet(_parser->buffer, &ch);

		if (ch == '=')
		{

			var_s *var = VarGet(_parser, _var_name, _var_name_len);
			if (var == NULL)
			{
				return 0;
			}

			if (var->type == CC_TYPE_FLOAT)
			{
				float ival = 0;
				if (!parseVarArgsFloat(_parser, ';', &ival))
				{
					return 0;
				}

				float value = 0;
				if (!VarValueGetFloat(_parser, var, &value))
				{
					return 0;
				}

				value /= ival;

				if (!VarValueSetFloat(_parser, var, value))
				{
					return 0;
				}
			}

			else if (var->type == CC_TYPE_INT)
			{
				int ival = 0;
				if (!parseVarArgsInt(_parser, ';', &ival))
				{
					return 0;
				}

				int value = 0;
				if (!VarValueGetInt(_parser, var, &value))
				{
					return 0;
				}

				value /= ival;

				if (!VarValueSetInt(_parser, var, value))
				{
					return 0;
				}
			}

			else if (var->type == CC_TYPE_LONG)
			{
				long ival = 0;
				if (!parseVarArgsLong(_parser, ';', &ival))
				{
					return 0;
				}

				long value = 0;
				if (!VarValueGetLong(_parser, var, &value))
				{
					return 0;
				}

				value /= ival;

				if (!VarValueSetLong(_parser, var, value))
				{
					return 0;
				}
			}

			else
			{
				parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;
			}

			return true;

		}
		else
		{
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return 0;
		}
	}
	else
	{
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return 0;
	}

	parseSetError(_parser, CC_CODE_BAD_SYMBOL);
	parseSetErrorPos(_parser, parseGetPos(_parser));
	return 0;
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
