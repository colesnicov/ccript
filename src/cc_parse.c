/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file cc_parse.c
 * @brief Implementace funkci pro parsovani skriptu
 *
 * @version 1b0
 * @date 26.06.2022
 *
 * @author Denis Colesnicov <eugustus@gmail.com>
 *
 * @copyright MIT License
 *
 */

#include <ccript/cc_function.h>
#include <ccript/cc_parseIf.h>
#include <ccript/cc_parseWhile.h>
#include "ccript/cc_buffer.h"
#include "ccript/cc_configs.h"
#include "ccript/cc_parser.h"
#include "ccript/cc_types.h"
#include "ccript/cc_var.h"
#include "ccript/common.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

bool parseBlock(parser_s *_parser, char _end_char) {
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

			if (!parseBlock(_parser, '}')) {
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

bool parseVarAssign(parser_s *_parser, char *_var_name, size_t _var_len) {
	// funkce priradi hodnotu prommene ktera byla definovana jiz drive

	parseSetErrorPos(_parser, parseGetPos(_parser));

	bufferNext(_parser);
	bufferSkipSpace(_parser);

	var_s *var = VarGet(_parser, _var_name, _var_len);
	if (var == NULL) {
		parseSetError(_parser, CC_CODE_VAR_NOT_DEFINED);
		CC_PRINT("ERROR: undefined variable '%s'!", _var_name);

		return false;
	}

	if (var->type == CC_TYPE_BOOL) {

		bool ival = 0;
		if (!parseVarArgsBool(_parser, ';', &ival)) {
			return false;
		}

		if (!VarValueSetBool(_parser, var, ival)) {
			return false;
		}

		bufferNext(_parser);

		return true;

	}

	else if (var->type == CC_TYPE_INT) {

		int ival = 0;
		if (!parseVarArgsInt(_parser, ';', &ival)) {
			return false;
		}

		if (!VarValueSetInt(_parser, var, ival)) {
			return false;
		}

		bufferNext(_parser);

		return true;

	}

	else if (var->type == CC_TYPE_FLOAT) {

		float fval = 0;
		if (!parseVarArgsFloat(_parser, ';', &fval)) {
			return false;
		}

		if (!VarValueSetFloat(_parser, var, fval)) {
			return false;
		}

		bufferNext(_parser);

		return true;

	}

	else if (var->type == CC_TYPE_CHAR) {

		char cval = 0;
		if (!parseVarArgsChar(_parser, ';', &cval)) {
			return false;
		}

		if (!VarValueSetChar(_parser, var, cval)) {
			return false;
		}

		bufferNext(_parser);

		return true;

	}

	if (var->type == CC_TYPE_STRING) {

		size_t fval_len = 50;
		char fval[CONFIG_CC_STRING_SIZE_CAPS] = { '\0' };
		if (!parseVarArgsString(_parser, ';', fval, &fval_len)) {
			return false;
		}

		if (!VarValueSetString(_parser, var, fval, fval_len)) {
			return false;
		}

		bufferNext(_parser);

		return true;

	}

	if (var->type == CC_TYPE_ARRAY) {
		parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
		return false;
	}

	else {
		parseSetError(_parser, CC_CODE_TYPE_UNKNOWN);
		return false;
	}

}

void parseSetErrorPos(parser_s *_parser, size_t _pos) {
	_parser->error_pos = _pos;
}

size_t parseGetPos(parser_s *_parser) {
	return ((size_t) _parser->buffer->offset) + _parser->buffer->fpos - _parser->buffer->length;
}
void parseSetError(parser_s *_parser, cc_code_t _error) {
	_parser->error = _error;
}

bool parseSkipComment(parser_s *_parser) {
	char ch = 0;

	bufferGet(_parser, &ch);

	if (ch == '/') {

		bufferNext(_parser);
		bufferGet(_parser, &ch);

		if (ch == '/') {
			// komentar

#if CONFIG_CC_PRINT_COMMENT
			char buf[CONFIG_CC_COMMENT_SIZE_CAPS] = { '\0' };
			size_t len = 0;
#endif

			bufferNext(_parser);
			bufferSkipSpace(_parser);

			while (bufferValid(_parser)) {

				bufferGet(_parser, &ch);

				if (ch == '\n') {
					// konec radku

#if CONFIG_CC_PRINT_COMMENT
					CC_PRINT("\\comment: '%s'\n", buf);
					memset(buf, 0, CC_COMMENT_SIZE);
#endif

					bufferNext(_parser);
					bufferSkipSpace(_parser);
					bufferGet(_parser, &ch);

					if (ch == '/') {
						return parseSkipComment(_parser);
					}

					return true;
				}

#if CONFIG_CC_PRINT_COMMENT
				if (len >= CC_COMMENT_SIZE) {
					CC_PRINT("\\comment: '%s'\n", buf);
					memset(buf, 0, CC_COMMENT_SIZE);
					len = 0;
				}

				buf[len++] = ch;
#endif

				bufferNext(_parser);
			}

		}

	}

	parseSetErrorPos(_parser, parseGetPos(_parser));
	parseSetError(_parser, CC_CODE_NOT_COMMENT);
	return false;

}

void parseSkipNewLine(parser_s *_parser) {
	char ch = 0;

	while (bufferGet(_parser, &ch)) {
		if (ch == '\n' || isspace(ch)) {
			bufferNext(_parser);
		} else {
			break;
		}
	}
}

void bufferSkipSpace(parser_s *_parser) {
	char ch = 0;
	while (bufferGet(_parser, &ch)) {
		if (isspace(ch)) {
			_parser->buffer->fpos++;
		} else {
			break;
		}
	}
}

bool parseIdentifier(parser_s *_parser, char *_name, size_t *_len) {

	size_t i = 0;
	char ch = 0;

	bufferGet(_parser, &ch);

	while (isalpha(ch) || isdigit(ch) || ch == '_') {
		_name[i++] = ch;
		if (!bufferNext(_parser) || !bufferGet(_parser, &ch)) {
			return false;
		}
	}

	*_len = i;
	return true;
}

bool parseVarDelete(parser_s *_parser, char *_var_name) {

	bufferSkipSpace(_parser);

	char ch = '\0';

	bufferGet(_parser, &ch);

	if (!isalpha(ch)) {
		parseSetErrorPos(_parser, parseGetPos(_parser));
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		return false;
	}

	char var_name[CC_KEYWORD_SIZE] = { '\0' };
	size_t var_name_len = 0;

	if (!parseIdentifier(_parser, var_name, &var_name_len)) {
		return false;
	}

	if (var_name_len == 0) {
		parseSetErrorPos(_parser, parseGetPos(_parser));
		parseSetError(_parser, CC_CODE_KEYWORD);
		return false;
	}

	bufferSkipSpace(_parser);

	bufferGet(_parser, &ch);

	if (ch != ';') {
		parseSetErrorPos(_parser, parseGetPos(_parser));
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		return false;
	}

	bufferNext(_parser);

	return VarFindAndDestroy(_parser, var_name, var_name_len);
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
