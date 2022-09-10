/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file cc_log.h
 * @brief Asi k nicemu
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

#define S1(x) #x
#define S2(x) S1(x)
#define CC_LOG_TAG __FILE__ " : " S2(__LINE__)

