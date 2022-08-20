/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file cc_parseWhile.c
 * @brief Implementace funkci pro parsovani smycky 'WHILE'.
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
#include <math.h>
#include "ccript/ccript.h"
#include "ccript/common.h"
#include "ccript/cc_var.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

var_s* parseWhile(parser_s *_parser) {
	char ch = 0;

	parseSkipNewLine(_parser);

	bufferGet(_parser, &ch);

	if (ch != '(') {
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return NULL;
	}

	bool cond_passed = false;

	size_t pos_condition_o;

	pos_condition_o = parseGetPos(_parser);

	if (!parseIfArguments(_parser, &cond_passed)) {
		return NULL;
	}

	bufferNext(_parser);
	parseSkipNewLine(_parser);

	size_t pos_block_o = parseGetPos(_parser);

	bufferGet(_parser, &ch);

	if (ch != '{') {
		parseSetError(_parser, CC_CODE_BAD_SYMBOL);
		parseSetErrorPos(_parser, parseGetPos(_parser));
		return NULL;
	}

	uint8_t scope = _parser->depth;

	while (bufferValid(_parser)) {

		if (cond_passed) {

// todo tady musim posunout o 1 dopredu? zda se ze to jinak nefunguje. mozna new line?
// preskakuji znak '{'
			bufferNext(_parser);

			_parser->depth++;
			var_s *ret_var = parseBlock(_parser, '}');

			_parser->depth = scope;
			parseClearScope(_parser);

			if (_parser->error == CC_CODE_RETURN) {
				return ret_var;
			}

			VarDestroy(ret_var);

			if (_parser->error == CC_CODE_BREAK) {
				// break
				// Navrat na zacatek bloku WHILE abych ho mohl preskocit

				parseSetError(_parser, CC_CODE_OK);

				_parser->buffer->offset = pos_block_o;

				bufferReload(_parser);
				bufferNext(_parser);

				if (!parserSkipBlock(_parser, '{', '}')) {
					return NULL;
				}

				parseSkipNewLine(_parser);

				return NULL;
			}

			else if (_parser->error == CC_CODE_CONTINUE) {
				// continue
				// Navrat k podmince

				parseSetError(_parser, CC_CODE_OK);

				_parser->buffer->offset = pos_condition_o;

				bufferReload(_parser);

				if (!parseIfArguments(_parser, &cond_passed)) {
					return NULL;
				}

				// Navrat na zacatek bloku WHILE a overeni jestli podminka prosla

				_parser->buffer->offset = pos_block_o;

				bufferReload(_parser);

				continue;

			}

			else if (_parser->error == CC_CODE_OK) {
				// Dosel do ukoncovaci zavorky
				// Navrat k podmince

				_parser->buffer->offset = pos_condition_o;

				bufferReload(_parser);

				if (!parseIfArguments(_parser, &cond_passed)) {
					return NULL;
				}

				// Navrat na zacatek bloku WHILE a overeni jestli podminka prosla

				_parser->buffer->offset = pos_block_o;

				bufferReload(_parser);

				continue;

			}

			else if (_parser->error >= CC_CODE_ERROR) {
				CC_PRINT("ERROR: while exit with code: '%s(%d)' '%c' %d.\n",
						cc_errorToString(_parser->error), _parser->error, ch, _parser->depth);

				return NULL;
			}

			bufferNext(_parser);

			return NULL;

		} else {
			//	preskocit blok while

			parseSkipNewLine(_parser);

			if (!parserSkipBlock(_parser, '{', '}')) {
				return NULL;
			}

			parseSkipNewLine(_parser);
			parseSetError(_parser, CC_CODE_OK);

			return NULL;

		}
	}

	parseSetError(_parser, CC_CODE_LOGIC);
	parseSetErrorPos(_parser, parseGetPos(_parser));
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
