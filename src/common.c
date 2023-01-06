/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file common.c
 * @brief Implementace dodatecnych privatnich funkci pro praci se znaky.
 * @details these appear in <ctypes.h> too, but we don't want macros and we don't want to
 * 			 drag in a table of character flags
 *
 * @version 1b1
 * @date 26.06.2022
 *
 * @author Denis Colesnicov <eugustus@gmail.com>
 *
 * @copyright MIT License
 *
 */

#include "ccript/common.h"

int charin(int c, const char *str) {
	while (*str) {
		if (c == *str++)
			return 1;
	}
	return 0;
}

int isspace(int c) {
	return (c == ' ') || (c == '\t') || (c == '\r');
}

int isdigit(int c) {
	return (c >= '0' && c <= '9');
}

int ishexchar(int c) {
	return (c >= '0' && c <= '9') || charin(c, "abcdefABCDEF");
}

int islower(int c) {
	return (c >= 'a' && c <= 'z');
}

int isupper(int c) {
	return (c >= 'A' && c <= 'Z');
}

int isalpha(int c) {
	return islower(c) || isupper(c);
}

int isalnum(int c) {
	return isalpha(c) || isdigit(c);
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
///

