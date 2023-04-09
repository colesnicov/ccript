/*
 * This file is a part of "CCRipt" Embeddable Custom 'C' Like Scripting Language Interpreter
 */

/**
 * @file cc_parser.h
 * @brief Funkce pro parsovani skriptu
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

#include "ccript/cc_types.h"

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////
//
// Definice nebo definice a prirazeni promenne
//
// <type> var;
// ^^^^^^^^^^
// <type> var = ...;
// ^^^^^^^^^^^^^^^^
//
////////////////////////////////

/**
 * @fn bool ParseDefineTypeBool(cc_parser_s*, char*)
 * @brief Parsuje definici nove promenne nebo funkce typu BOOL.
 *
 * @param _parser Ukazatel na parser.
 * @param _identifier_name Sdilene uloziste pro ziskany retezec. Kvuli setreni ram?
 *
 * @return FALSE ,jinak TRUE.
 */
bool ParseDefineTypeBool(cc_parser_s *_parser, char *_identifier_name);

/**
 * @fn bool ParseDefineTypeInt(cc_parser_s*)
 * @brief Parsuje definici nove promenne nebo funkce typu INT.
 *
 * @param _parser Ukazatel na parser.
 *
 * @return FALSE ,jinak TRUE.
 */
bool ParseDefineTypeInt(cc_parser_s *_parser);

/**
 * @fn bool ParseDefineTypeLong(cc_parser_s*)
 * @brief Parsuje definici nove promenne nebo funkce typu LONG.
 *
 * @param _parser Ukazatel na parser.
 * @return False nebo TRUE.
 */
bool ParseDefineTypeLong(cc_parser_s *_parser);

/**
 * @fn bool ParseDefineTypeFloat(cc_parser_s*)
 * @brief Parsuje definici nove promenne nebo funkce typu FLOAT.
 *
 * @param _parser Ukazatel na parser.
 *
 * @return FALSE ,jinak TRUE.
 */
bool ParseDefineTypeFloat(cc_parser_s *_parser);

/**
 * @fn bool ParseDefineTypeChar(cc_parser_s*)
 * @brief Parsuje definici nove promenne nebo funkce typu CHAR.
 *
 * @param _parser Ukazatel na parser.
 *
 * @return FALSE ,jinak TRUE.
 */
bool ParseDefineTypeChar(cc_parser_s *_parser);

/**
 * @fn bool ParseDefineTypeString(cc_parser_s*)
 * @brief Parsuje definici nove promenne nebo funkce typu STRING.
 *
 * @param _parser Ukazatel na parser.
 *
 * @return FALSE ,jinak TRUE.
 */
bool ParseDefineTypeString(cc_parser_s *_parser);

////////////////////////////////
//
// Prirazeni  a odstraneni promenne
//
// var = ...
// ^^^^^^
// delete var;
// ^^^^^^^^^^^
//
////////////////////////////////

/**
 * @fn bool parseVarAssign(cc_parser_s*, char*, size_t)
 * @brief Parsuje prirazeni promenne.
 *
 * @param _parser Ukazatel na parser.
 * @param _var_name Nazev promenne.
 * @param _var_len Delka nazvu promenne.
 *
 * @return FALSE ,jinak TRUE.
 */
bool parseVarAssign(cc_parser_s *_parser, char *_var_name, size_t _var_len);

/**
 * @fn bool parseVarDelete(cc_parser_s*, char*)
 * @brief Parsuje odstraneni promenne.
 *
 * @param _parser Ukazatel na parser.
 * @param _var_name Nazev promenne.
 *
 * @return FALSE ,jinak TRUE.
 */
bool parseVarDelete(cc_parser_s *_parser, char *_var_name);

////////////////////////////////
//
// Ziskani argumentu promenne
//
// var = argument;
//       ^^^^^^^^^
//
////////////////////////////////

