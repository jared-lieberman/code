/*
 * Name: pack.c
 *
 * By: Eric Magalhaes and Jared Lieberman
 *
 * Logins: e14magal | jliebs02
 *
 * Date: 10/21/16
 *
 * Purpose: This source file contains functions that will be   
 * employed to pack and unpack source images.  
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "except.h"
#include "bitpack.h"
#include "uarray.h"
#include "pack.h"
#include "assert.h"

/*
 * Name: compress_pack
 *
 * Purpose: This function will take in data to be packed into a 32-bit
 * word and calls on functions within this bitpack.c to excecute
 * the compression.
 */
void compress_pack(UArray_T bitpack_data)
{
        uint64_t word  = 0;
        unsigned width = 6;
        unsigned lsb   = 26;
        uint64_t value = *(unsigned *) UArray_at(bitpack_data, 0);

        word = Bitpack_newu(word, width, lsb, value);
        //width = 5;
        int64_t value_s;

        for (int i = 0; i < 3; i++) {
                lsb   -= width;
                value_s = *(signed *) UArray_at(bitpack_data, i + 1);
                word  = Bitpack_news(word, width, lsb, value_s);
        }

        width = 4;
        for (int i = 0; i < 2; i++) {
                lsb   -= width;
                value = *(unsigned *) UArray_at(bitpack_data, i + 4);
                word  = Bitpack_newu(word, width, lsb, value);
        }

        print_word(word);
        
}

/*
 * Name: print_word
 *
 * Purpose: This function is used to print out the words in a readable,
 * binary format.
 */
void print_word(uint64_t word)
{
        unsigned width = 8;

        for (int i = 3; i >= 0; i--) {
                putchar((char) Bitpack_getu(word, width, width * i));
        }
}

/*
 * Name: decompress_pack
 *
 * Purpose: This function calls on functions within this module that
 *      will excecute an extraction of data from a 32-bit word.
 */
UArray_T decompress_pack(FILE* input)
{
        uint64_t word = decompress_read_word(input);

        int array_length = 6;
        UArray_T bitpack_data = UArray_new(array_length, sizeof(int));

        bitpack_data = unpack(bitpack_data, word);

        return bitpack_data;

}

/*
 * Name: decompress_read_word
 *
 * Purpose: This function reads a file in binary format in order to
 * supply the program with a 32-bit word, ready for extraction.
 */
uint64_t decompress_read_word(FILE * input)
{
        uint64_t word = 0;

        for (int i = 3; i >= 0; i--) {
                int c = getc(input);

                word = Bitpack_newu(word, 8, 8 * i, (uint64_t) c);
        }
        return word;

}

/*
 * Name: unpack
 *
 * Purpose: This function employs functions within bitpack.c in order to
 * execute the extraction of a, b, c, d, PB, and PR values.
 */
UArray_T unpack(UArray_T bitpack_data, uint64_t word)
{
        *(unsigned *) UArray_at(bitpack_data, 0) = 
                (unsigned) Bitpack_getu(word, 6, 26);

        *(int *) UArray_at(bitpack_data, 1)      = 
                (int) Bitpack_gets(word, 6, 20);

        *(int *) UArray_at(bitpack_data, 2)      = 
                (int) Bitpack_gets(word, 6, 14);

        *(int *) UArray_at(bitpack_data, 3)      = 
                (int) Bitpack_gets(word, 6, 8); 

        *(unsigned *) UArray_at(bitpack_data, 4) = 
                (unsigned) Bitpack_getu(word, 4, 4);
                
        *(unsigned *) UArray_at(bitpack_data, 5) = 
                (unsigned) Bitpack_getu(word, 4, 0);    

        return bitpack_data;
}



