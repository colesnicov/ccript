/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file ccript.c
 * @brief Implementace verejnych funkci pro praci s interpreterem.
 *
 * @version 1b0
 * @date 26.06.2022
 *
 * @author Denis Colesnicov <eugustus@gmail.com>
 *
 * @copyright MIT License
 *
 */

#include <ccript/cc_buffer.h>
#include <ccript/cc_configs.h>
#include <ccript/cc_function.h>
#include <ccript/cc_log.h>
#include <ccript/cc_parseIf.h>
#include <ccript/cc_parser.h>
#include <ccript/cc_parseWhile.h>
#include <ccript/cc_types.h>
#include <ccript/common.h>
#include <ccript/cc_var.h>
#include "ccript/ccript.h"

#include <stdbool.h>
#include <string.h>

bool cc_init(parser_s *_parser, char *_path) {

	do {
		if (!bufferInit(_parser, _path, CONFIG_CC_BUFFER_SIZE_CAPS)) {
			break;
		}

		if (!VarInit(_parser)) {
			break;
		}

		if (!funcInit(_parser)) {
			break;
		}

		return true;

	} while (0);

	cc_deinit(_parser);

	return false;
}

void cc_deinit(parser_s *_parser) {
	VarDeinit(_parser);
	funcDeinit(_parser);
	bufferClose(_parser);
}

