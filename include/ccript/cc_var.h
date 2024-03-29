/*
 * This file is a part of "CCRipt" Embeddable Custom 'C' Like Scripting Language Interpreter
 */

/**
 * @file cc_var.h
 * @brief Definice funkci pro praci s promennymi.
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

#include <stddef.h>

#include <stdbool.h>

#include "ccript/cc_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**********************************/
/**********************************/
/**  INICIALIZACE/DEINICIALIZACE **/
/**********************************/
/**********************************/

/**
 * @fn bool VarInit(cc_parser_s*)
 * @brief Inicializuje a rezervuje pamet.
 *
 * @param _parser Ukazatel na parser.
 *
 * @return FALSE jestli neni dost pameti, jinak TRUE.
 */
bool VarInit(cc_parser_s *_parser);

/**
 * @fn void VarDeinit(cc_parser_s*)
 * @brief Odstrani vsechny promenne a uvolni pamet.
 *
 * @note Toto se netyka promennych ktere nejsou soucasti parseru. napriklad argumenty funkci...
 *
 * @param _parser
 */
void VarDeinit(cc_parser_s *_parser);

/**
 * @fn void VarClean(cc_parser_s*)
 * @brief Odstrani vsechny promenne.
 *
 * @param _parser Ukazatel na parser.
 */
void VarClean(cc_parser_s *_parser);

/**********************************/
/**********************************/
/** VYTVARENI/HLEDANI/ODSTRANENI **/
/**********************************/
/**********************************/

/**
 * @fn bool VarExists(cc_parser_s*, const char*, size_t)
 * @brief Overi existenci promenne.
 *
 * @param _parser	Ukazatel na parser.
 * @param _name		Nazev promenne.
 * @param _name_len Delka nazvu promenne.
 *
 * @return TRUE jestli promenna byla nalezena, jinak FALSE.
 */
bool VarExists(cc_parser_s *_parser, const char *_name, size_t _name_len);

/**
 * @fn var_s* VarCreate(const char*, uint8_t, cc_type_t, uint8_t)
 * @brief Vytvori objekt predstavujici promennou.
 *
 * @param _name		Nazev prommene.
 * @param _name_len Delka nazvu promenne.
 * @param _type		Datovy TYP promenne.
 * @param _scope	Rozsah platnosti.
 *
 * @return FALSE jestli neni pamet nebo jmeno je prazdne. Jinak TRUE.
 */
var_s* VarCreate(const char *_name, uint8_t _name_len, cc_type_t _type, uint8_t _scope);

/**
 * @fn var_s* VarCreateArray(const char*, size_t, uint8_t, cc_type_t)
 * @brief NOT IMPLEMENTED
 *
 * @param _name
 * @param _name_len
 * @param _size
 * @param _type
 *
 * @return
 */
var_s* VarCreateArray(const char *_name, size_t _name_len, uint8_t _size, cc_type_t _type);

/**
 * @fn var_s* VarCopy(cc_parser_s*, var_s*)
 * @brief Vytvori kopii promenne.
 *
 * @param _parser Ukazatel na parser.
 * @param _var Ukazatel na promennou ktera bude zkopirovana.
 * @return Ukazatel na novou promennou nebo NULL jestli neni dost pameti.
 */
var_s* VarCopy(cc_parser_s *_parser, var_s *_var);

/**
 * @fn bool VarStore(cc_parser_s*, var_s*, bool)
 * @brief Ulozi promennou pro dalsi pouziti.
 *
 * @param _parser Ukazatel na parser.
 * @param _var Ukazatel na promennou.
 * @param _overwrite Prepsat existujici.
 *
 * @return FALSE jestli neni pamet nebo promenna s timto nazvem jiz existuje. Jinak TRUE.
 */
bool VarStore(cc_parser_s *_parser, var_s *_var, bool _overwrite);

/**
 * @fn bool VarFindAndDestroy(cc_parser_s*, const char*, size_t)
 * @brief Vyhleda a odstrani promennou a pamet po ni.
 *
 * @param _parser Ukazatel na parser.
 * @param _name Nazev promenne k hledani.
 * @param _len Delka nazvu promenne.
 *
 * @return FALSE jestli neni nalezeno, jinak TRUE.
 */
bool VarFindAndDestroy(cc_parser_s *_parser, const char *_name, size_t _len);

/**
 * @fn void VarDestroy(var_s*)
 * @brief Odstrani promennou a uvolni pamet.
 *
 * @param _var Ukazatel na promennou.
 */
void VarDestroy(var_s *_var);

/**
 * @fn var_s* VarGet(cc_parser_s*, const char*, size_t)
 * @brief Vrati promennou podle jejiho nazvu.
 *
 * @param _parser Ukazatel na parser.
 * @param _name Nazev promenne k hledani.
 * @param _len Delka nazvu promenne ve znacich.
 *
 * @return Ukazatel na promennou, jestli nenalezeno tak NULL.
 */
