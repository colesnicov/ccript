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
	if (!cc_init(&parser, fileName)) {
		cc_deinit(&parser); // fixme to tady byt nemusi. init po sobe musi uklidi!
		return 2;
	}

	cc_registerFunction(&parser, "dump", 4, stdlib_dump);
	cc_registerFunction(&parser, "millis", 6, stdlib_millis);
	cc_registerFunction(&parser, "sleep", 5, stdlib_sleep);
	cc_registerFunction(&parser, "exit", 5, stdlib_exit);
	cc_registerFunction(&parser, "ccDebugInfo", 11, stdlib_DebugInfo);
	cc_registerFunction(&parser, "castTo", 4, stdlib_cast);
	cc_registerFunction(&parser, "print", 5, stdlib_print);
	cc_registerFunction(&parser, "println", 7, stdlib_println);
	cc_registerFunction(&parser, "system", 6, stdlib_system);
	cc_registerFunction(&parser, "strlen", 6, stdlib_strlen);
	cc_registerFunction(&parser, "strcat", 6, stdlib_strcat);

	cc_parse(&parser, 0);

	if (cc_errorHas(&parser)) {
		CC_PRINT("script fail at position '%lu' with code: '%s(%d)'\n", cc_errorGetPos(&parser),
				cc_errorToString(cc_errorGetCode(&parser)), cc_errorGetCode(&parser));

	}

	CC_PRINT("vars currently allocated %lu bytes.\n", VarGetSizeAll(&parser));

	VarDumpAll(&parser);

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