bool cc_parse(parser_s *_parser, char _end_char) {

	if (cc_errorHas(_parser)) {
		return false;
	}

	/**
	 * @var char keyword_name[CONFIG_CC_KEYWORD_SIZE_CAPS]
	 * @brief Nalezeny prvni vyraz ve vete
	 * @details	Typicky: type(int,string,...), function, command(if,while,...)
	 *
	 */
	char keyword_name[CONFIG_CC_KEYWORD_SIZE_CAPS] = { '\0' };

	char ch = '\0';

	parseSkipNewLine(_parser);

	bufferGet(_parser, &ch);

	uint8_t scope = _parser->depth;

	while (bufferValid(_parser)) {

		ch = 0; // fixme toto asi netreba. kdyz dojde k chybe s IO tak se musi ukoncit!?
		memset(keyword_name, '\0', CC_KEYWORD_SIZE);
		size_t keyword_len = 0;

		parseSkipNewLine(_parser);

		bufferGet(_parser, &ch);

		if (isdigit(ch)) {
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			parseSetErrorPos(_parser, parseGetPos(_parser));

			return false;
		}

		if (_end_char != 0 && ch == _end_char) {
			// Zrejme konec bloku

			_parser->depth = scope;
			parseClearScope(_parser);
			parseSetError(_parser, CC_CODE_OK); // fixme je toto potreba?

			return true;
		}

		if (ch == '{') {
			// Zrejme zanoreny blok

			bufferNext(_parser);

			_parser->depth++;

			if (!cc_parse(_parser, '}')) {
				return false;
			}

			_parser->depth = scope;
			parseClearScope(_parser);

			if (_parser->error == CC_CODE_OK) {
				bufferNext(_parser);

				continue;
			}

			else if (_parser->error < CC_CODE_ERROR) {
				// break/continue/return

				return true;
			}

			else {
				// nejaka chyba

				return false;
			}

		}

		else if (ch == '/') {
			// komentar

			if (!parseSkipComment(_parser)) {
				return false;
			}

			continue;

		}

		else if (ch == '+') {
			// inkrementace promenne
			// ++var;

			if (!parseVarIncrementLeft(_parser, keyword_name, keyword_len)) {
				return false;
			}

			bufferNext(_parser);

			continue;

		}

		else if (ch == '-') {
			// dekrementace promenne
			// --var;

			if (!parseVarDecrementLeft(_parser, keyword_name, keyword_len)) {
				return false;
			}

			bufferNext(_parser);

			continue;

		}

		else if (isalpha(ch)) {
			// promenna, funkce

//			size_t pos = parseGetPos(_parser);

			if (!parseIdentifier(_parser, keyword_name, &keyword_len)) {
				return false;
			}

			if (keyword_len == 0) {
				parseSetError(_parser, CC_CODE_KEYWORD);
				parseSetErrorPos(_parser, parseGetPos(_parser));
				return false;

			}

			bufferSkipSpace(_parser);

			if (keyword_len == 4 && strncmp(keyword_name, "bool", keyword_len) == 0) {
				if (ParseDefineTypeBool(_parser, keyword_name)) {
					continue;
				} else {

					return 0;
				}
			}

			else if (keyword_len == 3 && strncmp(keyword_name, "int", keyword_len) == 0) {
				if (ParseDefineTypeInt(_parser)) {
					continue;
				} else {
					return 0;
				}
			}

			else if (keyword_len == 5 && strncmp(keyword_name, "float", keyword_len) == 0) {
				if (ParseDefineTypeFloat(_parser)) {
					continue;
				} else {
					return 0;
				}
			}

			else if (keyword_len == 4 && strncmp(keyword_name, "char", keyword_len) == 0) {
				if (ParseDefineTypeChar(_parser)) {
					continue;
				} else {
					return 0;
				}
			}

			else if (keyword_len == 6 && strncmp(keyword_name, "string", keyword_len) == 0) {
				if (ParseDefineTypeString(_parser)) {
					continue;
				} else {
					return 0;
				}
			}

			else if (keyword_len == 6 && strncmp(keyword_name, "delete", keyword_len) == 0) {

				if (parseVarDelete(_parser, keyword_name)) {
					continue;
				} else {
					return 0;
				}
			}

			else if (keyword_len == 2 && strncmp(keyword_name, "if", keyword_len) == 0) {
				_parser->depth++;
				if (!parseIf(_parser)) {
					return false;
				}

				_parser->depth = scope;
				parseClearScope(_parser);

				if (_parser->error == CC_CODE_BREAK) {
					// muzu se nachazet ve smycce
					// break

					return true;
				}

				else if (_parser->error == CC_CODE_CONTINUE) {
					// muzu se nachazet ve smycce
					// continue

					return true;
				}

				else if (_parser->error == CC_CODE_RETURN) {
					// muzu se nachazet ve funkci
					// return

					return true;
				}

				else if (_parser->error >= CC_CODE_ERROR) {
					return false;
				}

				continue;

			}

			else if (keyword_len == 5 && strncmp(keyword_name, "while", keyword_len) == 0) {
				_parser->inLoop++;

				if (!parseWhile(_parser)) {
					return false;
				}
				_parser->inLoop--;

				continue;

			}

			else if (keyword_len == 5 && strncmp(keyword_name, "break", keyword_len) == 0) {
				if (!_parser->inLoop) {
					parseSetError(_parser, CC_CODE_OUT_OF_LOOP);

					return false;
				}

				parseSetError(_parser, CC_CODE_BREAK);

				return true;
			}

			else if (keyword_len == 8 && strncmp(keyword_name, "continue", keyword_len) == 0) {
				if (!_parser->inLoop) {
					parseSetError(_parser, CC_CODE_OUT_OF_LOOP);

					return false;
				}

				parseSetError(_parser, CC_CODE_CONTINUE);

				return true;
			}

			else if (keyword_len == 6 && strncmp(keyword_name, "return", keyword_len) == 0) {
				CC_PRINT("DEBUG: return\n");

				parseSetError(_parser, CC_CODE_RETURN);

				return true;
			}

			else {
				// promenna nebo funkce

				bufferSkipSpace(_parser);
				bufferGet(_parser, &ch);

				if (ch == '=') {
					// prirazeni promenne

					if (parseVarAssign(_parser, keyword_name, keyword_len)) {
						continue;
					} else {
						return 0;
					}
				}

				else if (ch == '[') {
					parseSetError(_parser, CC_CODE_BAD_SYMBOL);
					parseSetErrorPos(_parser, parseGetPos(_parser));
					return false;
				}

				else if (ch == '(') {
					// volani funkce

					var_s *var = funcCall(_parser, keyword_name, keyword_len);

					if (var != NULL) {
						VarDestroy(var);
					}

					if (_parser->error >= CC_CODE_ERROR) {
						return false;
					}

					bufferNext(_parser);
					bufferSkipSpace(_parser);
					bufferGet(_parser, &ch);

					if (ch != ';') {
						parseSetError(_parser, CC_CODE_BAD_SYMBOL);
						parseSetErrorPos(_parser, parseGetPos(_parser));

						return false;
					}

					bufferNext(_parser);

					continue;
				}

				else if (ch == '+') {
					// inkrementace promenne
					// var++;

					if (!parseVarIncrementRight(_parser, keyword_name, keyword_len)) {
						return false;
					}

					bufferNext(_parser);

					continue;

				}

				else if (ch == '-') {
					// Decrementace promenne
					// var--;

					if (!parseVarDecrementRight(_parser, keyword_name, keyword_len)) {
						return false;
					}

					bufferNext(_parser);

					continue;

				}

				else if (ch == '/') {
					// Deleni promenne
					// var /= 20;

					if (!parseVarDivideRight(_parser, keyword_name, keyword_len)) {
						return 0;
					}

					bufferNext(_parser);

					continue;

				}

				else if (ch == '*') {
					// Nasobeni promenne
					// var *= 20;

					if (!parseVarMultiplyRight(_parser, keyword_name, keyword_len)) {
						return 0;
					}

					bufferNext(_parser);

					continue;

				}

				else {
					parseSetError(_parser, CC_CODE_KEYWORD);
					parseSetErrorPos(_parser, parseGetPos(_parser)); // fixme tady se udava spatna pozice.
					return 0;
				}

			}

		} else if (_parser->buffer->fsize > _parser->error_pos) {
			CC_PRINT("ERROR: unexpected symbol '%c'\n", ch);
			parseSetError(_parser, CC_CODE_BAD_SYMBOL);
			return false;
		} else {

			return true;
		}
	}

	parseSetError(_parser, CC_CODE_LOGIC);
	parseSetErrorPos(_parser, parseGetPos(_parser));

	return 0;

}

