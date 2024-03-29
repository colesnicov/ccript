/*
 * This file is a part of "CCRipt" Embeddable Custom 'C' Like Scripting Language Interpreter
 */

/**
 * @file ccript.c
 * @brief Implementace verejnych funkci pro praci s interpreterem.
 * @since 26.06.2022
 *
 * @version 1r1
 * @date 08.04.2023
 *
 * @author Denis Colesnicov <eugustus@gmail.com>
 *
 * @copyright MIT License
 *
 */

#include "ccript/cc_block.h"
#include "ccript/cc_configs.h"
#include "ccript/cc_types.h"
#include "ccript/cc_function.h"
#include "ccript/cc_parseIf.h"
#include "ccript/cc_parser.h"
#include "ccript/cc_parseWhile.h"
#include "ccript/cc_types.h"
#include "ccript/common.h"
#include "ccript/cc_var.h"
#include "ccript/ccript.h"

#include <emblib/emblib.h>
#include <filebuffer/filebuffer.h>

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

bool cc_init(cc_parser_s *_parser)
{

	do
	{
		if (!VarInit(_parser))
		{
			break;
		}

		if (!funcInit(_parser))
		{
			break;
		}

		if (!blockInit(_parser))
		{
			break;
		}

		return true;

	} while (0);

	cc_deinit(_parser);

	return false;
}

void cc_deinit(cc_parser_s *_parser)
{
	if (NULL != _parser)
	{
		blockDeinit(_parser);
		VarDeinit(_parser);
		funcDeinit(_parser);
	}
}

var_s* cc_parse(cc_parser_s *_parser, const char *_path, cc_env_s *_env, uint8_t _env_count)
{
	_parser->error = CC_CODE_OK;
	_parser->error_pos = 0;
	var_s *ret_var = NULL;

	if (FILEBUFFER_OK != file_bufferInit(&_parser->buffer, _path, CC_BUFFER_LEN))
	{
		parseSetError(_parser, CC_CODE_IO);
		return ret_var;
	}

	_parser->env_count = _env_count;
	_parser->env = _env;

#if CC_PRINT_ENV
	for (uint8_t i = 0; i < _parser->env_count; i++)
	{
		if (CC_TYPE_FLOAT == _parser->env[i].type)
		{

			char buf[10] = {
					'\0' };
			ftoa(*((float*) (_parser->env[i].data)), buf, 3);

			CC_PRINT("env[%d] name(%s) type(%d)  data(%s)\n", i, _parser->env[i].name,
					_parser->env[i].type, buf);
		}
		else if (CC_TYPE_INT == _parser->env[i].type)
		{
			CC_PRINT("env[%d] name(%s) type(%d)  data(%d)\n", i, _parser->env[i].name,
					_parser->env[i].type, *((int* )(_parser->env[i].data)));
		}
		else if (CC_TYPE_BOOL == _parser->env[i].type)
		{
			CC_PRINT("env[%d] name(%s) type(%d)  data(%d)\n", i, _parser->env[i].name,
					_parser->env[i].type, *((int* )(_parser->env[i].data)));
		}
		else if (CC_TYPE_CHAR == _parser->env[i].type)
		{
			CC_PRINT("env[%d] name(%s) type(%d)  data(%c)\n", i, _parser->env[i].name,
					_parser->env[i].type, *((char* )(_parser->env[i].data)));
		}
		else if (CC_TYPE_STRING == _parser->env[i].type)
		{
			CC_PRINT("env[%d] name(%s) type(%d)  data(%s)\n", i, _parser->env[i].name,
					_parser->env[i].type, ((char* )(_parser->env[i].data)));
		}
		else if (CC_TYPE_LONG == _parser->env[i].type)
		{
			CC_PRINT("env[%d] name(%s) type(%d)  data(%ld)\n", i, _parser->env[i].name,
					_parser->env[i].type, *((long* )(_parser->env[i].data)));
		}
		else
		{
			CC_PRINT("env[%d] name(%s) unknown type\n", i, _parser->env[i].name);
		}
	}
#endif

	ret_var = parseBlock(_parser, 0);

	if (_parser->error != CC_CODE_RETURN)
	{
		VarDestroy(ret_var);
	}

	if (_parser->error >= CC_CODE_ERROR && _parser->error_pos >= _parser->buffer->fsize)
	{
		if (_parser->error_pos >= _parser->buffer->fsize)
		{
			// Kdyz jsme na konci souboru, neni to chyba
			_parser->error = CC_CODE_OK;
		}
	}

	VarClean(_parser);
	file_bufferClose(&(_parser->buffer));

	return ret_var;
}

