/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file test.c
 * @brief Test interpreteru.
 * @details Testovaci stroj UBUNTU 21!
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
#include <ccript/cc_function.h>
#include <ccript/cc_log.h>
#include <ccript/cc_log.h>
#include <ccript/cc_stdlib.h>
#include <ccript/cc_types.h>
#include <ccript/ccript.h>
#include "ccript/cc_var.h"
#include "ccript/cc_var_ext.h"
#include <string.h>

#include <climits>
#include <cstring>

extern "C" int main(int argc, char **argv) {

	printf("CHAR_BIT    :   %d\n", CHAR_BIT);
	printf("CHAR_MAX    :   %d\n", CHAR_MAX);
	printf("CHAR_MIN    :   %d\n", CHAR_MIN);
	printf("INT_MAX     :   %d\n", INT_MAX);
	printf("INT_MIN     :   %d\n", INT_MIN);
	printf("LONG_MAX    :   %ld\n", (long) LONG_MAX);
	printf("LONG_MIN    :   %ld\n", (long) LONG_MIN);
	printf("SCHAR_MAX   :   %d\n", SCHAR_MAX);
	printf("SCHAR_MIN   :   %d\n", SCHAR_MIN);
	printf("SHRT_MAX    :   %d\n", SHRT_MAX);
	printf("SHRT_MIN    :   %d\n", SHRT_MIN);
	printf("UCHAR_MAX   :   %d\n", UCHAR_MAX);
	printf("UINT_MAX    :   %u\n", (unsigned int) UINT_MAX);
	printf("ULONG_MAX   :   %lu\n", (unsigned long) ULONG_MAX);
	printf("USHRT_MAX   :   %d\n", (unsigned short) USHRT_MAX);

	if (argc < 2) {
		CC_PRINT("Pouziti: %s <filename>\n", argv[0]);
		return 0;
	}

	/**
	 * Nazev souboru.
	 */
	char fileName[PATH_MAX] = { "/home/denis/projects/ccript/data/" };
	{
		size_t len = strlen((char*) (argv[1]));
		memcpy(fileName + strlen(fileName), (char*) argv[1], sizeof(char) * len);
	}

	parser_s parser = PARSER_DEFAULT();
	if (!cc_init(&parser)) {
		cc_deinit(&parser); // fixme to tady byt nemusi. init po sobe musi uklidi!
		return 2;
	}

	cc_stdlib_registrate(&parser, NULL);
//	cc_registerFunction(&parser, "dump", 4, stdlib_dump, NULL);
//	cc_registerFunction(&parser, "millis", 6, stdlib_millis, NULL);
//	cc_registerFunction(&parser, "sleep", 5, stdlib_sleep, NULL);
//	cc_registerFunction(&parser, "exit", 5, stdlib_exit, NULL);
//	cc_registerFunction(&parser, "ccDebugInfo", 11, stdlib_DebugInfo, NULL);
//	cc_registerFunction(&parser, "castTo", 4, stdlib_cast, NULL);
//	cc_registerFunction(&parser, "print", 5, stdlib_print, NULL);
//	cc_registerFunction(&parser, "println", 7, stdlib_println, NULL);
//	cc_registerFunction(&parser, "system", 6, stdlib_system, NULL);
//	cc_registerFunction(&parser, "strlen", 6, stdlib_strlen, NULL);
//	cc_registerFunction(&parser, "strcat", 6, stdlib_strcat, NULL);

	var_s *var = cc_parse(&parser, fileName);

	if (var != NULL) {
		int ret = 0;
		cc_varGetInt(var, &ret);
		CC_PRINT("script returns '%d'\n", ret);
	}

	cc_varDestroy(var);

	if (cc_errorHas(&parser)) {
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
