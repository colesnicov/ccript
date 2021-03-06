/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file cc_parseIf.c
 * @brief Implementace funkci pro parsovani podminky 'IF'.
 *
 * @version 1b0
 * @date 26.06.2022
 *
 * @author Denis Colesnicov <eugustus@gmail.com>
 *
 * @copyright MIT License
 *
 */

#include <ccript/ccript.h>
#include "ccript/cc_parseIf.h"
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

void cleanIf(var_s **args, uint8_t *args_count) {

	for (uint8_t i = 0; i < *args_count; i++) {
		CC_PRINT("DEBUG: destroy var: '%s'\n", args[i]->name);
		VarDestroy(args[i]);
	}
	*args_count = 0;
}

bool parseIfArguments(parser_s *_parser, bool *_cond_passed) {

	char ch = 0;

	bool cond_result_left = true;
	bool cond_result_right = false;

	bufferNext(_parser);
	bufferSkipSpace(_parser);

	while (bufferValid(_parser)) {

		bufferGet(_parser, &ch);

		if (ch == '!') {
			// negace

			if (!parseIfArguments(_parser, &cond_result_right)) {

				return false;
			}

			cond_result_right = !cond_result_right;

		}

		else if (ch == '(') {
			// zanoreni

			if (!parseIfArguments(_parser, &cond_result_right)) {

				return false;
			}
		}

		else if (ch == '=') {
			// rovna se, tady nesmi
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;
		}

		else if (ch == '<') {
			// mensi, tady nesmi
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;
		}

		else if (ch == '>') {
			// vetsi, tady nesmi
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;
		}

		else if (ch == '&') {
			// and, tady nesmi
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;
		}

		else if (ch == '|') {
			// nebo, tady nesmi

			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;
		}

		else {
			// ostatni

			if (!parseIfPair(_parser, &cond_result_right)) {
				return false;
			}
		}

		bufferSkipSpace(_parser);

		bufferGet(_parser, &ch);

		if (ch == '&') {

			bufferNext(_parser);
			bufferGet(_parser, &ch);
			if (ch == '&') {
				// operator '&&'

				bool value = false;
				if (!parseIfArguments(_parser, &value)) {
					return false;
				}

				cond_result_right = (cond_result_right == value);
				*_cond_passed = (cond_result_left == cond_result_right);
				return true;
			}

			else {
				parseSetError(_parser, CC_CODE_BAD_SYMBOL);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;
			}
		}

		else if (ch == ')') {
			// konec argumentu

			*_cond_passed = (cond_result_left == cond_result_right);

			return true;
		} else {
			// chyba syntaxe?

			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;
		}

	}

	parseSetError(_parser, CC_CODE_LOGIC);
	parseSetErrorPos(_parser, parseGetPos(_parser));

	return false;
}

bool parseIfArg(parser_s *_parser, float *_cond_arg) {

	char ch;

	float left_arg = 0.0f;

	bool negation = false;

	bufferGet(_parser, &ch);

	if (ch == '!') {

		while (bufferNext(_parser)) {
			negation = !negation;

			bufferSkipSpace(_parser);
			bufferGet(_parser, &ch);

			if (ch != '!') {
				break;
			}
		}

	}

	if (isdigit(ch) || ch == '-') {
		// cislo

		char value[CONFIG_CC_KEYWORD_SIZE_CAPS] = { '\0' };

		size_t value_len = 0;
		bool is_bool = false;

		if (!parseValueFloat(_parser, value, &value_len, &is_bool)) {
			return false;
		}

		if (value_len == 0) {
			parseSetError(_parser, CC_CODE_KEYWORD);
			parseSetErrorPos(_parser, parseGetPos(_parser));
			return false;

		}

		*_cond_arg = (float) atof(value);
		return true;
	}

	else if (isalpha(ch)) {
		// promenna nebo funkce

		char var_name[CONFIG_CC_KEYWORD_SIZE_CAPS] = { '\0' };
		size_t var_name_len = 0;

		parseSetErrorPos(_parser, parseGetPos(_parser));

		if (!parseIdentifier(_parser, var_name, &var_name_len)) {
			return false;
		}

		if (var_name_len == 0) {
			parseSetError(_parser, CC_CODE_KEYWORD);

			return false;
		}

		var_s *_var = NULL;
		bool var_is_int = true;

		if (var_name_len == 4 && strncmp(var_name, "true", var_name_len) == 0) {

			*_cond_arg = 1.0f;
			return true;
		}

		else if (var_name_len == 5 && strncmp(var_name, "false", var_name_len) == 0) {

			*_cond_arg = 0.0f;
			return true;
		}

		else {

			{
				// funkce nebo promenna

				bufferSkipSpace(_parser);

				bufferGet(_parser, &ch);

				if (ch == '[') {
					// pole ...

					parseSetError(_parser, CC_CODE_BAD_SYMBOL);
					parseSetErrorPos(_parser, parseGetPos(_parser));

					return false;
				}

				else if (ch == '(') {
					// volani funkce ...

					_var = funcCall(_parser, var_name, var_name_len);

					if (_parser->error > CC_CODE_RETURN) {
						VarDestroy(_var);
						return false;
					}

					bufferNext(_parser);
					bufferSkipSpace(_parser);

					var_is_int = false;

				}

				else {
					// promenna

					_var = VarGet(_parser, var_name, var_name_len);

					var_is_int = true;

				}

				if (_var == NULL) {
					return false;
				}

				if (_var->type == (cc_type_t) CC_TYPE_BOOL) {
					bool _bool = false;

					if (!VarValueGetBool(_parser, _var, &_bool)) {
						if (var_is_int == false) {
							VarDestroy(_var);
						}
						return false;
					}

					left_arg = (float) _bool;

				} else if (_var->type == CC_TYPE_CHAR) {
					char _char = 0;

					if (!VarValueGetChar(_parser, _var, &_char)) {

						if (var_is_int == false) {
							VarDestroy(_var);
						}
						return false;
					}

					left_arg = (float) _char;

				} else if (_var->type == CC_TYPE_FLOAT) {

					if (!VarValueGetFloat(_parser, _var, _cond_arg)) {

						if (var_is_int == false) {
							VarDestroy(_var);
						}
						return false;
					}
				}

				else if (_var->type == CC_TYPE_INT) {
					int _int = 0;

					if (!VarValueGetInt(_parser, _var, &_int)) {

						if (var_is_int == false) {
							VarDestroy(_var);
						}
						return false;
					}
					left_arg = (float) _int;

				} else {

					parseSetError(_parser, CC_CODE_LOGIC);
					parseSetErrorPos(_parser, parseGetPos(_parser));
					if (var_is_int == false) {
						VarDestroy(_var);
					}
					return false;
				}

			}
		}

		if (var_is_int == false) {
			VarDestroy(_var);
		}

		*_cond_arg = left_arg;

		return true;
	}

	else if (ch == '(') {
		bool cond_passed = false;
		return parseIfArguments(_parser, &cond_passed);
	}

	else {
		// chyba syntaxe

		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;
	}

}