cc_code_t cc_errorGetCode(cc_parser_s *_parser)
{
	return _parser->error;
}

bool cc_errorHas(cc_parser_s *_parser)
{
	return _parser->error >= CC_CODE_ERROR;
}

size_t cc_errorGetPos(cc_parser_s *_parser)
{
	return _parser->error_pos;
}

const char* cc_errorToString(cc_code_t _err_code)
{

	switch (_err_code) {

		case CC_CODE_OK:
			return "CC_CODE_OK";
			break;

		case CC_CODE_RETURN:
			return "CC_CODE_RETURN";
			break;

		case CC_CODE_CONTINUE:
			return "CC_CODE_CONTINUE";
			break;

		case CC_CODE_BREAK:
			return "CC_CODE_BREAK";
			break;

		case CC_CODE_ERROR:
			return "CC_CODE_ERROR";
			break;

		case CC_CODE_BAD_SYMBOL:
			return "CC_CODE_BAD_SYMBOL";
			break;

		case CC_CODE_KEYWORD:
			return "CC_CODE_KEYWORD";
			break;

		case CC_CODE_KEYWORD_EMPTY:
			return "CC_CODE_KEYWORD_EMPTY";
			break;

		case CC_CODE_LOGIC:
			return "CC_CODE_LOGIC";
			break;

		case CC_CODE_NO_FILE:
			return "CC_CODE_NO_FILE";
			break;

		case CC_CODE_IO:
			return "CC_CODE_IO";
			break;

		case CC_CODE_FUNC_NOT_DEFINED:
			return "CC_CODE_FUNC_NOT_DEFINED";
			break;

		case CC_CODE_FUNC_ARGS_ERROR:
			return "CC_CODE_FUNC_ARGS_ERROR";
			break;

		case CC_CODE_FUNC_EXISTS:
			return "CC_CODE_FUNC_EXISTS";
			break;

		case CC_CODE_FUNC_RET_BAD_TYPE:
			return "CC_CODE_FUNC_RET_BAD_TYPE";
			break;

		case CC_CODE_VAR_NOT_DEFINED:
			return "CC_CODE_VAR_NOT_DEFINED";
			break;

		case CC_CODE_VAR_NOT_ASSIGNED:
			return "CC_CODE_VAR_NOT_ASSIGNED";
			break;

		case CC_CODE_VAR_BAD_TYPE:
			return "CC_CODE_VAR_BAD_TYPE";
			break;

		case CC_CODE_ARGS_BAD_TYPE:
			return "CC_CODE_ARGS_BAD_TYPE";
			break;

		case CC_CODE_VAR_EXISTS:
			return "CC_CODE_VAR_EXISTS";
			break;

		case CC_CODE_NOT_MEM:
			return "CC_CODE_NOT_MEM";
			break;

		case CC_CODE_NOT_COMMENT:
			return "CC_CODE_NOT_COMMENT";
			break;

		case CC_CODE_NOT_BOOL:
			return "CC_CODE_NOT_BOOL";
			break;

		case CC_CODE_NOT_INTEGER:
			return "CC_CODE_NOT_INTEGER";
			break;

		case CC_CODE_NOT_FLOAT:
			return "CC_CODE_NOT_FLOAT";
			break;

		case CC_CODE_NOT_CHAR:
			return "CC_CODE_NOT_CHAR";
			break;

		case CC_CODE_NOT_STRING:
			return "CC_CODE_NOT_STRING";
			break;

		case CC_CODE_NOT_ARRAY:
			return "CC_CODE_NOT_ARRAY";
			break;

		case CC_CODE_OUT_OF_LOOP:
			return "CC_CODE_OUT_OF_LOOP";
			break;

		case CC_CODE_ABORT:
			return "CC_CODE_ABORT";
			break;

		case CC_CODE_TYPE_UNKNOWN:
			return "CC_CODE_TYPE_UNKNOWN";
			break;

		case CC_CODE_VALUE_EMPTY:
			return "CC_CODE_VALUE_EMPTY";
			break;

		case CC_CODE_STRING_TOO_LONG:
			return "CC_CODE_STRING_TOO_LONG";
			break;

		case CC_CODE_ZERO_DIVIDED:
			return "CC_CODE_ZERO_DIVIDED";
			break;

		case CC_CODE_NOT_IMPLEMENTED:
			return "CC_CODE_NOT_IMPLEMENTED";
			break;

		default:
			return "unknown";
	}
}

