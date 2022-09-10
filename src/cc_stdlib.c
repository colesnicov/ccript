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

#include <float.h>
#include <string.h>

bool cc_stdlib_registrate(parser_s *_parser, void *_args) {
	bool ret = true;
	ret &= cc_registerFunction(_parser, "dump", 4, stdlib_dump, _args);
	ret &= cc_registerFunction(_parser, "exit", 5, stdlib_exit, _args);
	ret &= cc_registerFunction(_parser, "cast", 4, stdlib_cast, _args);
	ret &= cc_registerFunction(_parser, "print", 5, stdlib_print, _args);
	ret &= cc_registerFunction(_parser, "println", 7, stdlib_println, _args);
	ret &= cc_registerFunction(_parser, "system", 6, stdlib_system, _args);
	ret &= cc_registerFunction(_parser, "strlen", 6, stdlib_strlen, _args);
	ret &= cc_registerFunction(_parser, "strcat", 6, stdlib_strcat, _args);
	ret &= cc_registerFunction(_parser, "env", 3, stdlib_env, _args);

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

var_s* stdlib_env(parser_s *_parser, var_s **_vars, uint8_t _vars_count, void *_args) {

	if (_vars_count != 1) {
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		CC_PRINT("ERROR: bad arguments count!\n");
		return NULL;
	}

	char str[CC_KEYWORD_LEN + 1] = { '\0' };
	size_t len = CC_KEYWORD_LEN;

	if (!VarValueGetString(_parser, _vars[0], str, &len)) {
		parseSetError(_parser, CC_CODE_FUNC_ARGS_ERROR);
		return NULL;
	}

//	for (uint i = 0; i < _parser->env_count; i++) {
//		CC_PRINT("%s: env[%d] name(%s)  type(%d)  data(%ld)", LOG_TAG, i, _parser->env[i].name,
//				_parser->env[i].type, *((long* )(_parser->env[i].data)));
//	}

	for (uint8_t i = 0; i < _parser->env_count; i++) {

//		CC_PRINT("%s: env[%d] name(%s)  type(%d)  data(%ld)", LOG_TAG, i, _parser->env[i].name,
//				_parser->env[i].type, *((long* )(_parser->env[i].data)));
//
//		CC_PRINT("compare '%d:%d' %s:%s", len, strlen(_parser->env[i].name), str,
//				_parser->env[i].name);

		if (len != strlen(_parser->env[i].name) || strncmp(str, _parser->env[i].name, len) != 0) {
//			CC_PRINT("continue");
			continue;
		}

		cc_env_s env = _parser->env[i];
		char buf[20] = { '\0' };

		var_s *var = VarCreate(str, len, env.type, _parser->depth);

		if (_parser->env[i].type == CC_TYPE_INT) {
//			CC_PRINT("INIT: %d", (env).data == NULL);
			int *i = (int*) (env.data);
//			CC_PRINT("D: %d", *i);
//			CC_PRINT("ERROR: return int '%s':'%s'!\n", env.name, str);
			if (!VarValueSetInt(_parser, var, *i)) {
				VarDestroy(var);
				return NULL;
			}
		}

		else if (env.type == CC_TYPE_BOOL) {
//			CC_PRINT("ERROR: return bool '%s'!\n", str);

			if (!VarValueSetBool(_parser, var, (bool) (*(bool*) (env).data))) {
				VarDestroy(var);
				return NULL;
			}
		}

		else if (env.type == CC_TYPE_LONG) {

//			CC_PRINT("ERROR: return long '%s'!\n", str);
			if (!VarValueSetLong(_parser, var, (long) (*(long*) (env).data))) {
				VarDestroy(var);
				return NULL;
			}
		}

		else if (env.type == CC_TYPE_FLOAT) {

//			CC_PRINT("ERROR: return float '%s'!\n", str);
			if (!VarValueSetLong(_parser, var, (float) (*(float*) (env).data))) {
				VarDestroy(var);
				return NULL;
			}
		}

		else if (env.type == CC_TYPE_CHAR) {

//			CC_PRINT("ERROR: return char '%s'!\n", str);
			if (!VarValueSetChar(_parser, var, (char) *(char*) (env).data)) {
				VarDestroy(var);
				return NULL;
			}
		}

		else if (env.type == CC_TYPE_STRING) {

//			CC_PRINT("ERROR: return string '%s'!\n", str);
			if (!VarValueSetString(_parser, var, (char*) (env).data, strlen((env).data))) {
				VarDestroy(var);
				return NULL;
			}
		}

		return var;
	}

	CC_PRINT("ERROR: bad ENV!\n");

	return NULL;
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
	char buf[CONFIG_CC_STRING_LEN] = { '\0' };
	char buf2[CONFIG_CC_STRING_LEN] = { '\0' };

	size_t len = 0;

	for (uint8_t i = 0; i < _vars_count; i++) {

		memset(buf2, '\0', sizeof(char) * CC_VALUE_STRING_LEN);

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
			len = (size_t) snprintf(buf2, CONFIG_CC_STRING_LEN, "%.*f",
			CONFIG_CC_FLOAT_EXP_LEN, *(float*) (_vars[i]->data));
		} else if (_vars[i]->type == CC_TYPE_INT) {
			len = (size_t) snprintf(buf2, CONFIG_CC_STRING_LEN, "%d", *(int*) (_vars[i]->data));
		} else if (_vars[i]->type == CC_TYPE_STRING) {
			len = strlen(_vars[i]->data);
			memcpy(buf2, _vars[i]->data, len);
		} else if (_vars[i]->type == CC_TYPE_ARRAY) {
			CC_PRINT("ERROR: not supported!\n");
			parseSetError(_parser, CC_CODE_LOGIC);
			return NULL;

		}

		if (len_total + len > CC_VALUE_STRING_LEN) {
			CC_PRINT("ERROR: string is too long. @see CC_VALUE_STRING_LEN\n");
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

	char buf[CONFIG_CC_STRING_LEN] = { '\0' };

	if (_vars[0]->type == CC_TYPE_BOOL) {
		snprintf(buf, CONFIG_CC_STRING_LEN, "%d", *(bool*) _vars[0]->data);
	}

	else if (_vars[0]->type == CC_TYPE_CHAR) {
		snprintf(buf, CONFIG_CC_STRING_LEN, "%c", *(char*) _vars[0]->data);
	}

	else if (_vars[0]->type == CC_TYPE_FLOAT) {
		snprintf(buf, CONFIG_CC_STRING_LEN, "%.*f", CONFIG_CC_FLOAT_EXP_LEN,
				*(float*) _vars[0]->data);
	}

	else if (_vars[0]->type == CC_TYPE_INT) {
		snprintf(buf, CONFIG_CC_STRING_LEN, "%d", *(int*) _vars[0]->data);
	}

	else if (_vars[0]->type == CC_TYPE_STRING) {
		snprintf(buf, CONFIG_CC_STRING_LEN, "%s", (char*) _vars[0]->data);
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

	char buf[CONFIG_CC_STRING_LEN] = { '\0' };

	if (_vars[0]->type == CC_TYPE_BOOL) {
		snprintf(buf, CONFIG_CC_STRING_LEN, "%d", *(bool*) _vars[0]->data);
	}

	else if (_vars[0]->type == CC_TYPE_CHAR) {
		snprintf(buf, CONFIG_CC_STRING_LEN, "%c", *(char*) _vars[0]->data);
	}

	else if (_vars[0]->type == CC_TYPE_FLOAT) {
		snprintf(buf, CONFIG_CC_STRING_LEN, "%.*f", CONFIG_CC_FLOAT_EXP_LEN,
				*(float*) _vars[0]->data);
	}

	else if (_vars[0]->type == CC_TYPE_INT) {
		snprintf(buf, CONFIG_CC_STRING_LEN, "%d", *(int*) _vars[0]->data);
	}

	else if (_vars[0]->type == CC_TYPE_STRING) {
		snprintf(buf, CONFIG_CC_STRING_LEN, "%s", (char*) _vars[0]->data);
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
