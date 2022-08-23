/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file cc_parseInt.c
 * @brief Implementace funkci pro parsovani typu 'INT'.
 *
 * @version 1b0
 * @date 26.06.2022
 *
 * @author Denis Colesnicov <eugustus@gmail.com>
 *
 * @copyright MIT License
 *
 */

//#include "ccript/cc_buffer.h"
#include "ccript/cc_configs.h"
#include "ccript/cc_function.h"
#include "ccript/cc_log.h"
#include "ccript/cc_parser.h"
#include "ccript/cc_types.h"
#include "ccript/cc_var_ext.h"
#include "ccript/cc_var.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define OP_SUM	1
#define OP_SUB	2
#define OP_MUL	3
#define OP_DIV	4

bool ParseDefineTypeInt(parser_s *_parser) {
	char ch = '\0';

	file_bufferSkipSpace(_parser->buffer);

	file_bufferGet(_parser->buffer, &ch);

	if (!isalpha(ch)) {
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;
	}

	/**
	 * @var char identifier_name[CONFIG_CC_KEYWORD_LEN]
	 * @brief Nazev promenne/funkce
	 *
	 */
	char identifier_name[CONFIG_CC_KEYWORD_LEN] = { '\0' };
	/**
	 * @var size_t identifier_len
	 * @brief Delka nazvu promenne/funkce
	 *
	 */
	size_t identifier_len = 0;

	if (!parseIdentifier(_parser, identifier_name, &identifier_len)) {
		return false;
	}

	if (identifier_len == 0) {
		parseSetError(_parser, CC_CODE_KEYWORD);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;

	}

	file_bufferSkipSpace(_parser->buffer);
	file_bufferGet(_parser->buffer, &ch);

	if (ch == '=') {
		// definice a prirazeni promenne
		// ziskej nazev promenne (uz mam: 'keyword_position')
		// over neexistenci promenne
		// ziskej hodnotu prirazeni, over co nasleduje po vyrazu:
		// ';' konec vety
		// 'operator (+-/*)' scitani nekolika cisel
		// '(' volani funkce ktera vraci numeric

//		file_bufferNext(_parser->buffer);
//		file_bufferSkipSpace(_parser->buffer);

		int ival = 0;
		if (!parseVarArgsInt(_parser, ';', &ival)) {
			return false;
		}

		var_s *var = VarCreate(identifier_name, identifier_len, CC_TYPE_INT, _parser->depth);

		if (var == NULL) {
			return false;
		}

		if (!VarValueSetInt(_parser, var, ival)) {
			VarDestroy(var);
			return false;
		}

		if (!VarStore(_parser, var)) {
			VarDestroy(var);
			return false;
		}

		file_bufferNext(_parser->buffer);

		return true;

	}

	else if (ch == ';') {
		// definice promenne bez prirazeni

		var_s *var = VarCreate(identifier_name, identifier_len, CC_TYPE_INT, _parser->depth);
		if (var == NULL) {
			return false;
		}

		if (!VarStore(_parser, var)) {
			VarDestroy(var);
			return false;
		}

		file_bufferNext(_parser->buffer);

		return true;

	}

	else if (ch == '(') {
		// definice funkce

		return parseDefineBlock(_parser, CC_TYPE_INT, identifier_name, identifier_len);
	}

	else {
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;
	}

}

