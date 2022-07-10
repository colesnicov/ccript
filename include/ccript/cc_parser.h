/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file cc_parser.h
 * @brief Funkce pro parsovani skriptu
 *
 * @version 1b0
 * @date 26.06.2022
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
// Definice nebo definice a rirazeni promenne
//
// type var;
// ^^^^^^^^
// type var = ...;
// ^^^^^^^^^^^
//
////////////////////////////////

/**
 * @fn bool ParseDefineTypeBool(parser_s*, char*)
 * @brief Parsuje definici nove promenne nebo funkce typu BOOL.
 *
 * @param _parser Ukazatel na parser.
 * @param _keyword_name Sdilene uloziste pro ziskany retezec. Kvuli setreni ram?
 *
 * @return FALSE ,jinak TRUE.
 */
bool ParseDefineTypeBool(parser_s *_parser, char *_keyword_name);

/**
 * @fn bool ParseDefineTypeInt(parser_s*)
 * @brief Parsuje definici nove promenne nebo funkce typu INT.
 *
 * @param _parser Ukazatel na parser.
 *
 * @return FALSE ,jinak TRUE.
 */
bool ParseDefineTypeInt(parser_s *_parser);

/**
 * @fn bool ParseDefineTypeFloat(parser_s*)
 * @brief Parsuje definici nove promenne nebo funkce typu FLOAT.
 *
 * @param _parser Ukazatel na parser.
 *
 * @return FALSE ,jinak TRUE.
 */
bool ParseDefineTypeFloat(parser_s *_parser);

/**
 * @fn bool ParseDefineTypeChar(parser_s*)
 * @brief Parsuje definici nove promenne nebo funkce typu CHAR.
 *
 * @param _parser Ukazatel na parser.
 *
 * @return FALSE ,jinak TRUE.
 */
bool ParseDefineTypeChar(parser_s *_parser);

/**
 * @fn bool ParseDefineTypeString(parser_s*)
 * @brief Parsuje definici nove promenne nebo funkce typu STRING.
 *
 * @param _parser Ukazatel na parser.
 *
 * @return FALSE ,jinak TRUE.
 */
bool ParseDefineTypeString(parser_s *_parser);

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
 * @fn bool parseVarAssign(parser_s*, char*, size_t)
 * @brief Parsuje prirazeni promenne.
 *
 * @param _parser Ukazatel na parser.
 * @param _var_name Nazev promenne.
 * @param _var_len Delka nazvu promenne.
 *
 * @return FALSE ,jinak TRUE.
 */
bool parseVarAssign(parser_s *_parser, char *_var_name, size_t _var_len);

/**
 * @fn bool parseVarDelete(parser_s*, char*)
 * @brief Parsuje odstraneni promenne.
 *
 * @param _parser Ukazatel na parser.
 * @param _var_name Nazev promenne.
 *
 * @return FALSE ,jinak TRUE.
 */
bool parseVarDelete(parser_s *_parser, char *_var_name);

////////////////////////////////
//
// Ziskani argumentu promenne
//
// var = argument;
//       ^^^^^^^^^
//
////////////////////////////////

/**
 * @fn bool parseVarArgsBool(parser_s*, char, bool*)
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
bool parseVarArgsBool(parser_s *_parser, char _symbol_end, bool *_value);

/**
 * @fn bool parseVarArgsFloat(parser_s*, char, float*)
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
bool parseVarArgsFloat(parser_s *_parser, char _symbol_end, float *_value);

/**
 * @fn bool parseVarArgsInt(parser_s*, char, int*)
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
 * @return
 */
bool parseVarArgsInt(parser_s *_parser, char _symbol_end, int *_value);

/**
 * @fn bool parseVarArgsString(parser_s*, char, char*, size_t*)
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
bool parseVarArgsString(parser_s *_parser, char _symbol_end, char *_value, size_t *_value_len);

/**
 * @fn bool parseVarArgsChar(parser_s*, char, char*)
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
bool parseVarArgsChar(parser_s *_parser, char _symbol_end, char *_value);

////////////////////////////////
//
// levy operator
//
//  ++var nebo --var
//  ^^^^^      ^^^^^
//
////////////////////////////////

/**
 * @fn bool parseVarIncrementLeft(parser_s*, char*, size_t)
 * @brief Parsuje levou matematickou operaci odecteni 1 '--'.
 *
 * @param _parser Ukazatel na parser.
 * @param _var_name Nazev promenne.
 * @param _var_name_len Delka nazvu promenne.
 *
 * @return FALSE ,jinak TRUE.
 */
bool parseVarIncrementLeft(parser_s *_parser, char *_var_name, size_t _var_name_len);

