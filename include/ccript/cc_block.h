/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file cc_block.h
 * @brief Funkce pro parsovani funkcnich bloku ve skripte
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

/**
 * @fn bool parseBlock(cc_parser_s*, char)
 * @brief Zpracuje blok.
 *
 * @param _parser Ukazatel na parser.
 * @param _end_char Ukoncovaci znak.
 *
 * @details	doplnit
 *
 * @return FALSE jestli dojde k chybe, jinak TRUE.
 */
var_s* parseBlock(cc_parser_s *_parser, char _end_char);

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
 * @fn bool blockInit(cc_parser_s*)
 * @brief Inicializuje kontejner pro funkcni bloky.
 *
 * @param _parser Ukazatel na parser.
 *
 * @return TRUE jestli bez problemu, jinak FALSE.
 */
bool blockInit(cc_parser_s *_parser);

/**
 * @fn void blockDeinit(cc_parser_s*)
 * @brief Deinicializuje kontejner pro funkcni bloky.
 *
 * @param _parser Ukazatel na parser.
 */
void blockDeinit(cc_parser_s *_parser);

/**
 * @fn cc_block_s* blockGet(cc_parser_s*, const char*, size_t)
 * @brief Vratu ukazatel na strukturu funkcniho bloku.
 *
 * @param _parser Ukazatel na parser.
 * @param _name Nazev bloku.
 * @param _name_len Delka nazvu bloku.
 *
 * @return Ukazatel na blok nebo NULL pri chybe.
 */
cc_block_s* blockGet(cc_parser_s *_parser, const char *_name, size_t _name_len);

/**
 * @fn var_s* blockCall(cc_parser_s*, cc_block_s*)
 * @brief Zavola blok a vrati vysledek jeho prace.
 *
 * @param _parser Ukazatel na parser.
 * @param _block Ukazatel na strukturu funkcniho bloku.
 *
 * @return Ukazatel na promennou nebo NULL pri chybe.
 */
var_s* blockCall(cc_parser_s *_parser, cc_block_s *_block);

/**
 * @fn bool parseDefineBlock(cc_parser_s*, cc_type_t, const char*, size_t)
 * @brief Definuje strukturu funkcniho bloku.
 *
 * @param _parser Ukazatel na parser.
 * @param _type Typ dat ktery vraci (NEIMPLEMENTOVANO!)
 * @param _block_name Nazev bloku.
 * @param _block_name_len Delka nazvu bloku.
 *
 * @return TRUE jestli bez problemu, jinak FALSE.
 */
bool parseDefineBlock(cc_parser_s *_parser, cc_type_t _type, const char *_block_name,
		size_t _block_name_len);

/**
 * @fn bool parseBlockArguments(cc_parser_s*, cc_block_args_s**, uint8_t*)
 * @brief Zaska argumenty definovaneho funkcniho bloku.
 *
 * @param _parser Ukazatel na parser.
 * @param _args Sem se ulozi ziskane argumenty.
 * @param _args_count	Sem se ulozi pocet ziskanych argumentu.
 *
 * @return TRUE jestli bez problemu, jinak FALSE.
 */
bool parseBlockArguments(cc_parser_s *_parser, cc_block_args_s **_args, uint8_t *_args_count);

/**
 * @fn void garbageBlockDefArgs(cc_block_args_s**, uint8_t*)
 * @brief Odstrani argumenty po volani funkcniho bloku.
 *
 * @param _args Ukazatel na pole s argumenty.
 * @param _args_count Pocet argumentu v poli.
 */
void garbageBlockDefArgs(cc_block_args_s **_args, uint8_t *_args_count);

/**
 * @fn bool parseBlockCallArguments(cc_parser_s*, cc_block_s*, var_s**, uint8_t*)
 * @brief Zaska argumenty volaneho funkcniho bloku.
 * @note Asi neno volana!?
 *
 * @param _parser Ukazatel na parser.
 * @param _block Ukazatel na strukturu definice funkcniho bloku.
 * @param _args Sem se ulozi ziskane argumenty.
 * @param _args_count Sem se ulozi pocet ziskanych argumentu.
 *
 * @return TRUE jestli bez problemu, jinak FALSE.
 */
bool parseBlockCallArguments(cc_parser_s *_parser, cc_block_s *_block, var_s **_args,
		uint8_t *_args_count);

/**
 * @fn void dumpBlockArgs(cc_block_args_s**, uint8_t)
 * @brief Vypise informace o argumentech na STD.
 *
 * @param _args Ukazatel na pole s argumenty.
 * @param _args_count Pocet argumentu v poli.
 */
void dumpBlockArgs(cc_block_args_s **_args, uint8_t _args_count);

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

