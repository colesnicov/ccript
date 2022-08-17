/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file cc_stdlib.c
 * @brief Implementace  standardnich 'C' funkci pro volani ze skript.
 *
 * @version 1b0
 * @date 26.06.2022
 *
 * @author Denis Colesnicov <eugustus@gmail.com>
 *
 * @copyright MIT License
 *
 */

#include <ccript/cc_configs.h>
#include <ccript/cc_log.h>
#include <ccript/cc_parser.h>
#include <ccript/cc_stdlib.h>
#include <ccript/cc_types.h>
#include <ccript/cc_var_ext.h>
#include <ccript/ccript.h>
#include <ccript/common.h>
#include <stdint.h>
#include "ccript/cc_var.h"
#include CONFIG_CC_INCLUDE_SNPRINTF
#include <float.h>
#include <string.h>

bool stdlib_registrate(parser_s *_parser, void *_args) {
	bool ret = true;
	ret &= cc_registerFunction(_parser, "dump", 4, stdlib_dump, _args);
	ret &= cc_registerFunction(_parser, "millis", 6, stdlib_millis, _args);
	ret &= cc_registerFunction(_parser, "sleep", 5, stdlib_sleep, _args);
	ret &= cc_registerFunction(_parser, "exit", 5, stdlib_exit, _args);
	ret &= cc_registerFunction(_parser, "cast", 4, stdlib_cast, _args);
	ret &= cc_registerFunction(_parser, "print", 5, stdlib_print, _args);
	ret &= cc_registerFunction(_parser, "println", 7, stdlib_println, _args);
	ret &= cc_registerFunction(_parser, "system", 6, stdlib_system, _args);
	ret &= cc_registerFunction(_parser, "strlen", 6, stdlib_strlen, _args);
	ret &= cc_registerFunction(_parser, "strcat", 6, stdlib_strcat, _args);

	return ret;
}

var_s* stdlib_DebugInfo(parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args) {

	if (_vars_count == 0) {
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		CC_PRINT("ERROR: no arguments!\n");
		return NULL;
	}

	if (_vars[0]->type != CC_TYPE_STRING) {
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		CC_PRINT("ERROR: bad argument type!\n");
		return NULL;
	}

	size_t len = strlen(_vars[0]->data);

	if (len == 0) {
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		CC_PRINT("ERROR: empty argument value!\n");
		return false;

	}

	var_s *var = VarCreate("@", 1, CC_TYPE_INT, _parser->depth);

	if (!var) {
		return NULL;
	}

	if (len == 12 && strncmp(_vars[0]->data, "parser.depth", len) == 0) {

		if (!VarValueSetInt(_parser, var, _parser->depth)) {
			VarDestroy(var);
			return NULL;
		}

	}

	else if (len == 9 && strncmp(_vars[0]->data, "var.depth", len) == 0) {

		if (_vars_count < 2) {

			parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
			CC_PRINT("ERROR: wrong number of arguments!\n");

			VarDestroy(var);
			return NULL;
		}

		if (!VarValueSetInt(_parser, var, _vars[1]->scope)) {

			VarDestroy(var);
			return NULL;
		}

	}

	else if (len == 8 && strncmp(_vars[0]->data, "var.size", len) == 0) {

		if (_vars_count != 2) {
			parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
			CC_PRINT("ERROR: wrong number of arguments!\n");

			VarDestroy(var);
			return NULL;
		}

		int siz = VarGetSize(_vars[1]);

		if (!VarValueSetInt(_parser, var, siz)) {
			VarDestroy(var);
			return NULL;
		}

	}

	else if (len == 11 && strncmp(_vars[0]->data, "var.sizeAll", len) == 0) {

		if (_vars_count > 1) {
			parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
			CC_PRINT("ERROR: wrong number of arguments!\n");

			VarDestroy(var);
			return NULL;
		}

		int siz = VarGetSizeAll(_parser);

		if (!VarValueSetInt(_parser, var, siz)) {
			VarDestroy(var);
			return NULL;
		}

	}

	else {
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		CC_PRINT("ERROR: unknown command '%s'!\n", (char* ) _vars[0]->data);

		VarDestroy(var);
		return NULL;
	}

	return var;
}

var_s* stdlib_exit(parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args) {
	_parser->error = CC_CODE_ABORT;
	return NULL;
}