void cc_abort(cc_parser_s *_parser)
{
	_parser->error = CC_CODE_ABORT;
}

void cc_varDestroy(var_s *_var)
{
	VarDestroy(_var);
}

cc_code_t cc_varGetBool(var_s *_var, bool *_val)
{
	if (_var->type != CC_TYPE_BOOL)
	{
		return CC_CODE_VAR_BAD_TYPE;
	}

	if (_var->valid == false)
	{
		return CC_CODE_VAR_NOT_ASSIGNED;
	}

	*_val = *((bool*) (_var->data));

	return CC_CODE_OK;
}

cc_code_t cc_varGetInt(var_s *_var, int *_val)
{
	if (_var->type != CC_TYPE_INT)
	{
		return CC_CODE_VAR_BAD_TYPE;
	}

	if (_var->valid == false)
	{
		return CC_CODE_VAR_NOT_ASSIGNED;
	}

	*_val = *((int*) (_var->data));

	return CC_CODE_OK;

}

cc_code_t cc_varGetLong(var_s *_var, long *_val)
{
	if (_var->type != CC_TYPE_LONG)
	{
		return CC_CODE_VAR_BAD_TYPE;
	}

	if (_var->valid == false)
	{
		return CC_CODE_VAR_NOT_ASSIGNED;
	}

	*_val = *((long*) (_var->data));

	return CC_CODE_OK;

}

cc_code_t cc_varGetFloat(var_s *_var, float *_val)
{
	if (_var->type != CC_TYPE_FLOAT)
	{
		return CC_CODE_VAR_BAD_TYPE;
	}

	if (_var->valid == false)
	{
		return CC_CODE_VAR_NOT_ASSIGNED;
	}

	*_val = *((float*) (_var->data));

	return CC_CODE_OK;

}

cc_code_t cc_varGetChar(var_s *_var, char *_val)
{
	if (_var->type != CC_TYPE_CHAR)
	{
		return CC_CODE_VAR_BAD_TYPE;
	}

	if (_var->valid == false)
	{
		return CC_CODE_VAR_NOT_ASSIGNED;
	}

	memcpy(_val, _var->data, sizeof(char));

	return CC_CODE_OK;

}

cc_code_t cc_varGetString(var_s *_var, char *_val, size_t *_len)
{
	if (_var->type != CC_TYPE_STRING)
	{
		return CC_CODE_VAR_BAD_TYPE;
	}

	if (_var->valid == false)
	{
		return CC_CODE_VAR_NOT_ASSIGNED;
	}

	*_len = strlen((char*) _var->data);

	if (*_len > CC_VALUE_STRING_LEN)
	{
		return CC_CODE_STRING_TOO_LONG;
	}

	memcpy(_val, _var->data, *_len * sizeof(char));

	return CC_CODE_OK;

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

