/*
 * This file is a part of "CCRipt" Embeddable Custom 'C' Like Scripting Language Interpreter
 */

/**
 * @file example.c
 * @brief Test interpreteru.
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
#include "ccript/cc_function.h"
#include "ccript/cc_stdlib.h"
#include "ccript/cc_types.h"
#include "ccript/ccript.h"
#include "ccript/cc_var.h"
#include "ccript/cc_var_ext.h"

#include <emblib/emblib.h>

#include <stdlib.h>
#include <string.h>

#include <limits.h>
#include <string.h>

#include "stdlib_impl.h"

/**
 * @fn int main(int, char**)
 * @brief Jednoducha ukazka pouziti..
 *
 * @param argc Pocet parametru
 * @param argv Pole parametru
 *
 * @return Navratovy kod.
 */
int main(int argc, char **argv)
{
	if (argc < 2)
	{
		CC_PRINT("Pouziti: %s <cesta k souboru.>\n", argv[0]);
		return 0;
	}

	// definujeme parser
	cc_parser_s parser;

	memset(&parser, 0, sizeof(cc_parser_s));

	// inicializjeme parser
	if (!cc_init(&parser))
	{
		return 2;
	}

	// registrace STD funkci
	// deprecated
	cc_stdlib_registrate(&parser, NULL);
	cc_registerFunction(&parser, "abs", 3, stdlib_abs, NULL);

	// registrace vlastnich 'C' funkci
	cc_registerFunction(&parser, "millis", 6, stdlib_millis, NULL);
	cc_registerFunction(&parser, "sleep", 5, stdlib_sleep, NULL);

	// vytvorime si nekolik promennych prostredi
	// tyto promenne jsou staticke a neni mozne je menit behem zpracovani skripty
	float r = (rand() % 360);
	float p = (rand() % 360);
	char id[9] = {
			"a8f350fd" };

	// budeme mit 3 promenne prostredi
	uint8_t env_count = 3;

	// vytvorime je
	cc_env_s env[3] = {
			{
					"set",
					CC_TYPE_FLOAT,
					(void*) &r },
			{
					"ang",
					CC_TYPE_FLOAT,
					(void*) &p },
			{
					"id",
					CC_TYPE_STRING,
					(void*) id } };

	// zpracujeme skriptu. Jako argumenty predavame
	// Ukazatel na parser
	// Cestu k souboru
	// Pole s promennymi prostredi nebo NULL jestli to nepotrebujeme
	// Pocet promennych prostredi v poli nebo 0 jestli nejsou.
	// Jestli skripta neco vraci budeme to mit ve vracene promenne nebo NULL jestli nevraci nic.
	var_s *var = cc_parse(&parser, (char*) argv[1], env, env_count);

	// Vypiseme si co nam vratila scripta.
	if (var != NULL)
	{
		CC_PRINT("script returns:\n");
		VarDump(var);
	}
	else
	{
		CC_PRINT("script no returns value.\n");
	}

	// Odstranime vracenou promennou.
	cc_varDestroy(var);

	// Overime jestli behem zpracovani skripty doslo k chybe?
	if (cc_errorHas(&parser))
	{
		// Vypiseme si info o chybe.
		CC_PRINT("script fail at position '%lu' with code: '%s(%d)'\n", cc_errorGetPos(&parser),
				cc_errorToString(cc_errorGetCode(&parser)), cc_errorGetCode(&parser));
	}

	// Ziskame navraceny kod parseru.
	int err = cc_errorGetCode(&parser);

	// Deinicializace parseru
	cc_deinit(&parser);

	// Konec
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