/**
 * @fn bool parseVarDecrementLeft(parser_s*, char*, size_t)
 * @brief  Parsuje levou matematickou operaci pricteni 1 '++'.
 *
 * @param _parser Ukazatel na parser.
 * @param _var_name Nazev promenne.
 * @param _var_name_len Delka nazvu promenne.
 *
 * @return FALSE ,jinak TRUE.
 */
bool parseVarDecrementLeft(parser_s *_parser, char *_var_name, size_t _var_name_len);

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
 * @fn bool parseVarIncrementRight(parser_s*, char*, size_t)
 * @brief  Parsuje pravou matematickou operaci pricteni nebo prirazeni a operaci.
 * @details Napriklad toto `var++;` nebo toto `var += 1;`.
 *
 * @param _parser Ukazatel na parser.
 * @param _var_name Nazev promenne.
 * @param _var_name_len Delka nazvu promenne.
 *
 * @return FALSE ,jinak TRUE.
 */
bool parseVarIncrementRight(parser_s *_parser, char *_var_name, size_t _var_name_len);

/**
 * @fn bool parseVarDecrementRight(parser_s*, char*, size_t)
 * @brief  Parsuje pravou matematickou operaci odecteni nebo prirazeni a operaci.
 * @details Napriklad toto `var--;` nebo toto `var -= 1;`.
 *
 * @param _parser Ukazatel na parser.
 * @param _var_name Nazev promenne.
 * @param _var_name_len Delka nazvu promenne.
 *
 * @return FALSE ,jinak TRUE.
 */
bool parseVarDecrementRight(parser_s *_parser, char *_var_name, size_t _var_name_len);

/**
 * @fn bool parseVarMultiplyRight(parser_s*, char*, size_t)
 * @brief  Parsuje pravou matematickou operaci a prirazeni.
 * @details Toto `var *= 2;`.
 *
 * @param _parser Ukazatel na parser.
 * @param _var_name Nazev promenne.
 * @param _var_name_len Delka nazvu promenne.
 *
 * @return FALSE ,jinak TRUE.
 */
bool parseVarMultiplyRight(parser_s *_parser, char *_var_name, size_t _var_name_len);

/**
 * @fn bool parseVarDivideRight(parser_s*, char*, size_t)
 * @brief  Parsuje pravou matematickou operaci a prirazeni.
 * @details Toto `var /= 2;`.
 *
 * @param _parser Ukazatel na parser.
 * @param _var_name Nazev promenne.
 * @param _var_name_len Delka nazvu promenne.
 *
 * @return FALSE ,jinak TRUE.
 */
bool parseVarDivideRight(parser_s *_parser, char *_var_name, size_t _var_name_len);

//////////////////////////////
//
// Preskoceni casti skriptu
//
// ab { cd } ef
//    ^^^^^^
//
//////////////////////////////

/**
 * @fn void parseSkipNewLine(parser_s*)
 * @brief Preskoci nove radky a mezery dokud nenarazi na neco jineho.
 *
 * @note V pripade selhani nastavuje navratovy kod. @see cc_setError()
 *
 * @param _parser Ukazatel na parser.
 */
void parseSkipNewLine(parser_s *_parser);

/**
 * @fn void bufferSkipSpace(parser_s*)
 * @brief Preskoci mezery dokud nenarazi na neco jineho.
 *
 * @note V pripade selhani nastavuje navratovy kod. @see cc_setError()
 *
 * @param _parser Ukazatel na parser.
 */
void bufferSkipSpace(parser_s *_parser);

/**
 * @fn bool parseSkipComment(parser_s*)
 * @brief Preskoci komentar.
 *
 * @param _parser Ukazatel na parser.
 *
 * @return TRUE jestli bez problemu, jinak FALSE.
 *
 * @see bufferNext()
 * @see bufferGet()
 */
bool parseSkipComment(parser_s *_parser);

/**
 * @fn bool parserSkipBlock(parser_s*, char, char)
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
bool parserSkipBlock(parser_s *_parser, char _start_char, char _end_char);

/////////////////////////
//
// Ziskani hodnot
//
/////////////////////////

/**
 * @fn size_t parseIdentifier(parser_s*, char*)
 * @brief Ziska nazev promenne nebo funkce.
 * @note V pripade selhani nastavuje navratovy kod. @see cc_setError()
 * @note Nekontroluje jestli vyraz nezacina cislici, vrati vyraz '2string' ale nevrati 'to_string'!
 *
 * @param _parser Ukazatel na parser.
 * @param _name Sem se ulozi nazev.
 *
 * @return TRUE jestli bez problemu, jinak FALSE.
 */
bool parseIdentifier(parser_s *_parser, char *_name, size_t *_name_len);