var_s* stdlib_system(parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args) {

	if (_vars_count == 0) {
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		CC_PRINT("ERROR: no arguments!\n");
		return NULL;
	}

	if (_vars[0]->type != CC_TYPE_STRING) {
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		CC_PRINT("ERROR: bad argument type!\n");
		return NULL;
	}

	size_t len = strlen(_vars[0]->data);

	if (len == 0) {
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		CC_PRINT("ERROR: empty argument value!\n");
		return false;

	}

	if (len == 5 && strncmp(_vars[0]->data, "abort", len) == 0) {

		_parser->error = CC_CODE_ABORT;
		return NULL;
	}

	else if (len == 4 && strncmp(_vars[0]->data, "exit", len) == 0) {

		_parser->error = CC_CODE_ABORT;
		return NULL;
	}

	var_s *var = VarCreate("@", 1, CC_TYPE_INT, _parser->depth);

	if (!var) {
		return NULL;
	}

	else if (len == 6 && strncmp(_vars[0]->data, "millis", len) == 0) {

		int siz = VarGetSize(_vars[1]);

		if (!VarValueSetInt(_parser, var, siz)) {
			VarDestroy(var);
			return NULL;
		}

	}

	else {
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		CC_PRINT("ERROR: unknown command '%s'!\n", (char* ) _vars[0]->data);

		VarDestroy(var);
		return NULL;
	}

	return var;
}

var_s* stdlib_cast(parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args) {

	if (_vars_count == 0) {
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		CC_PRINT("ERROR: no arguments!\n");
		return NULL;
	}

	if (_vars[0]->type != CC_TYPE_STRING) {
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		CC_PRINT("ERROR: bad argument type!\n");
		return NULL;
	}

	size_t len = strlen(_vars[0]->data);

	if (len == 0) {
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		CC_PRINT("ERROR: empty argument value!\n");
		return false;

	}

	var_s *var = NULL;

	if (len == 3 && strncmp(_vars[0]->data, "int", len) == 0) {
		var = VarCastToInt(_parser, _vars[1]);
	}

	if (len == 4 && strncmp(_vars[0]->data, "long", len) == 0) {
		var = VarCastToLong(_parser, _vars[1]);
	}

	else if (len == 5 && strncmp(_vars[0]->data, "float", len) == 0) {
		var = VarCastToFloat(_parser, _vars[1]);
	}

	else if (len == 4 && strncmp(_vars[0]->data, "char", len) == 0) {
		var = VarCastToChar(_parser, _vars[1]);
	}

	else if (len == 6 && strncmp(_vars[0]->data, "string", len) == 0) {
		var = VarCastToString(_parser, _vars[1]);
	}

	else if (len == 4 && strncmp(_vars[0]->data, "bool", len) == 0) {
		var = VarCastToBool(_parser, _vars[1]);
	}

	else {
		CC_PRINT("ERROR: unknown type '%s'!\n", (char* )_vars[0]->data);
	}

	return var;
}

var_s* stdlib_strlen(parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args) {

	if (_vars_count == 0) {
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		CC_PRINT("ERROR: no arguments!\n");
		return NULL;
	}

	if (_vars[0]->type != CC_TYPE_STRING) {
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		CC_PRINT("ERROR: bad argument type!\n");
		return NULL;
	}

	size_t len = strlen(_vars[0]->data);

	if (len == 0) {
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		CC_PRINT("ERROR: empty argument value!\n");
		return false;

	}

	var_s *var = VarCreate("_", 1, CC_TYPE_INT, _parser->depth);

	if (var == NULL) {
		return NULL;
	}

	if (!VarValueSetInt(_parser, var, len)) {
		VarDestroy(var);
		return NULL;
	}

	return var;
}

