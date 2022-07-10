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

#include "ccript/cc_buffer.h"
#include "ccript/cc_configs.h"
#include "ccript/cc_parser.h"
#include "ccript/cc_types.h"
#include "ccript/cc_var.h"
#include "ccript/common.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

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

	while (isalpha(ch) || isdigit(ch)) {
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
