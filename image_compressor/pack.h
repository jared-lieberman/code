/*
 * Name: pack.h
 *
 * By: Jared Lieberman and Eric Magalhaes
 *
 * Logins: jliebe02 | emagal02
 *
 * Date: 10/23/16
 *
 * Purpose: This header file holds the interface for pack.h
 */

#ifndef PACK_INCLUDED
#define PACK_INCLUDED

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "except.h"
#include "bitpack.h"
#include "uarray.h"

extern void compress_pack(UArray_T bitpack_data);

extern void print_word(uint64_t word);

extern UArray_T decompress_pack(FILE* input);

extern uint64_t decompress_read_word(FILE * input);

extern UArray_T unpack(UArray_T bitpack_data, uint64_t word);

extern Except_T Bitpack_Overflow;

#endif