var_s* VarGet(cc_parser_s *_parser, const char *_name, size_t _len);

/**********************************/
/**********************************/
/**         PRACE S DATY         **/
/**********************************/
/**********************************/

/**********************************/
/**********************************/
/**           GETTERS            **/
/**********************************/
/**********************************/

/**
 *
 *
 * @fn bool VarValueGetBool(cc_parser_s *, var_s*, bool*)
 * @brief Ziska data z promenne.
 *
 *
 *
 * @param _parser Ukazatel na parser.
 * @param _var Ukazatel na promennou.
 * @param _bool Ukazatel na promennou do ktere se ulozi data.
 *
 * @return FALSE jestli promenna neni typu BOOL nebo neni inicializovana. Jinak TRUE..
 */
bool VarValueGetBool(cc_parser_s *_parser, var_s *_var, bool *_bool);

/**
 * @fn bool VarValueGetFloat(cc_parser_s *, var_s*, float*)
 * @brief Ziska data z promenne.
 *
 * @param _parser Ukazatel na parser.
 * @param _var Ukazatel na promennou.
 * @param _float Ukazatel na promennou do ktere se ulozi data.
 *
 * @return FALSE jestli promenna neni typu FLOAT nebo neni inicializovana. Jinak TRUE..
 */
bool VarValueGetFloat(cc_parser_s *_parser, var_s *_var, float *_float);

/**
 * @fn bool VarValueGetChar(cc_parser_s *, var_s*, char*)
 * @brief Ziska data z promenne.
 *
 * @param _parser Ukazatel na parser.
 * @param _var Ukazatel na promennou.
 * @param _char Ukazatel na promennou do ktere se ulozi data.
 *
 * @return FALSE jestli promenna neni typu CHAR nebo neni inicializovana. Jinak TRUE..
 */
bool VarValueGetChar(cc_parser_s *_parser, var_s *_var, char *_char);

/**
 * @fn bool VarValueGetInt(cc_parser_s *, var_s*, int*)
 * @brief Ziska data z promenne.
 *
 * @param _parser Ukazatel na parser.
 * @param _var Ukazatel na promennou.
 * @param _int Ukazatel na promennou do ktere se ulozi data.
 *
 * @return FALSE jestli promenna neni typu INT nebo neni inicializovana. Jinak TRUE..
 */
bool VarValueGetInt(cc_parser_s *_parser, var_s *_var, int *_int);

/**
 * @fn bool VarValueGetLong(cc_parser_s*, var_s*, long*)
 * @brief Ziska data z promenne.
 *
 * @param _parser Ukazatel na promennou.
 * @param _var Ukazatel na parser.
 * @param _int Ukazatel na promennou do ktere se ulozi data.
 *
 * @return FALSE jestli promenna neni typu LONG nebo neni inicializovana. Jinak TRUE..
 *
 */
bool VarValueGetLong(cc_parser_s *_parser, var_s *_var, long *_int);

/**
 * @fn bool VarValueGetString(cc_parser_s *, var_s*, char*, size_t*)
 * @brief Ziska data z promenne. Tady retezec.
 *
 * @param _parser Ukazatel na parser.
 * @param[in] _var Ukazatel na promennou.
 * @param[out] _str Zasobnik do ktereho se ulozi data.
 * @param[out] _len Delka retezce. V pripade selhani, s chybou ze predany zasobnik pro retezec
 * 					neni dost velky, se do ni ulozi kolik je potreba mista pro ulozeni dat.
 * 					Velikost je ve znacich.
 *
 * @return FALSE jestli promenna neni typu STRING, jestli neni inicializovana, jestli predany
 *  				zasobnik neni dostatecne velky. Jinak TRUE..
 */
bool VarValueGetString(cc_parser_s *_parser, var_s *_var, char *_str, size_t *_len);

/**********************************/
/**********************************/
/**           SETTERS            **/
/**********************************/
/**********************************/

/**
 * @fn bool VarValueSetInt(cc_parser_s *, var_s*, int)
 * @brief Priradi k promenne cislo.
 *
 * @note Vraci FALSE kdyz promenna neni typu 'INT'
 *
 * @param _parser Ukazatel na parser.
 * @param _var Promenna pro prirazeni
 * @param _int Cislo k prirazeni
 *
 * @return FALSE jestli neni pamet nebo spatny typ. Jinak TRUE
 *
 */
bool VarValueSetInt(cc_parser_s *_parser, var_s *_var, int _int);

/**
 * @fn bool VarValueSetLong(cc_parser_s*, var_s*, long)
 * @brief Priradi k promenne cislo.
 *
 * @note Vraci FALSE kdyz promenna neni typu 'LONG'
 *
 * @param _parser Ukazatel na parser.
 * @param _var Promenna pro prirazeni
 * @param _long Cislo k prirazeni
 *
 * @return FALSE jestli neni pamet nebo spatny typ. Jinak TRUE
 *
 */
