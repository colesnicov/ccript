/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file test.c
 * @brief Test interpreteru.
 * @details Testovaci stroj UBUNTU 21!
 *
 * @version 1b1.1
 * @date 26.06.2022
 *
 * @author Denis Colesnicov <eugustus@gmail.com>
 *
 * @copyright MIT License
 *
 */
#include <ccript/cc_configs.h>
#include <ccript/cc_function.h>
#include <ccript/cc_stdlib.h>
#include <ccript/cc_types.h>
#include <ccript/ccript.h>
#include "ccript/cc_var.h"
#include "ccript/cc_var_ext.h"

#include <emblib/emblib.h>

#include <stdlib.h>
#include <string.h>

#include <climits>
#include <cstring>

#include "stdlib_impl.h"
extern "C" int main(int argc, char **argv)
{
	if (argc < 2)
	{
		CC_PRINT("Pouziti: %s <filename>\n", argv[0]);
		return 0;
	}

	/**
	 * Nazev souboru.
	 */
	char fileName[PATH_MAX] = {
			"/home/denis/projects/ccript/data/" };
	{
		size_t len = strlen((char*) (argv[1]));
		memcpy(fileName + strlen(fileName), (char*) argv[1], sizeof(char) * len);
	}

	parser_s parser = PARSER_DEFAULT()
	;
	if (!cc_init(&parser))
	{
		cc_deinit(&parser); // fixme to tady byt nemusi. init po sobe musi uklidi!
		return 2;
	}

	cc_stdlib_registrate(&parser, NULL);

	cc_registerFunction(&parser, "millis", 6, stdlib_millis, NULL);
	cc_registerFunction(&parser, "sleep", 5, stdlib_sleep, NULL);
//	cc_registerFunction(&parser, "exit", 5, stdlib_exit, NULL);

	long r = (random() % LONG_MAX);
	int p = 4;
	char str[] = {
			"23:23" };

	cc_env_s env[5] = {
			{
					"trigger",
					cc_type_t::CC_TYPE_INT,
					(void*) ((int*) &p) },
			{
					"action",
					cc_type_t::CC_TYPE_INT,
					(void*) ((int*) &p) },
			{
					"pin",
					cc_type_t::CC_TYPE_INT,
					(void*) ((int*) &p) },
			{
					"time",
					cc_type_t::CC_TYPE_STRING,
					(void*) str },

			{
					"data",
					cc_type_t::CC_TYPE_LONG,
					(void*) &r } };

	var_s *var = cc_parse(&parser, fileName, env, 5);

	if (var != NULL)
	{

		if (var->type == cc_type_::CC_TYPE_STRING)
		{
			char buf[50] = {
					'\0' };
			size_t len = 49;
			cc_varGetString(var, buf, &len);
			CC_PRINT("script returns '%s'\n", buf);
		}
		else
		{
			int ret = 0;
			cc_varGetInt(var, &ret);
			CC_PRINT("script returns '%d'\n", ret);
		}
	}

	cc_varDestroy(var);

	if (cc_errorHas(&parser))
	{
		CC_PRINT("script fail at position '%lu' with code: '%s(%d)'\n", cc_errorGetPos(&parser),
				cc_errorToString(cc_errorGetCode(&parser)), cc_errorGetCode(&parser));

	}

	int err = cc_errorGetCode(&parser);

	cc_deinit(&parser);

	return err;
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