var_s* stdlib_strcat(parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args) {

	if (_vars_count < 2) {
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		CC_PRINT("ERROR: wrong number of arguments!\n");
		return NULL;
	}

	size_t len_total = 0;
	char buf[CONFIG_CC_STRING_SIZE_CAPS] = { '\0' };
	char buf2[CONFIG_CC_STRING_SIZE_CAPS] = { '\0' };

	size_t len = 0;

	for (uint8_t i = 0; i < _vars_count; i++) {

		memset(buf2, '\0', sizeof(char) * CC_STRING_SIZE);

		if (!_vars[i]->valid) {
			parseSetError(_parser, CC_CODE_VAR_NOT_ASSIGNED);
			CC_PRINT("ERROR: uninitialized variable '%s'!", _vars[i]->name);
			return NULL;
		}

		if (_vars[i]->type == CC_TYPE_BOOL) {
			if (*(bool*) (_vars[i]->data)) {
				buf2[0] = '1';
			} else {
				buf2[0] = '0';
			}
			len = 1;
		} else if (_vars[i]->type == CC_TYPE_CHAR) {
			len = 1;
			buf2[0] = *(char*) _vars[i]->data;
		} else if (_vars[i]->type == CC_TYPE_FLOAT) {
			len = (size_t) snprintf(buf2, CONFIG_CC_STRING_SIZE_CAPS, "%.*f",
			CONFIG_CC_FLOAT_EXP_LEN, *(float*) (_vars[i]->data));
		} else if (_vars[i]->type == CC_TYPE_INT) {
			len = (size_t) snprintf(buf2, CONFIG_CC_STRING_SIZE_CAPS, "%d",
					*(int*) (_vars[i]->data));
		} else if (_vars[i]->type == CC_TYPE_STRING) {
			len = strlen(_vars[i]->data);
			memcpy(buf2, _vars[i]->data, len);
		} else if (_vars[i]->type == CC_TYPE_ARRAY) {
			CC_PRINT("ERROR: not supported!\n");
			parseSetError(_parser, CC_CODE_LOGIC);
			return NULL;

		}

		if (len_total + len > CC_STRING_SIZE) {
			CC_PRINT("ERROR: string is too long. @see CC_STRING_SIZE\n");
			return NULL;
		}

		memcpy(buf + len_total, buf2, len);
		len_total += len;

	}

	var_s *var = VarCreate("@", 1, CC_TYPE_STRING, _parser->depth);

	if (var == NULL) {
		return NULL;
	}

	if (!VarValueSetString(_parser, var, buf, len_total)) {
		VarDestroy(var);
		return NULL;
	}

	return var;

}

var_s* stdlib_print(parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args) {

	if (_vars_count != 1) {
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		CC_PRINT("ERROR: wrong number of arguments!\n");
		return NULL;
	}

	char buf[CONFIG_CC_STRING_SIZE_CAPS] = { '\0' };

	if (_vars[0]->type == CC_TYPE_BOOL) {
		snprintf(buf, CONFIG_CC_STRING_SIZE_CAPS, "%d", *(bool*) _vars[0]->data);
	}

	else if (_vars[0]->type == CC_TYPE_CHAR) {
		snprintf(buf, CONFIG_CC_STRING_SIZE_CAPS, "%c", *(char*) _vars[0]->data);
	}

	else if (_vars[0]->type == CC_TYPE_FLOAT) {
		snprintf(buf, CONFIG_CC_STRING_SIZE_CAPS, "%.*f", CONFIG_CC_FLOAT_EXP_LEN,
				*(float*) _vars[0]->data);
	}

	else if (_vars[0]->type == CC_TYPE_INT) {
		snprintf(buf, CONFIG_CC_STRING_SIZE_CAPS, "%d", *(int*) _vars[0]->data);
	}

	else if (_vars[0]->type == CC_TYPE_STRING) {
		snprintf(buf, CONFIG_CC_STRING_SIZE_CAPS, "%s", (char*) _vars[0]->data);
	}

	CC_PRINT("%s", buf);

	return NULL;
}

var_s* stdlib_println(parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args) {

	if (_vars_count != 1) {
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		CC_PRINT("ERROR: wrong number of arguments!\n");
		return NULL;
	}

	char buf[CONFIG_CC_STRING_SIZE_CAPS] = { '\0' };

	if (_vars[0]->type == CC_TYPE_BOOL) {
		snprintf(buf, CONFIG_CC_STRING_SIZE_CAPS, "%d", *(bool*) _vars[0]->data);
	}

	else if (_vars[0]->type == CC_TYPE_CHAR) {
		snprintf(buf, CONFIG_CC_STRING_SIZE_CAPS, "%c", *(char*) _vars[0]->data);
	}

	else if (_vars[0]->type == CC_TYPE_FLOAT) {
		snprintf(buf, CONFIG_CC_STRING_SIZE_CAPS, "%.*f", CONFIG_CC_FLOAT_EXP_LEN,
				*(float*) _vars[0]->data);
	}

	else if (_vars[0]->type == CC_TYPE_INT) {
		snprintf(buf, CONFIG_CC_STRING_SIZE_CAPS, "%d", *(int*) _vars[0]->data);
	}

	else if (_vars[0]->type == CC_TYPE_STRING) {
		snprintf(buf, CONFIG_CC_STRING_SIZE_CAPS, "%s", (char*) _vars[0]->data);
	}

	CC_PRINT("%s\n", buf);

	return NULL;
}

var_s* stdlib_dump(parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args) {

	if (_vars_count != 1) {
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		CC_PRINT("ERROR: wrong number of arguments!\n");
		return NULL;
	}

	VarDump(_vars[0]);

	return NULL;
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
