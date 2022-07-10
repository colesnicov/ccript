/*
 * This file is a part of "CCRipt" Embeddable C Like Scripting Language Interpreter
 */

/**
 * @file cc_buffer.c
 * @brief Implementace dynamickeho pristupu k obsahu skriptu v souboru.
 *
 * @version 1b0
 * @date 26.06.2022
 *
 * @author Denis Colesnicov <eugustus@gmail.com>
 *
 * @copyright MIT License
 *
 */

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>

#include "ccript/cc_buffer.h"
#include "ccript/cc_configs.h"
#include "ccript/cc_parser.h"
#include "ccript/cc_types.h"

#include CONFIG_CC_INCLUDE_FILEIO

bool bufferReload(parser_s *_parser) {

	memset(_parser->buffer->data, '\0', CC_BUFFER_SIZE);

	if (_parser->buffer->offset > CC_BUFFER_SIZE) {
		_parser->buffer->offset -= CC_BUFFER_SIZE;
	} else {
		_parser->buffer->offset = 0;
	}

	if (fseek(_parser->buffer->fd, (long int)_parser->buffer->offset, SEEK_SET) != 0) {
		parseSetErrorPos(_parser, parseGetPos(_parser));
		parseSetError(_parser, CC_CODE_IO);

		CC_PRINT("ERROR: can't set position '%lu' in file!\n", _parser->buffer->offset);

		return false;
	}

	_parser->buffer->length = fread(_parser->buffer->data, sizeof(char), CC_BUFFER_SIZE,
			_parser->buffer->fd);
	if (_parser->buffer->length == 0) {
		parseSetErrorPos(_parser, parseGetPos(_parser));
		parseSetError(_parser, CC_CODE_IO);

		CC_PRINT("ERROR: can't read file at position '%lu'!\n", _parser->buffer->offset);

		return false;
	}

	_parser->buffer->offset = (size_t) ftell(_parser->buffer->fd);
	if (_parser->buffer->offset < 0) {
		parseSetErrorPos(_parser, parseGetPos(_parser));
		parseSetError(_parser, CC_CODE_IO);

		CC_PRINT("ERROR: can't get position '%lu' in file!\n", _parser->buffer->offset);

		return false;
	}

	return true;

}

bool bufferFill(parser_s *_parser) {

	CC_ASSERT(_parser->buffer->fd != NULL && "FILE NOT OPEN!!");

	if (_parser->buffer->fpos >= _parser->buffer->length) {

		memset(_parser->buffer->data, '\0', CC_BUFFER_SIZE);

		if (fseek(_parser->buffer->fd, (long int) _parser->buffer->offset, SEEK_SET) != 0) {
			parseSetErrorPos(_parser, parseGetPos(_parser));
			parseSetError(_parser, CC_CODE_IO);

			CC_PRINT("ERROR: can't set position '%lu' in file!\n", _parser->buffer->offset);

			return false;
		}

		_parser->buffer->length = fread(_parser->buffer->data, sizeof(char), CC_BUFFER_SIZE,
				_parser->buffer->fd);
		if (_parser->buffer->length == 0) {
			parseSetErrorPos(_parser, parseGetPos(_parser));
			parseSetError(_parser, CC_CODE_IO);

			return false;
		}

		_parser->buffer->offset = (size_t) ftell(_parser->buffer->fd);
		if (_parser->buffer->offset < 0) {
			parseSetErrorPos(_parser, parseGetPos(_parser));
			parseSetError(_parser, CC_CODE_IO);

			CC_PRINT("ERROR: can't get position '%lu' in file!\n", _parser->buffer->offset);

			return false;
		}

		_parser->buffer->fpos = 0;

		return true;

	}

	return true;
}

bool bufferInit(parser_s *_parser, const char *_path, size_t _buffer_size) {
	FILE *fd = fopen(_path, "r");
	if (fd == NULL) {
		return false;
	}

	buffer_s *buffer = (buffer_s*) CONFIG_CC_MALLOC(sizeof(buffer_s));
	if (buffer == NULL) {
		parseSetError(_parser, CC_CODE_NOT_MEM);
		CC_PRINT("ERROR: not enough memory for 'buffer'! %lu bytes needed.\n", sizeof(buffer_s));
		fclose(fd);

		_parser->buffer->fd = NULL;

		return false;
	}
	memset(buffer, 0, sizeof(buffer_s));

	char *data = (char*) CONFIG_CC_MALLOC(_buffer_size * sizeof(char));
	if (data == NULL) {
		parseSetError(_parser, CC_CODE_NOT_MEM);
		CC_PRINT("ERROR: not enough memory for 'buffer::data'! %lu bytes needed.\n",
				_buffer_size * sizeof(char));
		CONFIG_CC_FREE(buffer);
		fclose(fd);

		_parser->buffer = NULL;
		_parser->buffer->fd = NULL;

		return false;
	}

	memset(buffer, 0, _buffer_size * sizeof(char));

	buffer->fpos = 0;
	buffer->length = 0;
	buffer->offset = 0;
	buffer->fd = fd;
	buffer->data = data;

	_parser->buffer = buffer;

	if (!bufferFill(_parser)) {
		fclose(fd);

		CONFIG_CC_FREE(_parser->buffer->data);

		_parser->buffer->fd = NULL;
		_parser->buffer->data = NULL;
		_parser->buffer->fd = NULL;
		_parser->buffer = NULL;

		CONFIG_CC_FREE(buffer);

		return false;
	}

	fseek(fd, 0L, SEEK_END);
	buffer->fsize = ftell(fd);
	fseek(fd, 0L, SEEK_SET);

	return true;

}

void bufferClose(parser_s *_parser) {

	CC_ASSERT(_parser != NULL && "PARSER IS NULL!");

	if (_parser->buffer == NULL) {
		return;
	}

	if (_parser->buffer->fd != NULL) {
		fclose(_parser->buffer->fd);
		_parser->buffer->fd = NULL;
	}

	if (_parser->buffer->data != NULL) {
		CONFIG_CC_FREE(_parser->buffer->data);
		_parser->buffer->data = NULL;
	}

	CONFIG_CC_FREE(_parser->buffer);
	_parser->buffer = NULL;
}

bool bufferGet(parser_s *_parser, char *_ch) {

	if (!bufferFill(_parser)) {
		_ch[0] = '\0';
		return false;
	}

	_ch[0] = _parser->buffer->data[_parser->buffer->fpos];

	return true;
}

bool bufferNext(parser_s *_parser) {

	if (!bufferFill(_parser)) {
		return false;
	}

	_parser->buffer->fpos++;

	return true;

}

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