/**
 * @fn bool parseVarArgsBool(cc_parser_s*, char, bool*)
 * @brief Parsuje argumenty typu BOOL.
 *
 * @details Ukoncovaci znak muze byt:
 * 			<pre>
 * 			<code>
 * 			= true;
 * 			      ^
 * 			</code>
 * 			nebo
 * 			<code>
 * 			if(true && true) {
 * 			               ^
 * 			</code>
 * 			</pre>
 *
 * @param _parser Ukazatel na parser.
 * @param _symbol_end Ukoncovaci symbol.
 * @param _value Sem se ulozi ziskay argument.
 *
 * @return FALSE ,jinak TRUE.
 */
bool parseVarArgsBool(cc_parser_s *_parser, char _symbol_end, bool *_value);

/**
 * @fn bool parseVarArgsFloat(cc_parser_s*, char, float*)
 * @brief Parsuje argumenty typu FLOAT.
 *
 * @details Ukoncovaci znak muze byt:
 * 			<pre>
 * 			<code>
 * 			= true;
 * 			      ^
 * 			</code>
 * 			nebo
 * 			<code>
 * 			if(true && true) {
 * 			               ^
 * 			</code>
 * 			</pre>
 *
 * @param _parser Ukazatel na parser.
 * @param _symbol_end Ukoncovaci symbol.
 * @param _value Sem se ulozi ziskay argument.
 *
 * @return FALSE ,jinak TRUE.
 */
bool parseVarArgsFloat(cc_parser_s *_parser, char _symbol_end, float *_value);

/**
 * @fn bool parseVarArgsInt(cc_parser_s*, char, int*)
 * @brief Parsuje argumenty typu INT.
 *
 * @details Ukoncovaci znak muze byt:
 * 			<pre>
 * 			<code>
 * 			= true;
 * 			      ^
 * 			</code>
 * 			nebo
 * 			<code>
 * 			if(true && true) {
 * 			               ^
 * 			</code>
 * 			</pre>
 *
 * @param _parser Ukazatel na parser.
 * @param _symbol_end Ukoncovaci symbol.
 * @param _value Sem se ulozi ziskay argument.
 *
 * @return FALSE ,jinak TRUE.
 */
bool parseVarArgsInt(cc_parser_s *_parser, char _symbol_end, int *_value);

/**
 * @fn bool parseVarArgsLong(cc_parser_s*, char, long*)
 * @brief Parsuje argumenty typu LONG.
 *
 * @param _parser Ukazatel na parser.
 * @param _symbol_end Ukoncovaci symbol.
 * @param _value Sem se ulozi ziskay argument.
 *
 * @return FALSE ,jinak TRUE.
 */
bool parseVarArgsLong(cc_parser_s *_parser, char _symbol_end, long *_value);

/**
 * @fn bool parseVarArgsString(cc_parser_s*, char, char*, size_t*)
 * @brief Parsuje argumenty typu STRING.
 *
 * @details Ukoncovaci znak muze byt:
 * 			<pre>
 * 			<code>
 * 			= true;
 * 			      ^
 * 			</code>
 * 			nebo
 * 			<code>
 * 			if(true && true) {
 * 			               ^
 * 			</code>
 * 			</pre>
 *
 * @param _parser Ukazatel na parser.
 * @param _symbol_end Ukoncovaci symbol.
 * @param _value Sem se ulozi ziskany argument.
 * @param _value_len Sem se ulozi delka ziskaneho argumentu.
 *
 * @return FALSE ,jinak TRUE.
 */
bool parseVarArgsString(cc_parser_s *_parser, char _symbol_end, char *_value,
		size_t *_value_len);

/**
 * @fn bool parseVarArgsChar(cc_parser_s*, char, char*)
 * @brief Parsuje argumenty typu CHAR.
 *
 * @details Ukoncovaci znak muze byt:
 * 			<pre>
 * 			<code>
 * 			= true;
 * 			      ^
 * 			</code>
 * 			nebo
 * 			<code>
 * 			if(true && true) {
 * 			               ^
 * 			</code>
 * 			</pre>
 *
 * @param _parser Ukazatel na parser.
 * @param _symbol_end Ukoncovaci symbol.
 * @param _value Sem se ulozi ziskay argument.
 *
 * @return FALSE ,jinak TRUE.
 */
bool parseVarArgsChar(cc_parser_s *_parser, char _symbol_end, char *_value);

