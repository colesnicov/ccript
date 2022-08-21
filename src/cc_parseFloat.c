/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file cc_parseFloat.c
 * @brief Implementace funkci pro parsovani typu 'FLOAT'.
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
#include "ccript/cc_parser.h"
#include "ccript/cc_types.h"
#include "ccript/cc_var.h"
#include "ccript/common.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define OP_SUM	1
#define OP_SUB	2
#define OP_MUL	3
#define OP_DIV	4

bool ParseDefineTypeFloat(parser_s *_parser) {

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
		return false;

	}

	file_bufferSkipSpace(_parser->buffer);

	file_bufferGet(_parser->buffer, &ch);

	if (ch == '=') {
		// definice promenne a prirazeni

		file_bufferNext(_parser->buffer);
		file_bufferSkipSpace(_parser->buffer);

		float fval = 0;
		if (!parseVarArgsFloat(_parser, ';', &fval)) {
			return false;
		}

		var_s *var = VarCreate(identifier_name, identifier_len, CC_TYPE_FLOAT, _parser->depth);

		if (var == NULL) {
			return false;
		}

		if (!VarValueSetFloat(_parser, var, fval)) {
			VarDestroy(var);
			return false;
		}

		if (!VarStore(_parser, var)) {
			VarDestroy(var);
			return false;
		}

		file_bufferNext(_parser->buffer);

		return true;

	} else if (ch == ';') {
		// definice promenne bez prirazeni

		var_s *var = VarCreate(identifier_name, identifier_len, CC_TYPE_FLOAT, _parser->depth);
		if (var == NULL) {
			return false;
		}

		if (!VarStore(_parser, var)) {
			VarDestroy(var);
			return false;
		}

		file_bufferNext(_parser->buffer);

		return true;

	} else {
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;
	}

}

