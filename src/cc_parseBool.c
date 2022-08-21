/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file cc_parseBool.c
 * @brief Implementace funkci pro parsovani typu 'BOOL'.
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

#define OP_AND	5
#define OP_OR	6
#define OP_AND2	7 // unused
#define OP_OR2	8 // unused

bool ParseDefineTypeBool(parser_s *_parser, char *_keyword_name) {

	char ch = '\0';

	file_bufferSkipSpace(_parser->buffer);
	file_bufferGet(_parser->buffer, &ch);

	if (!isalpha(ch)) {
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));

		return false;
	}

	/**
	 * @var size_t identifier_len
	 * @brief Delka nazvu promenne/funkce
	 *
	 */
	size_t identifier_len = 0;

	memset(_keyword_name, '\0', CC_KEYWORD_LEN);

	{
		size_t pos = parseGetPos(_parser);
		if (!parseIdentifier(_parser, _keyword_name, &identifier_len)) {
			return false;
		}

		if (identifier_len == 0) {
			// fixme jinou direktivu CC_CODE_KEYWORD_EMPTY
			parseSetError(_parser, CC_CODE_KEYWORD);
			parseSetErrorPos(_parser, pos);
			return false;

		}
	}

	file_bufferSkipSpace(_parser->buffer);
	file_bufferGet(_parser->buffer, &ch);

	if (ch == '=') {
		// definice a prirazeni promenne

		file_bufferNext(_parser->buffer);
		file_bufferSkipSpace(_parser->buffer);

		bool ival = 0;
		if (!parseVarArgsBool(_parser, ';', &ival)) {
			return false;
		}

		var_s *var = VarCreate(_keyword_name, identifier_len, CC_TYPE_BOOL, _parser->depth);

		if (var == NULL) {
			return false;
		}

		if (!VarValueSetBool(_parser, var, ival)) {
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

		var_s *var = VarCreate(_keyword_name, identifier_len, CC_TYPE_BOOL, _parser->depth);
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

bool parseVarArgsBool(parser_s *_parser, char _symbol_end, bool *_value) {

	char value_name[CONFIG_CC_KEYWORD_LEN] = { '\0' };
	size_t value_len;
	bool negation = 0;
	bool ival = 1;
	bool ival_temp = 0;
	uint8_t last_op = 0;
	char ch = 0;

	while (FILEBUFFER_OK == file_bufferValid(_parser->buffer)) {
		memset(value_name, '\0', CC_KEYWORD_LEN);
		ival_temp = 0;
		negation = 0;

		parseSkipNewLine(_parser);
		file_bufferGet(_parser->buffer, &ch);
		if (ch == '!') {
			// negace
			negation = true;
			file_bufferNext(_parser->buffer);
			file_bufferSkipSpace(_parser->buffer);
			file_bufferGet(_parser->buffer, &ch);

		}

		if (isdigit(ch)) {
			// cislo

			if (!parseValueBool(_parser, value_name, &value_len)) {
				return false;
			}

			if (value_len == 0) {
				return false;

			}

			ival_temp = atoi(value_name);
		}

		else if (ch == '(') {
			file_bufferNext(_parser->buffer);
			file_bufferSkipSpace(_parser->buffer);

			if (!parseVarArgsBool(_parser, ')', &ival_temp)) {
				return false;
			}
		}

		else if (isalpha(ch)) {
			// promenna nebo funkce

			if (!parseIdentifier(_parser, value_name, &value_len)) {
				return false;
			}

			if (value_len == 0) {
				return false;

			}

			file_bufferSkipSpace(_parser->buffer);
			file_bufferGet(_parser->buffer, &ch);

			if (value_len == 4 && strncmp(value_name, "true", value_len) == 0) {
				ival_temp = 1;
			}

			else if (value_len == 5 && strncmp(value_name, "false", value_len) == 0) {
				ival_temp = 0;
			}

			else if (ch == '[') {
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
					return false;
				}

				file_bufferNext(_parser->buffer);
				file_bufferSkipSpace(_parser->buffer);

				if (FILEBUFFER_OK != file_bufferGet(_parser->buffer, &ch)
						|| (ch != ';' && !charin(ch, "|&"))) {
					parseSetError(_parser, CC_CODE_BAD_SYMBOL);
					parseSetErrorPos(_parser, parseGetPos(_parser));
					VarDestroy(var);
					return false;
				}

				/// fixme kod chyby nastavovat ve funkci!
				if (!VarValueGetBool(_parser, var, &ival_temp)) {
					parseSetErrorPos(_parser, parseGetPos(_parser));
					VarDestroy(var);
					return false;
				}

				VarDestroy(var);

			}

			else if (ch == '|') {

				var_s *var = VarGet(_parser, value_name, value_len);
				if (var == NULL) {
					CC_PRINT("ERROR: undefined variable '%s'.\n", value_name);
					return false;
				}

				if (!VarValueGetBool(_parser, var, &ival_temp)) {
					return false;
				}

				last_op = OP_OR;

			}

			else if (ch == '&') {

				var_s *var = VarGet(_parser, value_name, value_len);
				if (var == NULL) {
					CC_PRINT("ERROR: undefined variable '%s'.\n", value_name);
					return false;
				}

				if (!VarValueGetBool(_parser, var, &ival_temp)) {
					return false;
				}

				last_op = OP_AND;

			}

			else if (ch == ';') {

				var_s *var = VarGet(_parser, value_name, value_len);
				if (var == NULL) {
					CC_PRINT("ERROR: undefined variable '%s'.\n", value_name);
					return false;
				}

				if (!VarValueGetBool(_parser, var, &ival_temp)) {
					return false;
				}

			}

			else {
				parseSetError(_parser, CC_CODE_BAD_SYMBOL);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;
			}

		} else {
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;
		}

		file_bufferSkipSpace(_parser->buffer); // fixme je toto potreba?
		file_bufferGet(_parser->buffer, &ch);

		if (negation) {
			ival_temp = !ival_temp;
		}

		if (ch == _symbol_end) {
			//konec

			if (last_op == OP_OR) {
				ival = ival | ival_temp;
			}

			else if (last_op == OP_AND) {
				ival = ival & ival_temp;
			}

			else {
				ival = ival_temp;
			}

			*_value = ival;
			file_bufferNext(_parser->buffer);
			return true;

		}

		else if (ch == '(') {
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;

		}

		else if (ch == '[') {
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;
		}

		else if (ch == '|') {

			if (last_op == OP_OR) {
				ival = ival | ival_temp;
			}

			else if (last_op == OP_AND) {
				ival = ival & ival_temp;
			}

			else {
				ival = ival_temp;
			}

			last_op = OP_OR;
			file_bufferNext(_parser->buffer);
			file_bufferSkipSpace(_parser->buffer);
			continue;
		}

		else if (ch == '&') {

			if (last_op == OP_OR) {
				ival = ival | ival_temp;
			}

			else if (last_op == OP_AND) {
				ival = ival & ival_temp;
			}

			else {
				ival = ival_temp;
			}

			last_op = OP_AND;
			file_bufferNext(_parser->buffer);
			file_bufferSkipSpace(_parser->buffer);
			continue;
		}

		else {

			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));

			return false;
		}
	}
	return false;
}

bool parseValueBool(parser_s *_parser, char *_value, size_t *_value_len) {
	char ch;

	file_bufferGet(_parser->buffer, &ch);

	if (!isdigit(ch) && ch != '-') {
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;

	}

	size_t i = 0;

	_value[i++] = ch;

	while (true) {

		if (FILEBUFFER_OK == file_bufferNext(_parser->buffer)
				&& FILEBUFFER_OK == file_bufferGet(_parser->buffer, &ch)) {

			if (!isalpha(ch)) {
				*_value_len = i;
				return true;
			}

			_value[i++] = ch;
		} else {
			break;
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
