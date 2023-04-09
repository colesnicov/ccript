/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file common.h
 * @brief Definice dodatecnych privatnich funkci pro praci se znaky.
 * @details these appear in <ctypes.h> too, but we don't want macros and we don't want to
 * 			 drag in a table of character flags
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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @fn int charin(int, const char*)
 * @brief Je znak v retezci?
 *
 * @param c Znak k hledani.
 * @param str Retezec k prohledani.
 *
 * @return TRUE jestli znak je v retezci, jinak FALSE
 */
int charin(int c, const char *str);

/**
 * @fn int isdigit(int)
 * @brief Je to cisloce v rozmezi od 0 do 9?
 *
 * @param c Znak k overeni.
 * @return TRUE jestli ano, jinak FALSE
 */
int isdigit(int c);

/**
 * @fn int ishexchar(int)
 * @brief Je to v ramci HEX?
 *
 * @param c Znak k overeni.
 * @return TRUE jestli ano, jinak FALSE
 */
int ishexchar(int c);

/**
 * @fn int islower(int)
 * @brief Je to male pismeno?
 *
 * @param c Znak k overeni.
 * @return TRUE jestli ano, jinak FALSE
 */
int islower(int c);

/**
 * @fn int isupper(int)
 * @brief Je to velke pismeno?
 *
 * @param c Znak k overeni.
 * @return TRUE jestli ano, jinak FALSE
 */
int isupper(int c);

/**
 * @fn int isalpha(int)
 * @brief Je to pismeno?
 * @details Nebere ohled na velikost pismen.
 *
 * @param c Znak k overeni.
 * @return TRUE jestli ano, jinak FALSE
 */
int isalpha(int c);

/**
 * @fn int isalnum(int)
 * @brief Je to alfanumericky znak?
 *
 * @param c Znak k overeni.
 * @return TRUE jestli ano, jinak FALSE
 */
int isalnum(int c);

/**
 *
 * @fn int isspace(int)
 * @brief Je to 'prazdny' znak?
 * @details Znaky mezera ( ), return (\\r) a NL (\\n).
 *
 * @param c Znak k overeni.
 * @return TRUE jestli ano, jinak FALSE.
 */
int isspace(int c);

#ifdef __cplusplus
}
#endif