bool parseVarArgsFloat(parser_s *_parser, char _symbol_end, float *_value) {

	char value_name[CONFIG_CC_NUMERIC_LEN] = { '\0' };
	size_t value_len;
	float fval = 0;
	float fval_temp = 0;
	uint8_t last_op = 0;
	char ch = 0;

	while (FILEBUFFER_OK == file_bufferValid(_parser->buffer)) {
		memset(value_name, '\0', CC_VALUE_NUMERIC_LEN);
		fval_temp = 0.0f;

		parseSkipNewLine(_parser);
		file_bufferGet(_parser->buffer, &ch);

		if (isdigit(ch) || ch == '-' || ch == '.') {
			// cislo

			bool is_float = false;
			if (!parseValueFloat(_parser, value_name, &value_len, &is_float)) {
				return false;
			}

			if (value_len == 0) {
				parseSetError(_parser, CC_CODE_KEYWORD);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;

			}

			fval_temp = atof(value_name);

		} else if (ch == '(') {
			file_bufferNext(_parser->buffer);
			file_bufferSkipSpace(_parser->buffer);

			if (!parseVarArgsFloat(_parser, ')', &fval_temp)) {
				return false;
			}
		} else {
			// promenna nebo funkce

			if (!parseIdentifier(_parser, value_name, &value_len)) {
				return false;
			}

			if (value_len == 0) {
				parseSetError(_parser, CC_CODE_KEYWORD);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;

			}

			file_bufferSkipSpace(_parser->buffer);

			file_bufferGet(_parser->buffer, &ch);

			if (ch == '[') {
				parseSetError(_parser, CC_CODE_BAD_SYMBOL);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;

			} else if (ch == '(') {
				// volani funkce

				size_t pos = parseGetPos(_parser);
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

//				if (ch != ';' && !charin(ch, "+-/*")) {
				if (ch != _symbol_end && !charin(ch, "+-/*")) {
					parseSetError(_parser, CC_CODE_BAD_SYMBOL);
					parseSetErrorPos(_parser, parseGetPos(_parser));

					VarDestroy(var);
					return false;
				}

				if (var->type != CC_TYPE_FLOAT) {
					CC_PRINT("ERROR: function '%s' return bad type.\n", value_name);
					parseSetError(_parser, CC_CODE_LOGIC);
					parseSetErrorPos(_parser, parseGetPos(_parser));
					VarDestroy(var);
					return false;
				}

				if (!VarValueGetFloat(_parser, var, &fval_temp)) {
					VarDestroy(var);
					return false;
				}

				VarDestroy(var);

			}

			else {

				var_s *var = VarGet(_parser, value_name, value_len);
				if (var == NULL) {
					return false;
				}

				if (!VarValueGetFloat(_parser, var, &fval_temp)) {
					return false;
				}

			}

		}

		file_bufferSkipSpace(_parser->buffer);
		file_bufferGet(_parser->buffer, &ch);

		if (ch == _symbol_end) {
			if (last_op == OP_SUM) {
				fval += fval_temp;
			} else if (last_op == OP_SUB) {
				fval -= fval_temp;
			} else if (last_op == OP_MUL) {
				fval *= fval_temp;
			} else if (last_op == OP_DIV) {
				fval /= fval_temp;
			} else {
				fval = fval_temp;
			}

			*_value = fval;
			file_bufferNext(_parser->buffer);

			return true;

		} else if (ch == '(') {
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			CC_PRINT("ERROR: function call?\n");

			return false;

		} else if (ch == '[') {
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			CC_PRINT("ERROR: array access?\n");

			return false;
		} else if (ch == '+') {

			float f = fval_temp;

			if (last_op == OP_SUM) {
				fval += f;
			} else if (last_op == OP_SUB) {
				fval -= f;
			} else if (last_op == OP_MUL) {
				fval *= f;
			} else if (last_op == OP_DIV) {
				fval /= f;
			} else {
				fval = f;
			}

			last_op = OP_SUM;

			file_bufferNext(_parser->buffer);

			continue;
		}
		if (ch == '-') {

			float f = fval_temp;

			if (last_op == OP_SUM) {
				fval += f;
			} else if (last_op == OP_SUB) {
				fval -= f;
			} else if (last_op == OP_MUL) {
				fval *= f;
			} else if (last_op == OP_DIV) {
				fval /= f;
			} else {
				fval = f;
			}

			last_op = OP_SUB;

			file_bufferNext(_parser->buffer);

			continue;

		} else if (ch == '/') {

			float f = fval_temp;

			if (last_op == OP_SUM) {
				fval += f;
			} else if (last_op == OP_SUB) {
				fval -= f;
			} else if (last_op == OP_MUL) {
				fval *= f;
			} else if (last_op == OP_DIV) {
				fval /= f;
			} else {
				fval = f;
			}

			last_op = OP_DIV;

			file_bufferNext(_parser->buffer);

			continue;

		} else if (ch == '*') {

			float f = fval_temp;

			if (last_op == OP_SUM) {
				fval += f;
			} else if (last_op == OP_SUB) {
				fval -= f;
			} else if (last_op == OP_MUL) {
				fval *= f;
			} else if (last_op == OP_DIV) {
				fval /= f;
			} else {
				fval = f;
			}

			last_op = OP_MUL;

			file_bufferNext(_parser->buffer);

			continue;

		} else {

			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;
		}
	}
	return false;
}

bool parseValueFloat(parser_s *_parser, char *_value, size_t *_value_len, bool *_is_float) {

	// todo predelat aby float mel (mohl mit) na konci pismeno 'f'?
	char ch;
	bool hasDot = false;

	file_bufferGet(_parser->buffer, &ch);

	if (!isdigit(ch) && ch != '-' && ch != '.') {
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;

	}

	size_t i = 0;

	_value[i++] = ch;

	while (FILEBUFFER_OK == file_bufferValid(_parser->buffer)) {

		file_bufferNext(_parser->buffer);
		file_bufferGet(_parser->buffer, &ch);

		if (!isdigit(ch)) {
			if (ch == '.') {
				if (hasDot) {
					CC_PRINT("ERROR: second dot in the float value");
					parseSetError(_parser, CC_CODE_BAD_SYMBOL);
					parseSetErrorPos(_parser, parseGetPos(_parser));
					return false;
				}
				hasDot = true;
			} else {
				*_is_float = hasDot;

				*_value_len = i;
				return true;
			}
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