////////////////////////////////
//
// levy operator
//
//  ++var nebo --var
//  ^^^^^      ^^^^^
//
////////////////////////////////

/**
 * @fn bool parseVarIncrementLeft(cc_parser_s*, char*, size_t)
 * @brief Parsuje levou matematickou operaci odecteni 1 '--'.
 *
 * @param _parser Ukazatel na parser.
 * @param _var_name Nazev promenne.
 * @param _var_name_len Delka nazvu promenne.
 *
 * @return FALSE ,jinak TRUE.
 */
bool parseVarIncrementLeft(cc_parser_s *_parser, char *_var_name, size_t _var_name_len);

/**
 * @fn bool parseVarDecrementLeft(cc_parser_s*, char*, size_t)
 * @brief  Parsuje levou matematickou operaci pricteni 1 '++'.
 *
 * @param _parser Ukazatel na parser.
 * @param _var_name Nazev promenne.
 * @param _var_name_len Delka nazvu promenne.
 *
 * @return FALSE ,jinak TRUE.
 */
bool parseVarDecrementLeft(cc_parser_s *_parser, char *_var_name, size_t _var_name_len);

////////////////////////////////
//
// pravy operator
//
//  var++ nebo var--
//  ^^^^^      ^^^^^
//  var += nebo var -=
//  ^^^^^^      ^^^^^^
//  var /= nebo var *=
//  ^^^^^^      ^^^^^^
//
////////////////////////////////

/**
 * @fn bool parseVarIncrementRight(cc_parser_s*, char*, size_t)
 * @brief  Parsuje pravou matematickou operaci pricteni nebo prirazeni a operaci.
 * @details Napriklad toto `var++;` nebo toto `var += 1;`.
 *
 * @param _parser Ukazatel na parser.
 * @param _var_name Nazev promenne.
 * @param _var_name_len Delka nazvu promenne.
 *
 * @return FALSE ,jinak TRUE.
 */
bool parseVarIncrementRight(cc_parser_s *_parser, char *_var_name, size_t _var_name_len);

/**
 * @fn bool parseVarDecrementRight(cc_parser_s*, char*, size_t)
 * @brief  Parsuje pravou matematickou operaci odecteni nebo prirazeni a operaci.
 * @details Napriklad toto `var--;` nebo toto `var -= 1;`.
 *
 * @param _parser Ukazatel na parser.
 * @param _var_name Nazev promenne.
 * @param _var_name_len Delka nazvu promenne.
 *
 * @return FALSE ,jinak TRUE.
 */
bool parseVarDecrementRight(cc_parser_s *_parser, char *_var_name, size_t _var_name_len);

/**
 * @fn bool parseVarMultiplyRight(cc_parser_s*, char*, size_t)
 * @brief  Parsuje pravou matematickou operaci a prirazeni.
 * @details Toto `var *= 2;`.
 *
 * @param _parser Ukazatel na parser.
 * @param _var_name Nazev promenne.
 * @param _var_name_len Delka nazvu promenne.
 *
 * @return FALSE ,jinak TRUE.
 */
bool parseVarMultiplyRight(cc_parser_s *_parser, char *_var_name, size_t _var_name_len);

/**
 * @fn bool parseVarDivideRight(cc_parser_s*, char*, size_t)
 * @brief  Parsuje pravou matematickou operaci a prirazeni.
 * @details Toto `var /= 2;`.
 *
 * @param _parser Ukazatel na parser.
 * @param _var_name Nazev promenne.
 * @param _var_name_len Delka nazvu promenne.
 *
 * @return FALSE ,jinak TRUE.
 */
bool parseVarDivideRight(cc_parser_s *_parser, char *_var_name, size_t _var_name_len);

//////////////////////////////
//
// Preskoceni casti skriptu
//
// ab { cd } ef
//    ^^^^^^
//
//////////////////////////////

/**
 * @fn void parseSkipNewLine(cc_parser_s*)
 * @brief Preskoci nove radky a mezery dokud nenarazi na neco jineho.
 *
 * @note V pripade selhani nastavuje navratovy kod. @see cc_setError()
 *
 * @param _parser Ukazatel na parser.
 */