bool VarValueSetLong(cc_parser_s *_parser, var_s *_var, long _long);

/**
 * @fn bool VarValueSetBool(cc_parser_s *, var_s*, bool)
 * @brief Priradi promenne pravdivostni hodnotu
 *
 * @note Vraci FALSE kdyz promenna neni typu 'BOOL'
 *
 * @param _parser Ukazatel na parser.
 * @param _var Promenna pro prirazeni
 * @param _bool Boolean k prirazeni
 *
 * @return FALSE jestli neni pamet nebo spatny typ. Jinak TRUE
 *
 */
bool VarValueSetBool(cc_parser_s *_parser, var_s *_var, bool _bool);

/**
 * @fn bool VarValueSetFloat(cc_parser_s *, var_s*, float)
 * @brief   Priradi k promenne cislo s plovouci carkou.
 *
 * @note Vraci FALSE kdyz promenna neni typu 'FLOAT'
 *
 * @param _parser Ukazatel na parser.
 * @param _var Promenna pro prirazeni
 * @param _float Cislo s desetinou carkou k prirazeni
 *
 * @return FALSE jestli neni pamet nebo spatny typ. Jinak TRUE
 */
bool VarValueSetFloat(cc_parser_s *_parser, var_s *_var, float _float);

/**
 * @fn bool VarValueSetChar(cc_parser_s *, var_s*, char)
 * @brief  Priradi k promenne znak.
 *
 * @note Vraci FALSE kdyz promenna neni typu 'CHAR'
 *
 * @param _parser Ukazatel na parser.
 * @param _var Promenna pro prirazeni
 * @param _char Znak k prirazeni
 *
 * @return FALSE jestli neni pamet nebo spatny typ. Jinak TRUE
 */
bool VarValueSetChar(cc_parser_s *_parser, var_s *_var, char _char);

/**
 * @fn bool VarValueSetString(cc_parser_s *, var_s*, char*, size_t)
 * @brief Priradi k promenne retezec.
 *
 * @note Vraci FALSE kdyz promenna neni typu 'STRING'
 *
 * @param _parser Ukazatel na parser.
 * @param _var Promenna pro prirazeni
 * @param _str Retezec k prirazeni
 * @param _str_len Delka retezce
 *
 * @return FALSE jestli neni pamet nebo spatny typ. Jinak TRUE
 */
bool VarValueSetString(cc_parser_s *_parser, var_s *_var, char *_str, size_t _str_len);

/**********************************/
/**********************************/
/**    PRACE S TYPEM PROMENNE    **/
/**********************************/
/**********************************/

/**
 * @fn var_s* VarCastToInt(cc_parser_s *, var_s*)
 * @brief  Prevede promennou na typ INT
 *
 * @param _parser Ukazatel na parser.
 * @param _var_from Promenna k prevodu
 *
 * @return Nova promenna typu INT
 */
var_s* VarCastToInt(cc_parser_s *_parser, var_s *_var_from);

/**
 * @fn var_s* VarCastToLong(cc_parser_s*, var_s*)
 * @brief  Prevede promennou na typ LONG
 *
 * @param _parser Ukazatel na parser.
 * @param _var_from Promenna k prevodu
 *
 * @return Nova promenna typu LONG
 *
 */
var_s* VarCastToLong(cc_parser_s *_parser, var_s *_var_from);

/**
 * @fn var_s* VarCastToBool(cc_parser_s *, var_s*)
 * @brief  Prevede promennou na typ BOOL
 *
 * @param _parser Ukazatel na parser.
 * @param _var_from Promenna k prevodu
 *
 * @return Nova promenna typu BOOL
 */
var_s* VarCastToBool(cc_parser_s *_parser, var_s *_var_from);

/**
 * @fn var_s* VarCastToFloat(cc_parser_s *, var_s*)
 * @brief  Prevede promennou na typ FLOAT
 *
 * @param _parser Ukazatel na parser.
 * @param _var_from Promenna k prevodu
 *
 * @return Nova promenna typu FLOAT
 */
var_s* VarCastToFloat(cc_parser_s *_parser, var_s *_var_from);

/**
 * @fn var_s* VarCastToString(cc_parser_s *, var_s*)
 * @brief  Prevede promennou na typ STRING
 *
 * @param _parser Ukazatel na parser.
 * @param _var_from Promenna k prevodu
 *
 * @return Nova promenna typu STRING
 */
var_s* VarCastToString(cc_parser_s *_parser, var_s *_var_from);

/**
 * @fn var_s* VarCastToChar(cc_parser_s *, var_s*)
 * @brief  Prevede promennou na typ CHAR
 *
 * @param _parser Ukazatel na parser.
 * @param _var_from Promenna k prevodu
 *
 * @return Nova promenna typu CHAR
 */
var_s* VarCastToChar(cc_parser_s *_parser, var_s *_var_from);

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
