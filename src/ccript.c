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

bool cc_init(parser_s *_parser) {

	do {
		if (!VarInit(_parser)) {
			CC_PRINT("ccript error 2!\n");
			break;
		}

		if (!funcInit(_parser)) {
			CC_PRINT("ccript error 3!\n");
			break;
		}

		if (!blockInit(_parser)) {
			CC_PRINT("ccript error 3!\n");
			break;
		}

		return true;

	} while (0);

	cc_deinit(_parser);

	return false;
}

void cc_deinit(parser_s *_parser) {
	blockDeinit(_parser);
	VarDeinit(_parser);
	funcDeinit(_parser);
	bufferClose(_parser);
}

bool cc_parse(parser_s *_parser, const char *_path) {
	_parser->error = CC_CODE_OK;
	_parser->error_pos = 0;

	if (!bufferInit(_parser, _path, CONFIG_CC_BUFFER_LEN)) {
		return false;
	}

	var_s *ret_var = parseBlock(_parser, 0);
	VarDestroy(ret_var);

	if (_parser->error >= CC_CODE_ERROR && _parser->error_pos >= _parser->buffer->fsize) {
		if (_parser->error_pos >= _parser->buffer->fsize) {
			// Kdyz jsme na konci souboru, neni to chyba
			_parser->error = CC_CODE_OK;
		} else {
			return false;
		}
	}

	VarClean(_parser);
	bufferClose(_parser);

	return true;
}

cc_code_t cc_errorGetCode(parser_s *_parser) {
	return _parser->error;
}

bool cc_errorHas(parser_s *_parser) {
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

		case CC_CODE_NO_FILE:
			return "CC_CODE_NO_FILE";

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

		case CC_CODE_NOT_IMPLEMENTED:
			return "CC_CODE_NOT_IMPLEMENTED";

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