bool parseVarArgsInt(parser_s *_parser, char _symbol_end, int *_value) {

	char value_name[CONFIG_CC_NUMERIC_LEN] = { '\0' };
	size_t value_len;
	int ival = 0;
	int ival_temp = 0;
	uint8_t last_op = 0;
	char ch = 0;
	file_bufferNext(_parser->buffer);
	file_bufferGet(_parser->buffer, &ch);
	if (ch == '\n') {
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;
	}

	file_bufferSkipSpace(_parser->buffer);

	file_bufferGet(_parser->buffer, &ch);
	if (ch == '\n') {
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;
	}

	{
		// muze zacinat pouze pismenem, cislici znakem '-', '+' a '('

		file_bufferGet(_parser->buffer, &ch);

		if (!isalnum(ch) && !charin(ch, "-(")) {
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));

			return false;
		}
	}

	while (FILEBUFFER_OK == file_bufferValid(_parser->buffer)) {
		memset(value_name, '\0', CC_VALUE_NUMERIC_LEN);
		ival_temp = 0;
		parseSkipNewLine(_parser);

		file_bufferGet(_parser->buffer, &ch);

		if (ch == '(') {
			// zanoreni

//			file_bufferNext(_parser->buffer);

			if (!parseVarArgsInt(_parser, ')', &ival_temp)) {
				return false;
			}
		}

		else

		if (isdigit(ch) || ch == '-') {
			// cislo

			if (!parseValueInt(_parser, value_name, &value_len)) {
				return false;
			}

			if (value_len == 0) {
				parseSetError(_parser, CC_CODE_KEYWORD);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;

			}

			ival_temp = atoi(value_name);

		}

		else {
			// promenna nebo funkce

			size_t pos = parseGetPos(_parser);

			if (!parseIdentifier(_parser, value_name, &value_len)) {
				return false;
			}

			if (value_len == 0) {
				parseSetError(_parser, CC_CODE_KEYWORD);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;

			}

			parseSetErrorPos(_parser, parseGetPos(_parser));
			file_bufferSkipSpace(_parser->buffer);
			file_bufferGet(_parser->buffer, &ch);

			if (ch == '[') {
				parseSetError(_parser, CC_CODE_BAD_SYMBOL);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;
			} else if (ch == '(') {
				// volani funkce

				var_s *var = funcCall(_parser, value_name, value_len);

				if (_parser->error > CC_CODE_RETURN) {
					VarDestroy(var);
					return false;
				}

				if (var == NULL) {
					parseSetError(_parser, CC_CODE_LOGIC);
					parseSetErrorPos(_parser, pos);

					CC_PRINT("ERROR: function '%s' return 'null'.\n", value_name);
					return false;
				}

				file_bufferNext(_parser->buffer);
				file_bufferSkipSpace(_parser->buffer);
				file_bufferGet(_parser->buffer, &ch);

				if (ch != ';' && !charin(ch, "+-/*")) {
					parseSetError(_parser, CC_CODE_BAD_SYMBOL);
					parseSetErrorPos(_parser, parseGetPos(_parser));
					VarDestroy(var);
					return false;
				}

				if (var->type != CC_TYPE_INT) {
					CC_PRINT("ERROR: function '%s' returns bad type.\n", value_name);
					parseSetError(_parser, CC_CODE_LOGIC);
					parseSetErrorPos(_parser, pos);
					VarDestroy(var);
					return false;
				}

				if (!VarValueGetInt(_parser, var, &ival_temp)) {
					VarDestroy(var);
					return false;
				}

				VarDestroy(var);
//				file_bufferNext(_parser->buffer);

			}

			else {
				file_bufferGet(_parser->buffer, &ch);

				if (ch != _symbol_end && !charin(ch, "+-/*")) {
					parseSetError(_parser, CC_CODE_BAD_SYMBOL);
					return false;
				}

				var_s *var = VarGet(_parser, value_name, value_len);
				if (var == NULL) {
					parseSetError(_parser, CC_CODE_VAR_NOT_DEFINED);
					return false;
				}

				if (!VarValueGetInt(_parser, var, &ival_temp)) {
					return false;
				}

			}

		}

		if (last_op == OP_SUM) {
			ival += ival_temp;
			last_op = 0;
		}

		else if (last_op == OP_SUB) {
			ival -= ival_temp;
			last_op = 0;
		}

		else if (last_op == OP_MUL) {
			ival *= ival_temp;
			last_op = 0;
		}

		else if (last_op == OP_DIV) {
			ival /= ival_temp;
			last_op = 0;
		} else {
			ival = ival_temp;

		}

		{/* fixme toto je potreba?*/
			file_bufferSkipSpace(_parser->buffer);

			file_bufferGet(_parser->buffer, &ch);
		}

		if (ch == '+') {
			last_op = OP_SUM;
			file_bufferNext(_parser->buffer);

			continue;
		}
		if (ch == '-') {
			last_op = OP_SUB;
			file_bufferNext(_parser->buffer);

			continue;
		}

		else if (ch == '/') {
			last_op = OP_DIV;
			file_bufferNext(_parser->buffer);

			continue;
		}

		else if (ch == '*') {
			last_op = OP_MUL;
			file_bufferNext(_parser->buffer);

			continue;
		}

		else if (ch == _symbol_end) {
			*_value = ival;
			file_bufferNext(_parser->buffer);

			return true;

		}

		else if (ch == '(') {
			CC_PRINT("ASA");
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;

		}

		else if (ch == '[') {
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;
		}

		else {
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;
		}
	}
	return false;
}

bool parseValueInt(parser_s *_parser, char *_value, size_t *_len) {
	char ch;

	file_bufferGet(_parser->buffer, &ch);

	if (!isdigit(ch) && ch != '-') {
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;

	}

	size_t i = 0;

	_value[i++] = ch;

	while (FILEBUFFER_OK == file_bufferNext(_parser->buffer)) {

		file_bufferGet(_parser->buffer, &ch);
		if (!isdigit(ch)) {
			*_len = i;
			return true;
		}

		_value[i++] = ch;
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