void parseSkipNewLine(cc_parser_s *_parser);

/**
 * @fn bool parseSkipComment(cc_parser_s*)
 * @brief Preskoci komentar.
 *
 * @param _parser Ukazatel na parser.
 *
 * @return TRUE jestli bez problemu, jinak FALSE.
 *
 * @see bufferNext()
 * @see bufferGet()
 */
bool parseSkipComment(cc_parser_s *_parser);

/**
 * @fn bool parserSkipBlock(cc_parser_s*, char, char)
 * @brief Preskoci blok.
 *
 * @param _parser Ukazatel na parser.
 * @param _start_char Znak ktery oznacuje zacatek bloku.
 * @param _end_char Znak ktery oznacuje konec bloku.
 *
 * @return TRUE jestli bez problemu, jinak FALSE.
 *
 * @details Tady se udava pocatecni znacka i koncova, aby se vedelo o urovni zanoreni,
 * 			zanoreni muze byt takove:
 * 			```
 * 			{
 * 				// here started (dept == 0)
 * 				{ // here increase depth (dept++)
 * 				...
 * 				} // here decrease depth (depth--)
 * 			} /// here ends (dept == 0)
 * 			```
 *
 * @details Do bloku je treba nejdrive vstoupit:
 * 			```
 * 			if(ch == '{'){
 * 				bufferNext(parser);
 * 				parserSkipBlock(parser, '{', '}');
 * 			}
 * 			...
 * 			```
 *
 * @see bufferNext()
 * @see bufferGet()
 */
bool parserSkipBlock(cc_parser_s *_parser, char _start_char, char _end_char);

/////////////////////////
//
// Ziskani hodnot
//
/////////////////////////

/**
 * @fn bool parseIdentifier(cc_parser_s*, char*, size_t*)
 * @brief Ziska nazev promenne nebo funkce.
 * @note V pripade selhani nastavuje navratovy kod. @see cc_setError()
 * @note Nekontroluje jestli vyraz nezacina cislici, vrati vyraz '2string' ale nevrati 'to_string'!
 *
 * @param _parser Ukazatel na parser.
 * @param _name Sem se ulozi nazev.
 * @param _name_len Sem se ulozi delka nazvu.
 *
 * @return TRUE jestli bez problemu, jinak FALSE.
 */
bool parseIdentifier(cc_parser_s *_parser, char *_name, size_t *_name_len);

/**
 * @fn int parseValueBool(cc_parser_s*, char*, size_t *)
 * @brief Parsuje 'hodnotu' typu BOOL.
 * @details Je to v pripade kdy `bool var = 1;` nebo `var = 1;`.
 *
 * @param _parser Ukazatel na parser.
 * @param _value Sem se ulozi ziskana data.
 * @param _value_len	Sem se ulozi delka ziskanych dat.
 *
 * @return TRUE jestli bez problemu, jinak FALSE.
 */
bool parseValueBool(cc_parser_s *_parser, char *_value, size_t *_value_len);

/**
 * @fn size_t parseValueInt(cc_parser_s*, char*, size_t *)
 * @brief Parsuje 'hodnotu' typu INT.
 * @details Je to v pripade kdy `int var = 1;` nebo `var = 1;`.
 *
 * @param _parser Ukazatel na parser.
 * @param _value Sem se ulozi ziskana data.
 * @param _value_len	Sem se ulozi delka ziskanych dat.
 *
 * @return TRUE jestli bez problemu, jinak FALSE.
 */
bool parseValueInt(cc_parser_s *_parser, char *_value, size_t *_value_len);

/**
 * @fn bool parseValueLong(cc_parser_s*, char*, size_t*)
 * @brief Ziska LONG.
 *
 * @param _parser Ukazatel na parser.
 * @param _value Sem se ulozi ziskana data.
 * @param _value_len	Sem se ulozi delka ziskanych dat.
 *
 * @return TRUE jestli bez problemu, jinak FALSE.
 */
bool parseValueLong(cc_parser_s *_parser, char *_value, size_t *_value_len);