cc_code_t cc_errorGetCode(parser_s *_parser) {
	return _parser->error;
}

bool cc_errorHas(parser_s *_parser) {

	if (_parser->error == CC_CODE_IO) {
		// Kdyz dojde na konec souboru, neni to chyba

		return _parser->error_pos < _parser->buffer->fsize;
	}

	return _parser->error >= CC_CODE_ERROR;
}

size_t cc_errorGetPos(parser_s *_parser) {
	return _parser->error_pos;
}

const char* cc_errorToString(cc_code_t _err_code) {

	switch (_err_code) {

		case CC_CODE_FUNC_ARGS_ERROR:
			return "CC_CODE_FUNC_ARGS_ERROR";

		case CC_CODE_FUNC_BAD_TYPE:
			return "CC_CODE_FUNC_BAD_TYPE";

		case CC_CODE_NOT_BOOL:
			return "CC_CODE_NOT_BOOL";

		case CC_CODE_TYPE_UNKNOWN:
			return "CC_CODE_TYPE_UNKNOWN";

		case CC_CODE_VALUE_EMPTY:
			return "CC_CODE_VALUE_EMPTY";

		case CC_CODE_STRING_TOO_LONG:
			return "CC_CODE_STRING_TOO_LONG";

		case CC_CODE_OK:
			return "CC_CODE_OK";

		case CC_CODE_RETURN:
			return "CC_CODE_RETURN";

		case CC_CODE_CONTINUE:
			return "CC_CODE_CONTINUE";

		case CC_CODE_BREAK:
			return "CC_CODE_BREAK";

		case CC_CODE_ERROR:
			return "CC_CODE_ERROR";

		case CC_CODE_BAD_SYMBOL:
			return "CC_CODE_BAD_SYMBOL";

		case CC_CODE_KEYWORD:
			return "CC_CODE_KEYWORD";

		case CC_CODE_LOGIC:
			return "CC_CODE_LOGIC";

		case CC_CODE_IO:
			return "CC_CODE_IO";

		case CC_CODE_FUNC_NOT_DEFINED:
			return "CC_CODE_FUNC_NOT_DEFINED";

		case CC_CODE_FUNC_EXISTS:
			return "CC_CODE_FUNC_EXISTS";

		case CC_CODE_VAR_NOT_DEFINED:

			return "CC_CODE_VAR_NOT_DEFINED";

		case CC_CODE_VAR_NOT_ASSIGNED:

			return "CC_CODE_VAR_NOT_ASSIGNED";

		case CC_CODE_VAR_BAD_TYPE:
			return "CC_CODE_VAR_BAD_TYPE";

		case CC_CODE_VAR_EXISTS:
			return "CC_CODE_VAR_EXISTS";

		case CC_CODE_NOT_MEM:
			return "CC_CODE_NOT_MEM";

		case CC_CODE_NOT_COMMENT:
			return "CC_CODE_NOT_COMMENT";

		case CC_CODE_NOT_INTEGER:
			return "CC_CODE_NOT_INTEGER";

		case CC_CODE_NOT_FLOAT:
			return "CC_CODE_NOT_FLOAT";

		case CC_CODE_NOT_CHAR:
			return "CC_CODE_NOT_CHAR";

		case CC_CODE_NOT_STRING:
			return "CC_CODE_NOT_STRING";

		case CC_CODE_NOT_ARRAY:
			return "CC_CODE_NOT_ARRAY";

		case CC_CODE_OUT_OF_LOOP:
			return "CC_CODE_OUT_OF_LOOP";

		case CC_CODE_ABORT:
			return "CC_CODE_ABORT";

	}

	return "CC_CODE_UNKNOWN";
}

void cc_abort(parser_s *_parser) {
	/// fixme tady neco jineho.
	_parser->error = CC_CODE_ABORT;
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

