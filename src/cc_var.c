/*
 * This file is a part of "CCRipt" Embeddable Custom 'C' Like Scripting Language Interpreter
 */

/**
 * @file cc_var.c
 * @brief Implementace funkci pro praci s promennymi.
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

#include "ccript/cc_configs.h"
#include "ccript/cc_parser.h"
#include "ccript/cc_types.h"
#include "ccript/cc_var.h"
#include "ccript/cc_var_ext.h"
#include "cvector/cvector.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "emblib/emblib.h"

bool VarInit(cc_parser_s *_parser)
{
	_parser->vars = (cvector_s*) CC_MALLOC(sizeof(cvector_s));
	if (_parser->vars == NULL)
	{
		parseSetError(_parser, CC_CODE_NOT_MEM);
		CC_VAR_DEBUG("ERROR: not enough memmory for variables.\n");
		return false;
	}
	memset(_parser->vars, 0, sizeof(cvector_s));

	if (!cvector_init(_parser->vars))
	{
		parseSetError(_parser, CC_CODE_NOT_MEM);
		CC_VAR_DEBUG("ERROR: not enough memmory for variables container.\n");
	}
	return true;
}

bool VarExists(cc_parser_s *_parser, const char *_name, size_t _name_len)
{
	for (int i = 0; i < cvector_total(_parser->vars); i++)
	{

		var_s *var = (var_s*) cvector_get(_parser->vars, i);
		CC_ASSERT(var != NULL);

		if (strlen(var->name) == _name_len && strncmp(var->name, _name, _name_len) == 0)
		{

			return true;
		}
	}

	return false;
}

bool VarValueSetBool(cc_parser_s *_parser, var_s *_var, bool _bool)
{

	if (_var->type != CC_TYPE_BOOL)
	{
		parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
		return false;
	}

	if (_var->data == NULL)
	{
		_var->data = CC_MALLOC(sizeof(bool));

		if (_var->data == NULL)
		{

			parseSetError(_parser, CC_CODE_NOT_MEM);

			return false;
		}
	}

	memcpy(_var->data, &_bool, 1 * sizeof(bool));

	_var->valid = true;

	return true;

}

bool VarValueSetFloat(cc_parser_s *_parser, var_s *_var, float _float)
{

	if (_var->type != CC_TYPE_FLOAT)
	{
		parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
		return false;
	}

	if (_var->data == NULL)
	{
		_var->data = CC_MALLOC(sizeof(float));

		if (_var->data == NULL)
		{
			parseSetError(_parser, CC_CODE_NOT_MEM);

			return false;
		}
	}

	memcpy(_var->data, &_float, 1 * sizeof(float));

	_var->valid = true;

	return true;

}

bool VarValueSetInt(cc_parser_s *_parser, var_s *_var, int _int)
{

	if (_var->type != CC_TYPE_INT)
	{
		parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
		return false;
	}

	if (_var->data == NULL)
	{
		_var->data = CC_MALLOC(sizeof(int));

		if (_var->data == NULL)
		{
			parseSetError(_parser, CC_CODE_NOT_MEM);

			return false;
		}
	}

	memcpy(_var->data, &_int, 1 * sizeof(int));

	_var->valid = true;

	return true;

}

bool VarValueSetLong(cc_parser_s *_parser, var_s *_var, long _long)
{

	if (_var->type != CC_TYPE_LONG)
	{
		parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
		return false;
	}

	if (_var->data == NULL)
	{
		_var->data = CC_MALLOC(sizeof(long));

		if (_var->data == NULL)
		{
			parseSetError(_parser, CC_CODE_NOT_MEM);

			return false;
		}
	}

	memcpy(_var->data, &_long, 1 * sizeof(long));

	_var->valid = true;

	return true;

}

bool VarValueSetChar(cc_parser_s *_parser, var_s *_var, char _char)
{

	if (_var->type != CC_TYPE_CHAR)
	{
		parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
		return false;
	}

	if (_var->data == NULL)
	{
		_var->data = CC_MALLOC(sizeof(char));

		if (_var->data == NULL)
		{
			parseSetError(_parser, CC_CODE_NOT_MEM);

			return false;
		}

	}

	memcpy(_var->data, &_char, 1 * sizeof(char));

	_var->valid = true;

	return true;

}

bool VarValueSetString(cc_parser_s *_parser, var_s *_var, char *_str, size_t _str_len)
{

	if (_var->type != CC_TYPE_STRING)
	{
		parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
		return false;
	}

	if (_var->data != NULL)
	{
		CC_FREE(_var->data);
		_var->data = NULL;
		_var->valid = false;
	}
// fixme volat realloc?

	_var->data = CC_MALLOC((_str_len + 1) * sizeof(char));
	if (_var->data == NULL)
	{
		parseSetError(_parser, CC_CODE_NOT_MEM);

		return false;
	}

	memset(_var->data, 0, (_str_len + 1) * sizeof(char));
	memcpy(_var->data, _str, _str_len * sizeof(char));

	_var->valid = true;

	return true;

}

bool VarStore(cc_parser_s *_parser, var_s *_var, bool _overwrite)
{

	if (NULL == _var)
	{
		return false;
	}

	if (VarExists(_parser, _var->name, strlen(_var->name)))
	{
		if (_overwrite)
		{
			VarFindAndDestroy(_parser, _var->name, strlen(_var->name));
		}
		else
		{
			parseSetError(_parser, CC_CODE_VAR_EXISTS);
			CC_VAR_DEBUG("ERROR: variable '%s' exists!\n", _var->name);

			return false;
		}
	}

	return cvector_add(_parser->vars, _var);

}

var_s* VarCreate(const char *_name, uint8_t _name_len, cc_type_t _type, uint8_t _scope)
{

	if (_name_len == 0 || _name == NULL)
	{
//		parseSetError(_parser, CC_CODE_LOGIC);
		CC_VAR_DEBUG("ERROR: variable name is 'null'\n");
		return NULL;
	}

	if (_type >= CC_TYPE_MAX)
	{
		CC_VAR_DEBUG("ERROR: variable '%s' type unknown!\n", _name);
//		parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
		return NULL;
	}

	var_s *var_t = (var_s*) CC_MALLOC(sizeof(var_s));
	if (var_t == NULL)
	{
//		parseSetError(_parser, CC_CODE_NOT_MEM);
		CC_VAR_DEBUG("ERROR: 2 not enought memmory for variable '%s'.\n", _name);
		return NULL;
	}

	var_t->name = (char*) CC_MALLOC((_name_len + 1) * sizeof(char));

	if (var_t->name == NULL)
	{
//		parseSetError(_parser, CC_CODE_NOT_MEM);
		CC_VAR_DEBUG("ERROR: 3 not enought memmory for variable '%s:%d'.\n", _name, _name_len);
		CC_FREE(var_t);

		return NULL;
	}

	memset(var_t->name, 0, sizeof(char) * (_name_len + 1));
	memcpy(var_t->name, _name, sizeof(char) * _name_len);

	var_t->data = NULL;
	var_t->type = _type;
	var_t->scope = _scope;
	var_t->valid = false;

	return var_t;
}

var_s* VarCopy(cc_parser_s *_parser, var_s *_var)
{

	if (_var->type >= CC_TYPE_MAX)
	{
		return NULL;
	}

	var_s *var_t = (var_s*) CC_MALLOC(sizeof(var_s));
	if (var_t == NULL)
	{
		return NULL;
	}

	var_t->name = (char*) CC_MALLOC((strlen(_var->name) + 1) * sizeof(char));

	if (var_t->name == NULL)
	{
		CC_FREE(var_t);

		return NULL;
	}

	memset(var_t->name, 0, sizeof(char) * (strlen(_var->name) + 1));
	memcpy(var_t->name, _var->name, sizeof(char) * strlen(_var->name));

	var_t->data = NULL;
	var_t->type = _var->type;
	var_t->scope = _var->scope;
	var_t->valid = false;

	if (_var->type == CC_TYPE_FLOAT)
	{
		float v = 0.0f;
		if (!VarValueGetFloat(_parser, _var, &v))
		{
			VarDestroy(var_t);
			return NULL;
		}

		if (!VarValueSetFloat(_parser, var_t, v))
		{
			VarDestroy(var_t);
			return NULL;
		}
	}

	else if (_var->type == CC_TYPE_BOOL)
	{
		bool v;
		if (!VarValueGetBool(_parser, _var, &v))
		{
			VarDestroy(var_t);
			return NULL;
		}

		if (!VarValueSetBool(_parser, var_t, v))
		{
			VarDestroy(var_t);
			return NULL;
		}
	}

	else if (_var->type == CC_TYPE_STRING)
	{
		char v[CC_VALUE_STRING_LEN + 1] = {
				'\0' };
		size_t vl = 0;
		if (!VarValueGetString(_parser, _var, v, &vl))
		{
			VarDestroy(var_t);
			return NULL;
		}

		if (!VarValueSetString(_parser, _var, v, vl))
		{
			VarDestroy(var_t);
			return NULL;
		}
	}

	else if (_var->type == CC_TYPE_CHAR)
	{
		char v = 0;
		if (!VarValueGetChar(_parser, var_t, &v))
		{
			VarDestroy(var_t);
			return NULL;
		}

		if (!VarValueSetChar(_parser, _var, v))
		{
			VarDestroy(var_t);
			return NULL;
		}
	}

	else if (_var->type == CC_TYPE_INT)
	{
		int v;
		if (!VarValueGetInt(_parser, _var, &v))
		{
			VarDestroy(var_t);
			return NULL;
		}

		if (!VarValueSetInt(_parser, var_t, v))
		{
			VarDestroy(var_t);
			return NULL;
		}

	}

	else if (_var->type == CC_TYPE_LONG)
	{
		long v;
		if (!VarValueGetLong(_parser, _var, (long*) &v))
		{
			VarDestroy(var_t);
			return false;
		}

		if (!VarValueSetLong(_parser, var_t, v))
		{
			VarDestroy(var_t);
			return NULL;
		}

	}

	else
	{
		VarDestroy(var_t);
		parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
		return NULL;

	}

	return var_t;
}

typedef struct {
	uint8_t limit;
	uint8_t count;
	void **data;
} var_array_s;

var_s* VarCreateArray(const char *_name, size_t _name_len, uint8_t _size, cc_type_t _type)
{

	if (_name_len == 0 || _name == NULL)
	{
//		parseSetError(_parser, CC_CODE_LOGIC);
		CC_VAR_DEBUG("ERROR: variable name is 'null'\n");
		return NULL;
	}

	if (_type >= CC_TYPE_MAX)
	{
		CC_VAR_DEBUG("ERROR: variable '%d' type unknown!\n", _type);
//		parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
		return NULL;
	}

	var_s *var_t = (var_s*) CC_MALLOC(sizeof(var_s));
	if (var_t == NULL)
	{
//		parseSetError(_parser, CC_CODE_NOT_MEM);
		CC_VAR_DEBUG("ERROR: 4 not enought memmory for variable '%s'.\n", _name);
		return NULL;
	}

	var_t->name = (char*) CC_MALLOC((_name_len + 1) * sizeof(char));

	if (var_t->name == NULL)
	{
//		parseSetError(_parser, CC_CODE_NOT_MEM);
		CC_VAR_DEBUG("ERROR: 5 not enought memmory for variable '%s'.\n", _name);
		CC_FREE(var_t);
		var_t = NULL;
		return NULL;
	}

	memset(var_t->name, 0, (_name_len + 1) * sizeof(char));
	memcpy(var_t->name, _name, _name_len);

	CC_VAR_DEBUG("array[%s]\n", VarTypeToString(_type));
	if (_type == CC_TYPE_INT)
	{

		CC_VAR_DEBUG("array\n");
		var_array_s *holder = CC_MALLOC(sizeof(*holder));
		if (holder == NULL)
		{
//			parseSetError(_parser, CC_CODE_NOT_MEM);
			CC_VAR_DEBUG("neni pamet");
			CC_FREE(var_t->name);
			CC_FREE(var_t);
			var_t = NULL;

		}

		holder->limit = _size;
		holder->count = 0;
		holder->data = NULL;
		var_t->data = holder;
	}

	var_t->type = _type;
	var_t->valid = false;

	return var_t;
}

void VarDestroy(var_s *_var)
{

	if (_var != NULL)
	{

		if (_var->name != NULL)
		{

			CC_VAR_DEBUG("DEBUG: destroy variable '%s'.\n", _var->name);

			CC_FREE(_var->name);
			_var->name = NULL;
		}

		if (_var->data != NULL)
		{

			if (_var->type == CC_TYPE_ARRAY)
			{
				var_array_s *arr = ((var_array_s*) (_var->data));
				if (arr->data != NULL)
				{
					for (uint8_t i = 0; i < arr->count; i++)
					{
						CC_FREE(arr->data[i]);
						arr->data[i] = NULL;
					}

					CC_FREE(arr->data);
					arr->data = NULL;
				}

				CC_FREE(_var->data);

			}
			else
			{
				CC_FREE(_var->data);
				_var->data = NULL;
			}
		}

		CC_FREE(_var);
		_var = NULL;
	}

}

bool VarFindAndDestroy(cc_parser_s *_parser, const char *_name, size_t _len)
{

	size_t len;
	int i;

	for (i = 0; i < cvector_total(_parser->vars); i++)
	{
		var_s *var = (var_s*) cvector_get(_parser->vars, i);
		len = strlen(var->name);

		if (_len == len && strncmp(_name, var->name, _len) == 0)
		{

			VarDestroy(var);

			return cvector_delete(_parser->vars, i);
		}
	}

	parseSetError(_parser, CC_CODE_VAR_NOT_DEFINED);

	CC_VAR_DEBUG("ERROR: undefined variable '%s'!\n", _name);

	return false;
}

void VarDeinit(cc_parser_s *_parser)
{
	int i;

	if (_parser->vars == NULL)
	{
		return;
	}

	var_s *var = NULL;

	CC_VAR_DEBUG("DEBUG: destroy vars: '%d'.\n", cvector_total(_parser->vars));
	for (i = 0; i < cvector_total(_parser->vars); i++)
	{
		var = cvector_get(_parser->vars, i);
		VarDestroy(var);
	}

	cvector_deinit(_parser->vars);
	CC_FREE(_parser->vars);
	_parser->vars = NULL;

}

void VarClean(cc_parser_s *_parser)
{
	assert(_parser != NULL && "PARSER NOT INITIALIZED!");

	if (_parser->vars == NULL)
	{
		return;
	}

	int i = cvector_total(_parser->vars);
	while (i)
	{
		i--;
		cvector_delete_dealloc(_parser->vars, i, freeVar);

	}

}

var_s* VarGet(cc_parser_s *_parser, const char *_name, size_t _len)
{

	var_s *var = NULL;

	for (int i = 0; i < cvector_total(_parser->vars); i++)
	{
		var = cvector_get(_parser->vars, i);

		if (_len == strlen(var->name) && strncmp(var->name, _name, _len) == 0)
		{
			if (var->scope > _parser->depth)
			{
				parseSetError(_parser, CC_CODE_LOGIC);
				CC_VAR_DEBUG("error: variable '%s' out of scope: '%d>%d'", var->name, var->scope,
						_parser->depth);

				break;
			}

			return var;
		}

	}

	parseSetError(_parser, CC_CODE_VAR_NOT_DEFINED);
	CC_VAR_DEBUG("ERROR: undefined variable '%s'.\n", _name);
	return NULL;
}

size_t VarGetSize(var_s *_var)
{

	CC_ASSERT(_var != NULL && "var is NULL");

	size_t siz = 0;

	siz += sizeof(char) * strlen(_var->name); // name
	siz += sizeof(cc_type_t); // type
	siz += sizeof(uint8_t); // scope
	siz += sizeof(bool); // valid

	if (_var->valid)
	{
		if (_var->type == CC_TYPE_INT)
		{
			siz += sizeof(int); // data
		}
		else if (_var->type == CC_TYPE_FLOAT)
		{
			siz += sizeof(float); // data
		}
		else if (_var->type == CC_TYPE_BOOL)
		{
			siz += sizeof(bool); // data
		}
		else if (_var->type == CC_TYPE_CHAR)
		{
			siz += sizeof(char); // data
		}
		else if (_var->type == CC_TYPE_STRING)
		{
			siz += sizeof(char) * (strlen(_var->data) + 1); // data
		}
		else if (_var->type == CC_TYPE_ARRAY)
		{
			CC_VAR_DEBUG("\tdata='[]'\n");
		}
		else
		{
			CC_VAR_DEBUG("\tdata\n");
		}
	}

	return siz;
}

size_t VarGetSizeAll(cc_parser_s *_parser)
{
	size_t siz = 0;

	for (int i = 0; i < cvector_total(_parser->vars); i++)
	{
		var_s *var = cvector_get(_parser->vars, i);
		if (!var)
		{
			CC_VAR_DEBUG("ERROR: var '#%d' is 'null'\n", i);
			parseSetError(_parser, CC_CODE_VAR_NOT_DEFINED);
			return 0;
		}
		siz += VarGetSize(var);
	}

	return siz;
}

void VarDump(var_s *_var)
{
	CC_PRINT("  ------ DUMP VAR\n");
	if (_var == NULL)
	{

		CC_PRINT("  ------ VAR IS 'NULL'\n");
		CC_PRINT("  ------ END DUMP VAR\n");
		return;
	}

	CC_PRINT("\tname='%s';\n", _var->name);
	CC_PRINT("\ttype='%i:%s';\n", _var->type, VarTypeToString(_var->type));
	CC_PRINT("\tvalid='%i';\n", _var->valid);
	CC_PRINT("\tscope='%d'\n", _var->scope);
	if (_var->valid)
	{
		if (_var->type == CC_TYPE_INT)
		{
			int *data = (int*) (_var->data);
			CC_PRINT("\tdata='%d'\n", *data);
		}

		else if (_var->type == CC_TYPE_LONG)
		{
			long *data = (long*) (_var->data);

			char buf[CC_VALUE_LONG_LEN] = {
					'\0' };
			ltoa((long) *data, buf, 10);
			CC_PRINT("\tdata='%s'\n", buf);
		}

		else if (_var->type == CC_TYPE_FLOAT)
		{
			float *data = (float*) (_var->data);
			char buf[16] = {
					'\0' };
			ftoa(*data, buf, CC_FLOAT_EXP_LEN);
			CC_PRINT("\tdata='%s'\n", buf);
		}

		else if (_var->type == CC_TYPE_BOOL)
		{
			bool *data = (bool*) (_var->data);
			CC_PRINT("\tdata='%d'\n", (int ) (*(data)));
		}

		else if (_var->type == CC_TYPE_CHAR)
		{
			char *data = (char*) (_var->data);
			CC_PRINT("\tdata='%c'\n", data[0]);
		}

		else if (_var->type == CC_TYPE_STRING)
		{
			char *data = (char*) (_var->data);
			CC_PRINT("\tdata='%s'\n", data);
		}

		else if (_var->type == CC_TYPE_ARRAY)
		{
			CC_PRINT("\tdata='[]'\n");
		}

		else
		{
			CC_PRINT("\tdata\n");
		}
	}

	CC_PRINT("  ------ END DUMP VAR\n");
}

void VarDumpAll(cc_parser_s *_parser)
{
	CC_PRINT("DUMP ALL VARS(%d)\n", cvector_total(_parser->vars));

	for (int i = 0; i < cvector_total(_parser->vars); i++)
	{
		var_s *var = cvector_get(_parser->vars, i);
		CC_PRINT("iteration='%d';\n", i);
		VarDump(var);
	}

	CC_PRINT("END DUMP ALL VARS\n");
}

bool VarValueGetBool(cc_parser_s *_parser, var_s *_var, bool *_bool)
{
	if (_var->type != CC_TYPE_BOOL)
	{
		parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
		return false;
	}

	if (_var->valid == false)
	{
		parseSetError(_parser, CC_CODE_VAR_NOT_ASSIGNED);
		return false;
	}

	*_bool = *((bool*) (_var->data));

	return true;
}

bool VarValueGetFloat(cc_parser_s *_parser, var_s *_var, float *_float)
{
	if (_var->type != CC_TYPE_FLOAT)
	{
		parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
		return false;
	}

	if (_var->valid == false)
	{
		parseSetError(_parser, CC_CODE_VAR_NOT_ASSIGNED);
		return false;
	}

	*_float = *((float*) (_var->data));

	return true;
}

bool VarValueGetChar(cc_parser_s *_parser, var_s *_var, char *_char)
{
	if (_var->type != CC_TYPE_CHAR)
	{
		parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
		return false;
	}

	if (_var->valid == false)
	{
		parseSetError(_parser, CC_CODE_VAR_NOT_ASSIGNED);
		return false;
	}

	memcpy(_char, _var->data, sizeof(char));

	return true;
}

bool VarValueGetInt(cc_parser_s *_parser, var_s *_var, int *_int)
{
	if (_var->type != CC_TYPE_INT)
	{
		parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
		return false;
	}

	if (_var->valid == false)
	{
		parseSetError(_parser, CC_CODE_VAR_NOT_ASSIGNED);
		return false;
	}

	*_int = *((int*) _var->data);
	return true;
}

bool VarValueGetLong(cc_parser_s *_parser, var_s *_var, long *_int)
{
	if (_var->type != CC_TYPE_LONG)
	{
		parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
		return false;
	}

	if (_var->valid == false)
	{
		parseSetError(_parser, CC_CODE_VAR_NOT_ASSIGNED);
		return false;
	}

	*_int = *((long*) _var->data);
	return true;
}

bool VarValueGetString(cc_parser_s *_parser, var_s *_var, char *_str, size_t *_len)
{
	if (_var->type != CC_TYPE_STRING)
	{
		parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
		return false;
	}

	if (_var->valid == false)
	{
		parseSetError(_parser, CC_CODE_VAR_NOT_ASSIGNED);
		return false;
	}
	size_t len = strlen(_var->data);

	if (len > CC_VALUE_STRING_LEN)
	{
		CC_VAR_DEBUG("ERROR: string is too long '%u>%u'\n", len, *_len);
		*_len = len;
		parseSetError(_parser, CC_CODE_STRING_TOO_LONG);
		return false;
	}

	*_len = len;
	memcpy(_str, _var->data, len * sizeof(char));

	return true;
}

var_s* VarCastToString(cc_parser_s *_parser, var_s *_var_from)
{

	char val[CC_VALUE_STRING_LEN + 1] = {
			'\0' };
	size_t len = 0;

	if (_var_from->type == CC_TYPE_FLOAT)
	{
		float v = 0.0f;
		if (!VarValueGetFloat(_parser, _var_from, &v))
		{
			return false;
		}
		len = (size_t) strlen(ftoa(v, val, CC_FLOAT_EXP_LEN));
	}

	else if (_var_from->type == CC_TYPE_BOOL)
	{
		bool v = false;
		if (!VarValueGetBool(_parser, _var_from, &v))
		{
			return false;
		}
		len = (size_t) sprintf(val, "%d", v);
	}

	else if (_var_from->type == CC_TYPE_INT)
	{
		int v = 0;
		if (!VarValueGetInt(_parser, _var_from, &v))
		{
			return false;
		}
		len = (size_t) sprintf(val, "%d", v);
	}

	else if (_var_from->type == CC_TYPE_LONG)
	{
		long v = 0;
		if (!VarValueGetLong(_parser, _var_from, &v))
		{
			return false;
		}
		ltoa(v, val, 10);
		len = strlen(val);
	}

	else if (_var_from->type == CC_TYPE_INT)
	{
		int v = 0;
		if (!VarValueGetInt(_parser, _var_from, &v))
		{
			return false;
		}
		len = (size_t) sprintf(val, "%d", v);
	}

	else if (_var_from->type == CC_TYPE_CHAR)
	{
		char v = 0;
		if (!VarValueGetChar(_parser, _var_from, &v))
		{
			return false;
		}
		len = 1;
		val[0] = v;
	}

	else
	{
		parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
		return false;

	}

	var_s *var = VarCreate("_", 1, CC_TYPE_STRING, _var_from->scope);
	if (!var)
	{
		return NULL;
	}

	if (!VarValueSetString(_parser, var, val, len))
	{
		VarDestroy(var);
		return NULL;
	}

	return var;
}

var_s* VarCastToBool(cc_parser_s *_parser, var_s *_var_from)
{
	bool val = 0;

	if (_var_from->type == CC_TYPE_FLOAT)
	{
		float v = 0.0f;
		if (!VarValueGetFloat(_parser, _var_from, &v))
		{
			return NULL;
		}
		val = (bool) v;
	}

	else if (_var_from->type == CC_TYPE_BOOL)
	{

		if (!VarValueGetBool(_parser, _var_from, &val))
		{
			return NULL;
		}
	}

	else if (_var_from->type == CC_TYPE_STRING)
	{
		char v[CC_VALUE_STRING_LEN + 1] = {
				'\0' };
		size_t vl = 0;
		if (!VarValueGetString(_parser, _var_from, v, &vl))
		{
			return NULL;
		}
		val = (bool) atoi(v);
	}

	else if (_var_from->type == CC_TYPE_CHAR)
	{
		char v = 0;
		if (!VarValueGetChar(_parser, _var_from, &v))
		{
			return NULL;
		}
		val = (bool) v;
	}

	else if (_var_from->type == CC_TYPE_INT)
	{
		if (!VarValueGetInt(_parser, _var_from, (int*) &val))
		{
			return NULL;
		}

	}

	else if (_var_from->type == CC_TYPE_LONG)
	{

		if (!VarValueGetLong(_parser, _var_from, (long*) &val))
		{
			return false;
		}

	}

	{
		parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
		return NULL;

	}

	var_s *var = VarCreate("_", 1, CC_TYPE_BOOL, _var_from->scope);
	if (!var)
	{
		return NULL;
	}

	if (!VarValueSetBool(_parser, var, val))
	{
		VarDestroy(var);
		return NULL;
	}

	return var;
}

var_s* VarCastToChar(cc_parser_s *_parser, var_s *_var_from)
{
	char val = 0;

	if (_var_from->type == CC_TYPE_FLOAT)
	{
		float v = 0.0f;
		if (!VarValueGetFloat(_parser, _var_from, &v))
		{
			return NULL;
		}
		val = (char) v;
	}

	else if (_var_from->type == CC_TYPE_BOOL)
	{
		CC_VAR_DEBUG("ERROR: can't convert type 'bool' to 'char'!\n");
		return NULL;
	}

	else if (_var_from->type == CC_TYPE_STRING)
	{
		CC_VAR_DEBUG("ERROR: can't convert type 'string' to 'char'!\n");
		return NULL;
	}

	else if (_var_from->type == CC_TYPE_CHAR)
	{

		if (!VarValueGetChar(_parser, _var_from, &val))
		{
			return NULL;
		}
	}

	else if (_var_from->type == CC_TYPE_INT)
	{
		int v = 0;
		if (!VarValueGetInt(_parser, _var_from, (int*) &v))
		{
			return NULL;
		}
		val = (char) v;

	}
	else
	{
		parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
		return NULL;

	}

	var_s *var = VarCreate("_", 1, CC_TYPE_CHAR, _var_from->scope);
	if (!var)
	{
		return NULL;
	}

	if (!VarValueSetChar(_parser, var, val))
	{
		VarDestroy(var);
		return NULL;
	}

	return var;
}

var_s* VarCastToInt(cc_parser_s *_parser, var_s *_var_from)
{
	int val = 0;

	if (_var_from->type == CC_TYPE_FLOAT)
	{
		float v = 0.0f;
		if (!VarValueGetFloat(_parser, _var_from, &v))
		{
			return false;
		}
		val = (int) v;
	}

	else if (_var_from->type == CC_TYPE_BOOL)
	{
		bool v = false;
		if (!VarValueGetBool(_parser, _var_from, &v))
		{
			return false;
		}
		val = v;
	}

	else if (_var_from->type == CC_TYPE_STRING)
	{
		char v[51] = {
				'\0' };
		size_t vl = 0;
		if (!VarValueGetString(_parser, _var_from, v, &vl))
		{
			return false;
		}
		val = atoi(v);
	}

	else if (_var_from->type == CC_TYPE_CHAR)
	{
		char v = 0;
		if (!VarValueGetChar(_parser, _var_from, &v))
		{
			return false;
		}
		val = (uint8_t) v;
	}

	else if (_var_from->type == CC_TYPE_INT)
	{
		if (!VarValueGetInt(_parser, _var_from, &val))
		{
			return false;
		}

	}

	else if (_var_from->type == CC_TYPE_LONG)
	{
		long v = 0;
		if (!VarValueGetLong(_parser, _var_from, &v))
		{
			return false;
		}
		val = (int) v;
	}

	else
	{
		parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
		return false;

	}

	var_s *var = VarCreate("_", 1, CC_TYPE_INT, _var_from->scope);
	if (!var)
	{
		return NULL;
	}

	if (!VarValueSetInt(_parser, var, val))
	{
		VarDestroy(var);
		return NULL;
	}

	return var;
}

var_s* VarCastToLong(cc_parser_s *_parser, var_s *_var_from)
{
	long val = 0;

	if (_var_from->type == CC_TYPE_FLOAT)
	{
		float v = 0.0f;
		if (!VarValueGetFloat(_parser, _var_from, &v))
		{
			return false;
		}
		val = (long) v;
	}

	else if (_var_from->type == CC_TYPE_BOOL)
	{
		bool v = false;
		if (!VarValueGetBool(_parser, _var_from, &v))
		{
			return false;
		}
		val = (long) v;
	}

	else if (_var_from->type == CC_TYPE_STRING)
	{
		char v[51] = {
				'\0' };
		size_t vl = 0;
		if (!VarValueGetString(_parser, _var_from, v, &vl))
		{
			return false;
		}
		val = atol(v);
	}

	else if (_var_from->type == CC_TYPE_CHAR)
	{
		char v = 0;
		if (!VarValueGetChar(_parser, _var_from, &v))
		{
			return false;
		}
		val = (long) v;
	}

	else if (_var_from->type == CC_TYPE_INT)
	{
		if (!VarValueGetInt(_parser, _var_from, (int*) &val))
		{
			return false;
		}

	}

	else if (_var_from->type == CC_TYPE_LONG)
	{
		if (!VarValueGetLong(_parser, _var_from, &val))
		{
			return false;
		}

	}

	else
	{
		parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
		return false;

	}

	var_s *var = VarCreate("_", 1, CC_TYPE_LONG, _var_from->scope);
	if (!var)
	{
		return NULL;
	}

	if (!VarValueSetLong(_parser, var, val))
	{
		VarDestroy(var);
		return NULL;
	}

	return var;
}

var_s* VarCastToFloat(cc_parser_s *_parser, var_s *_var_from)
{
	float val = 0;

	if (_var_from->type == CC_TYPE_FLOAT)
	{
		if (!VarValueGetFloat(_parser, _var_from, &val))
		{
			return NULL;
		}
	}
	else if (_var_from->type == CC_TYPE_BOOL)
	{
		bool v = false;
		if (!VarValueGetBool(_parser, _var_from, &v))
		{
			return NULL;
		}
		val = (float) v;
	}
	else if (_var_from->type == CC_TYPE_STRING)
	{
		char v[51] = {
				'\0' };
		size_t vl = 0;
		if (!VarValueGetString(_parser, _var_from, v, &vl))
		{
			return NULL;
		}
		val = (float) atof(v);
	}
	else if (_var_from->type == CC_TYPE_CHAR)
	{
		char v = 0;
		if (!VarValueGetChar(_parser, _var_from, &v))
		{
			return NULL;
		}
		val = (float) v;
	}

	else if (_var_from->type == CC_TYPE_INT)
	{
		int v = 0;
		if (!VarValueGetInt(_parser, _var_from, &v))
		{
			return NULL;
		}
		val = (float) v;
	}

	else if (_var_from->type == CC_TYPE_LONG)
	{
		long v = 0;
		if (!VarValueGetLong(_parser, _var_from, &v))
		{
			return false;
		}
		val = (float) v;
	}

	else
	{
		parseSetError(_parser, CC_CODE_VAR_BAD_TYPE);
		return false;

	}

	var_s *var = VarCreate("_", 1, CC_TYPE_FLOAT, _var_from->scope);

	if (!var)
	{
		return NULL;
	}

	if (!VarValueSetFloat(_parser, var, val))
	{
		VarDestroy(var);
		return NULL;
	}

	return var;
}

const char* VarTypeToString(cc_type_t _type)
{

	switch (_type) {
		case CC_TYPE_BOOL:
			return (const char*) "BOOL";
		case CC_TYPE_INT:
			return (const char*) "INT";
		case CC_TYPE_LONG:
			return (const char*) "LONG";
		case CC_TYPE_FLOAT:
			return (const char*) "FLOAT";
		case CC_TYPE_CHAR:
			return (const char*) "CHAR";
		case CC_TYPE_STRING:
			return (const char*) "STRING";
		case CC_TYPE_ARRAY:
			return (const char*) "ARRAY";
		default:
			return (const char*) "__UNKNOWN__";
	}
}

void VarGarbageScope(cc_parser_s *_parser)
{
	var_s *var = NULL;
	uint8_t i = 0;

	// tady WHILE proto ze kontejner se nevaliduje?
	while (true)
	{

		if (((cvector_s*) _parser->vars)->total == 0)
		{
			break;
		}

		if (i >= ((cvector_s*) _parser->vars)->total)
		{
			break;
		}

		var = cvector_get((cvector_s*) (_parser->vars), (int) i);
		if (!var)
		{
			CC_VAR_DEBUG("DEBUG: variable with index '%d' of '%u' is 'null'", i,
					(unsigned )((cvector_s* ) _parser->vars)->total);
			cvector_delete((cvector_s*) _parser->vars, i);

			continue;
		}

		if (_parser->depth < var->scope)
		{

			CC_VAR_DEBUG("debug: destroy scoped variable '%s'!\n\n", var->name);

			cvector_delete_dealloc((cvector_s*) _parser->vars, i, freeVar);

			continue;
		}

		i++;

	}
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