bool parseIfPair(parser_s *_parser, bool *_cond_passed) {

	// Ziska jeden argument 'a' nebo par 'a==b'
	// Kdyz narazi na '&', '|', ')' vraci true
	// nebo kdyz narazi na dalsi serii logickych operatoru, selze.

	char ch = 0;

	float left_arg = 0.0f;

	if (!parseIfArg(_parser, &left_arg)) {
		return false;
	}

	bufferSkipSpace(_parser);
	bufferGet(_parser, &ch);

	if (ch == '=') {
		bufferNext(_parser);
		bufferGet(_parser, &ch);

		if (ch == '=') {
			bufferNext(_parser);
			bufferSkipSpace(_parser);

			float right_arg = 0;
			if (!parseIfArg(_parser, &right_arg)) {
				return false;
			}

			*_cond_passed = left_arg == right_arg;
			return true;
		}
	}

	else if (ch == '!') {
		bufferNext(_parser);
		bufferGet(_parser, &ch);

		if (ch == '=') {
			bufferNext(_parser);
			bufferSkipSpace(_parser);

			float right_arg = 0;
			if (!parseIfArg(_parser, &right_arg)) {
				return false;
			}

			*_cond_passed = (left_arg != right_arg);
			return true;
		}
	}

	else if (ch == '<') {
		bufferNext(_parser);
		bufferGet(_parser, &ch);

		float right_arg = 0;

		if (ch == '=') {
			bufferNext(_parser);
			bufferSkipSpace(_parser);

			if (!parseIfArg(_parser, &right_arg)) {
				return false;
			}

			*_cond_passed = (left_arg <= right_arg);
			return true;
		}

		bufferSkipSpace(_parser);
		if (!parseIfArg(_parser, &right_arg)) {
			return false;
		}

		*_cond_passed = left_arg < right_arg;
		return true;

	}

	else if (ch == '>') {
		bufferNext(_parser);
		bufferGet(_parser, &ch);

		float right_arg = 0;

		if (ch == '=') {
			bufferNext(_parser);
			bufferSkipSpace(_parser);

			if (!parseIfArg(_parser, &right_arg)) {
				return false;
			}

			*_cond_passed = left_arg >= right_arg;
			return true;
		} else {

			bufferSkipSpace(_parser);
			if (!parseIfArg(_parser, &right_arg)) {
				return false;
			}

			*_cond_passed = (left_arg > right_arg);
			return true;
		}

	}

	*_cond_passed = (bool) left_arg;
	return true;

}