/**
 * @fn int parseValueBool(parser_s*, char*)
 * @brief Parsuje 'hodnotu' typu BOOL.
 * @details Je to v pripade kdy `bool var = 1;` nebo `var = 1;`.
 *
 * @param _parser Ukazatel na parser.
 * @param _value Sem se ulozi ziskana data.
 * @param _value_len	Sem se ulozi delka ziskanych dat.
 *
 * @return TRUE jestli bez problemu, jinak FALSE.
 */
bool parseValueBool(parser_s *_parser, char *_value, size_t *_value_len);

/**
 * @fn size_t parseValueInt(parser_s*, char*)
 * @brief Parsuje 'hodnotu' typu INT.
 * @details Je to v pripade kdy `int var = 1;` nebo `var = 1;`.
 *
 * @param _parser Ukazatel na parser.
 * @param _value Sem se ulozi ziskana data.
 * @param _value_len	Sem se ulozi delka ziskanych dat.
 *
 * @return TRUE jestli bez problemu, jinak FALSE.
 */
bool parseValueInt(parser_s *_parser, char *_value, size_t *_value_len);

/**
 * @fn size_t parseValueFloat(parser_s*, char*, bool*)
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
bool parseValueFloat(parser_s *_parser, char *_value, size_t *_value_len, bool *_is_float);

/**
 * @fn size_t ParseValueChar(parser_s*, char*, size_t)
 * @brief Parsuje 'hodnotu' typu CHAR.
 * @details Je to v pripade kdy `char var = 'a';` nebo `var = 'a';`.
 *
 * @param _parser		Ukazatel na parser.
 * @param _value		Sem se ulozi ziskana data.
 * @param _value_len	Sem se ulozi delka ziskanych dat.
 *
 * @return TRUE jestli bez problemu, jinak FALSE.
 */
bool ParseValueChar(parser_s *_parser, char *_value, size_t *_value_len);

/**
 * @fn size_t ParseValueString(parser_s*, char*)
 * @brief Parsuje 'hodnotu' typu STRING.
 * @details Je to v pripade kdy `string var = "hello";` nebo `var = "hello";`.
 *
 * @param _parser 		Ukazatel na parser.
 * @param _value 		Sem se ulozi ziskana data.
 * @param _value_len	Sem se ulozi delka ziskanych dat.
 *
 * @return TRUE jestli bez problemu, jinak FALSE.
 */
bool ParseValueString(parser_s *_parser, char *_value, size_t *_value_len);

/**
 * @fn bool parseFuncArguments(parser_s*, const char*, size_t, var_s**, uint8_t*)
 * @brief Parsuje argumenty volane funkce.
 *
 * @param _parser Ukazatel na parser.
 * @param phrase_name Asi k nicemu.
 * @param phrase_name_len Asi k nicemu.
 * @param _args Pole ukazatelu na ziskane argumenty.
 * @param _args_count Sem se ulozi pocet ziskanych argumentu.
 *
 * @return TRUE jestli bez problemu, jinak FALSE.
 */
bool parseFuncArguments(parser_s *_parser, const char *phrase_name, size_t phrase_name_len,
		var_s **_args, uint8_t *_args_count);

/////////////////////////////////
//
// Interni: pozice v souboru, chybovy stav, vycisteni pameti po zanorenych blocich...
//
/////////////////////////////////

/**
 * @fn size_t parseGetPos(parser_s*)
 * @brief Vrati aktualni pozici v souboru.
 *
 * @param _parser Ukazatel na parser.
 *
 * @return Pozici v souboru, 0 jestli selze.
 */
size_t parseGetPos(parser_s *_parser);

/**
 * @fn void parseSetErrorPos(parser_s*, size_t)
 * @brief Nastavi kde, na jake pozici, v souboru doslo k chybe.
 *
 * @param _parser Ukazatel na parser.
 * @param _pos Pozice v souboru.
 */

void parseSetErrorPos(parser_s *_parser, size_t _pos);

/**
 * @fn void parseSetError(parser_s*, cc_code_t)
 * @brief Nastavy pracovni staav parseru.
 *
 * @param _parser Ukazatel na parser.
 * @param _error Pracovni kod
 *
 * @see cc_code_t
 */
void parseSetError(parser_s *_parser, cc_code_t _error);

/**
 * @fn void parseClearScope(parser_s*)
 * @brief Odstrani promenne definovane v zanorennem bloku.
 * @details <pre><code>
 * 			int var1 = 0;
 * 			if(1){ int var2 = 0; }
 * 				   ^^^^^^^^^^^^^
 * 			 </code></pre>
 *
 * @param _parser Ukazatel na parser.
 */
void parseClearScope(parser_s *_parser);

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

