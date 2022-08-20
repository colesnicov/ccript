/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file cc_parseChar.c
 * @brief Implementace funkci pro parsovani typu 'CHAR'.
 *
 * @version 1b0
 * @date 26.06.2022
 *
 * @author Denis Colesnicov <eugustus@gmail.com>
 *
 * @copyright MIT License
 *
 */

#include "ccript/cc_buffer.h"
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

bool ParseValueChar(parser_s *_parser, char *_value, size_t *_value_len) {

	char ch;
	char ch_temp;

	bufferGet(_parser, &ch);

	if (ch == '\'') {
		// prvni znak musi byt apostrof (')

		bufferNext(_parser);
		bufferGet(_parser, &ch);

		ch_temp = ch;

		if (ch == '\\') {
			// escape sequence?
			// Nebezpecne znaky jsou:
			// ',\ a kombinace jako
			// '\\'
			// '\''

			bufferNext(_parser);
			bufferGet(_parser, &ch);

			if (ch == '\'') {
				// apostrof

				bufferNext(_parser);
				bufferGet(_parser, &ch);

				if (ch == '\'') {
					// Konec komandy

					_value[0] = '\'';
					*_value_len = 1;

					return true;
				}

				else {
					parseSetError(_parser, CC_CODE_BAD_SYMBOL);
					parseSetErrorPos(_parser, parseGetPos(_parser));

					return false;
				}

			}

			else if (ch == '\\') {
				// zpetne lomitko (\)

				bufferNext(_parser);
				bufferGet(_parser, &ch);

				if (ch == '\'') {
					// Konec komandy

					_value[0] = '\\';
					*_value_len = 1;
					return true;
				}

				else if (ch == ';') {
					// Konec komandy

					_value[0] = ch_temp;
					*_value_len = 1;

					return true;
				}

				else {
					parseSetError(_parser, CC_CODE_BAD_SYMBOL);
					parseSetErrorPos(_parser, parseGetPos(_parser));

					return false;
				}

			}

			else if (charin(ch, "nrt")) {
				// znak novy radek, return nebo tab (\n,\r,\t)

				ch_temp = ch;

				bufferNext(_parser);
				bufferGet(_parser, &ch);

				if (ch == '\'') {
					// Konec komandy

					if (ch_temp == 'n') {
						_value[0] = '\n';
					} else if (ch_temp == 'r') {
						_value[0] = '\r';
					} else if (ch_temp == 't') {
						_value[0] = '\t';
					}

					*_value_len = 1;
					return true;
				}

				else if (ch == ';') {
					// Konec komandy

					CC_PRINT("ERROR: 333 Todle je asi spatne. musi nejdrive koncit (') "
							"a az potom muze byt ',' nebo ';' nebo ')' a podobne.\n");

					return false;

					_value[0] = ch_temp;
					*_value_len = 1;

					return true;
				}

				else {
					parseSetError(_parser, CC_CODE_BAD_SYMBOL);
					parseSetErrorPos(_parser, parseGetPos(_parser));

					return false;
				}

			}

			else {
				parseSetError(_parser, CC_CODE_BAD_SYMBOL);
				parseSetErrorPos(_parser, parseGetPos(_parser));

				return false;
			}
		}

		else {
			// znak

			bufferNext(_parser);
			bufferGet(_parser, &ch);

			if (ch == '\'' && ch_temp != '\\') {
				// Konec komandy

// @fixme Znova projit takove funkce a overit si spravnost logiky. nektere sekce jsou zbytecne!?

				_value[0] = ch_temp;

				*_value_len = 1;
				return true;
			}

			else if (ch == ';') {
				// Konec komandy

				CC_PRINT("ERROR: 334 Todle je asi spatne. musi nejdrive koncit (') "
						"a az potom muze byt ',', ';', ')' a podobne.\n");

				parseSetError(_parser, CC_CODE_BAD_SYMBOL);
				parseSetErrorPos(_parser, parseGetPos(_parser));

				return false;

				_value[0] = '\0';

				*_value_len = 1;
				return true;
			}

			else {

				parseSetError(_parser, CC_CODE_BAD_SYMBOL);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;
			}
		}
	}

	else {
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;
	}
}

bool parseVarArgsChar(parser_s *_parser, char _symbol_end, char *_value) {

	size_t value_len = 0;
	char value_name[CONFIG_CC_STRING_LEN] = { '\0' };

	size_t fval_temp_len = 0;
	char fval_temp = 0;

	char ch = 0;

	parseSkipNewLine(_parser);

	bufferGet(_parser, &ch);

	if (ch == '\'') {
		if (!ParseValueChar(_parser, &fval_temp, &fval_temp_len)) {
			return false;
		}

		if (fval_temp_len == 0) { // fixme brani v prirazeni prazdneho retezce?
			parseSetError(_parser, CC_CODE_KEYWORD);
			parseSetErrorPos(_parser, parseGetPos(_parser));

			return false;
		}
		bufferNext(_parser);

	}

	else if (ch == '(') {

		bufferNext(_parser);
		bufferSkipSpace(_parser);
		if (!parseVarArgsChar(_parser, ')', &fval_temp)) {
			return false;
		}
	}

	else {
		// promenna nebo funkce

		if (!parseIdentifier(_parser, value_name, &value_len)) {
			return false;
		}

		if (value_len == 0) {
			parseSetError(_parser, CC_CODE_KEYWORD);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;
		}

		bufferSkipSpace(_parser);
		bufferGet(_parser, &ch);

		if (ch == '[') {
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;
		}

		else if (ch == '(') {
			// volani funkce ...

			var_s *var = funcCall(_parser, value_name, value_len);

			if (_parser->error > CC_CODE_RETURN) {
				VarDestroy(var);
				return false;
			}

			if (var == NULL) {

				CC_PRINT("ERROR: function '%s' return 'null'.\n", value_name);
				parseSetError(_parser, CC_CODE_LOGIC);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;
			}

			bufferNext(_parser);
			bufferSkipSpace(_parser);
			bufferGet(_parser, &ch);

			if (ch != ';') {
				parseSetError(_parser, CC_CODE_BAD_SYMBOL);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				VarDestroy(var);
				return false;
			}

			if (var->type == CC_TYPE_CHAR) {
				value_len = 1;
				if (!VarValueGetChar(_parser, var, &fval_temp)) {
					VarDestroy(var);
					return false;
				}
			}

			else {

				CC_PRINT("ERROR: function '%s' return bad type.\n", value_name);
				VarDestroy(var);
				return false;
			}

			VarDestroy(var);

		}

		else if (ch == ';') {

			var_s *var = VarGet(_parser, value_name, value_len);
			if (var == NULL) {
				return false;
			}

			if (!VarValueGetChar(_parser, var, &fval_temp)) {
				return false;
			}

		}

		else {
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;
		}

	}

	bufferSkipSpace(_parser);

	bufferGet(_parser, &ch);

	if (ch == _symbol_end) {
		//konec vety

		if (value_len + fval_temp_len > CC_VALUE_STRING_LEN) {
			parseSetError(_parser, CC_CODE_STRING_TOO_LONG);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;
		}

		*_value = fval_temp;

		bufferNext(_parser);
		return true;

	}

	else if (ch == '(') {
		parseSetError(_parser, CC_CODE_LOGIC);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;

	}

	else if (ch == '[') {
		parseSetError(_parser, CC_CODE_LOGIC);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;
	}

	else {
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