/**
 * @fn var_s* parseReturnArguments(cc_parser_s*)
 * @brief Ziska navracenou hodnotu.
 * @details Vracena promenna se pak musi odstranit!
 * @see VarDestroy
 *
 * @param _parser Ukazatel na parser.
 *
 * @return Ukazatel na vracenou promennou.
 */
var_s* parseReturnArguments(cc_parser_s *_parser);

/**
 * @fn size_t parseValueFloat(cc_parser_s*, char*, size_t *, bool*)
 * @brief Parsuje 'hodnotu' typu FLOAT.
 * @details Je to v pripade kdy `float var = 1.5;` nebo `var = 1.5;`.
 *
 * @param _parser Ukazatel na parser.
 * @param _value Sem se ulozi ziskana data.
 * @param _value_len	Sem se ulozi delka ziskanych dat.
 * @param _is_float Sem se ulozi jestli ziskana data obsahuji tecku (type FLOAT).
 *
 * @return TRUE jestli bez problemu, jinak FALSE.
 */
bool parseValueFloat(cc_parser_s *_parser, char *_value, size_t *_value_len, bool *_is_float);

/**
 * @fn size_t ParseValueChar(cc_parser_s*, char*, size_t*)
 * @brief Parsuje 'hodnotu' typu CHAR.
 * @details Je to v pripade kdy `char var = 'a';` nebo `var = 'a';`.
 *
 * @param _parser		Ukazatel na parser.
 * @param _value		Sem se ulozi ziskana data.
 * @param _value_len	Sem se ulozi delka ziskanych dat.
 *
 * @return TRUE jestli bez problemu, jinak FALSE.
 */
bool ParseValueChar(cc_parser_s *_parser, char *_value, size_t *_value_len);

/**
 * @fn size_t ParseValueString(cc_parser_s*, char*, size_t *)
 * @brief Parsuje 'hodnotu' typu STRING.
 * @details Je to v pripade kdy `string var = "hello";` nebo `var = "hello";`.
 *
 * @param _parser 		Ukazatel na parser.
 * @param _value 		Sem se ulozi ziskana data.
 * @param _value_len	Sem se ulozi delka ziskanych dat.
 *
 * @return TRUE jestli bez problemu, jinak FALSE.
 */
bool ParseValueString(cc_parser_s *_parser, char *_value, size_t *_value_len);

/**
 * @fn bool parseFuncArguments(cc_parser_s*, var_s**, uint8_t*)
 * @brief Parsuje argumenty volane funkce.
 *
 * @param _parser Ukazatel na parser.
 * @param _args Pole ukazatelu na ziskane argumenty.
 * @param _args_count Sem se ulozi pocet ziskanych argumentu.
 *
 * @return TRUE jestli bez problemu, jinak FALSE.
 */
bool parseFuncArguments(cc_parser_s *_parser, var_s **_args, uint8_t *_args_count);

/////////////////////////////////
//
// Interni: pozice v souboru, chybovy stav, vycisteni pameti po zanorenych blocich...
//
/////////////////////////////////

/**
 * @fn size_t parseGetPos(cc_parser_s*)
 * @brief Vrati aktualni pozici v souboru.
 *
 * @param _parser Ukazatel na parser.
 *
 * @return Pozici v souboru, 0 jestli selze.
 */
size_t parseGetPos(cc_parser_s *_parser);

/**
 * @fn void parseSetErrorPos(cc_parser_s*, size_t)
 * @brief Nastavi kde, na jake pozici, v souboru doslo k chybe.
 *
 * @param _parser Ukazatel na parser.
 * @param _pos Pozice v souboru.
 */

void parseSetErrorPos(cc_parser_s *_parser, size_t _pos);

/**
 * @fn void parseSetError(cc_parser_s*, cc_code_t)
 * @brief Nastavy pracovni staav parseru.
 *
 * @param _parser Ukazatel na parser.
 * @param _error Pracovni kod
 *
 * @see cc_code_t
 */
void parseSetError(cc_parser_s *_parser, cc_code_t _error);

#ifdef __cplusplus
}
#endif

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