bool parseIf(parser_s *_parser) {
	char ch = 0;

	parseSkipNewLine(_parser);

	bufferGet(_parser, &ch);

	if (ch != '(') {
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;
	}

	bool cond_passed = false;

	if (!parseIfArguments(_parser, &cond_passed)) {
		return false;
	}

	bufferNext(_parser);
	parseSkipNewLine(_parser);

	bufferGet(_parser, &ch);

	if (ch != '{') {
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return false;
	}

	if (cond_passed) {
		bufferNext(_parser);

		if (!cc_parse(_parser, '}')) {
			return false;
		}

		if (_parser->error == CC_CODE_BREAK) {

			return true;
		}

		else if (_parser->error == CC_CODE_CONTINUE) {

			return true;
		}

		else if (_parser->error == CC_CODE_RETURN) {

			return true;
		}

		else if (_parser->error >= CC_CODE_ERROR) {
			return false;
		}

		bufferNext(_parser);
		parseSkipNewLine(_parser);

		char _name[CONFIG_CC_STRING_SIZE_CAPS] = { '\0' };

		size_t pos_total = 0;
		size_t pos_f = 0;
		while (bufferValid(_parser)) {

			bufferGet(_parser, &ch);

			if (ch == 'e') {
				pos_total = _parser->buffer->offset;
				pos_f = _parser->buffer->fpos;

				memset(_name, '\0', sizeof(char) * CC_STRING_SIZE);

				size_t ll = 0;
				if (!parseIdentifier(_parser, _name, &ll)) {
					return false;
				}

				if (ll == 4 && strncmp(_name, "elif", ll) == 0) {
					// elif, preskocit
					parseSkipNewLine(_parser);
					if (!parserSkipBlock(_parser, '(', ')')) {
						return false;
					}

					parseSkipNewLine(_parser);
					if (!parserSkipBlock(_parser, '{', '}')) {
						return false;
					}

					parseSkipNewLine(_parser);

					continue;
				}

				else if (ll == 4 && strncmp(_name, "else", ll) == 0) {
					// else, preskocit
					parseSkipNewLine(_parser);
					if (!parserSkipBlock(_parser, '{', '}')) {
						return false;
					}

					parseSkipNewLine(_parser);

					return true;
				}

				else {
					// else neni. konec
					// vracim buffer do pozice pred hledanim

					_parser->buffer->fpos = pos_f;
					_parser->buffer->offset = pos_total;

					bufferReload(_parser);

					// fixme tento radek zpusobuje problem? zakomentuj ho
					parseSkipNewLine(_parser);

					return true;

				}
			}

			else {
				// else neni. konec
				return true;
			}
		}

		parseSetError(_parser, CC_CODE_LOGIC);
		parseSetErrorPos(_parser, parseGetPos(_parser));

		return false;
	} else {
		//	preskocit blok if

		if (!parserSkipBlock(_parser, '{', '}')) {
			return false;
		}

		parseSkipNewLine(_parser);

		bufferGet(_parser, &ch);

		if (ch == 'e') {

			size_t pos_total = _parser->buffer->offset;
			size_t pos_f = _parser->buffer->fpos;
			char _name[51] = { '\0' };

			size_t ll = 0;
			if (!parseIdentifier(_parser, _name, &ll)) {
				return false;
			}

			if (ll == 4 && strncmp(_name, "elif", ll) == 0) {

				if (!parseIf(_parser)) {
					return false;
				}

				return true;
			}

			else if (ll == 4 && strncmp(_name, "else", ll) == 0) {
				// preskocit

				parseSkipNewLine(_parser);

				bufferGet(_parser, &ch);

				if (ch != '{') {
					parseSetError(_parser, CC_CODE_BAD_SYMBOL);
					parseSetErrorPos(_parser, parseGetPos(_parser));

					return false;
				}

				bufferNext(_parser);

				_parser->depth++;

				if (!cc_parse(_parser, '}')) {
					return false;
				}

				if (_parser->error == CC_CODE_BREAK) {
					// fixme kontrolovat jestli se zrovna nachazim ve smycce (a switch)?
					return true;
				}

				else if (_parser->error == CC_CODE_CONTINUE) {
					return true;
				}

				else if (_parser->error == CC_CODE_RETURN) {
					return true;
				}

				else if (_parser->error >= CC_CODE_ERROR) {
					return false;
				}

				bufferNext(_parser);
				parseSkipNewLine(_parser);

				return true;

			} else {
				// neni 'else', vratit buffer do pozice pred hledanim

				_parser->buffer->fpos = pos_f;
				_parser->buffer->offset = pos_total;

				bufferReload(_parser);

				parseSkipNewLine(_parser);

				return true;
			}
		} else {

			return true;
		}

	}

}

bool parserSkipBlock(parser_s *_parser, char _start_char, char _end_char) {
	char ch = 0;
	uint8_t depth = 1;

		bufferNext(_parser);

	while (bufferValid(_parser)) {

		bufferGet(_parser, &ch);

		if (ch == '/') {
			// komentar/operator

			if (!parseSkipComment(_parser)) {
				return false;
			}

			continue;
		}

		if (ch == _start_char) {
			depth++;

			bufferNext(_parser);
			continue;
		}

		else if (ch == _end_char) {

			if (depth > 1) {
				depth--;
				bufferNext(_parser);
				continue;
			}

			else {
				bufferNext(_parser);
				return true;
			}

		}
		bufferNext(_parser);

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
