/*
 * Name: bitpack_data.h
 *
 * By: Jared Lieberman and Eric Magalhaes
 *
 * Logins: jliebe02 | emagal02
 *
 * Date: 10/21/16
 *
 * Purpose: This module is used to pack data into 32-bit words as well as 
 * extract data from 32-bit words. 
 */
#include <stdio.h>
#include <stdlib.h>
#include "except.h"
#include "bitpack.h"
#include "math.h"
#include "assert.h"


static inline uint64_t shift_left(uint64_t word, unsigned width);
static inline uint64_t shift_right_unsigned(uint64_t word, unsigned value);
static inline uint64_t shift_right_signed(uint64_t word, unsigned value);

Except_T Bitpack_Overflow = { "Overflow packing bits" };

/*
 * Name: compress_pack
 *
 * Purpose: This function will take in data to be packed into a 32-bit
 * word and calls on functions within this bitpack.c to excecute
 * the compression.
 */
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        if (width == 0) {
                return false;
        }

        unsigned total_bits = 64;

        assert(width <= total_bits);

        uint64_t mask = ~0;

        mask = shift_left(mask, total_bits - width);

        mask = shift_right_unsigned(mask, total_bits - width);

        if (n <= mask) {
                return true;
        }
        return false;
}

/*
 * Name: compress_pack
 *
 * Purpose: This function will take in data to be packed into a 32-bit
 * word and calls on functions within this bitpack.c to excecute
 * the compression.
 */
bool Bitpack_fitss(int64_t n, unsigned width)
{
        if (width == 0) {
                return false;
        }

        unsigned total_bits = 64;

        assert(width <= total_bits);

        uint64_t mask_lower = ~0;

        mask_lower = shift_left(mask_lower, width - 1);

        uint64_t mask_upper = ~0;

        mask_upper = shift_right_unsigned(mask_upper, total_bits - width + 1);

        if ((n >= (int64_t) mask_lower) && (n <= (int64_t) mask_upper)) {

                return true;
        }

        return false;
}

/*
 * Name: compress_pack
 *
 * Purpose: This function will take in data to be packed into a 32-bit
 * word and calls on functions within this bitpack.c to excecute
 * the compression.
 */
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        if (width == 0) {
                return 0;
        }

        unsigned total_bits = 64;

        assert(width <= total_bits);
        assert(width + lsb <= total_bits);

        uint64_t mask = ~0;

        mask = shift_left(mask, total_bits - width);

        mask = shift_right_unsigned(mask, (total_bits - width - lsb));

        word = (mask & word);

        word = shift_right_unsigned(word, lsb);

        return word;
}

/*
 * Name: compress_pack
 *
 * Purpose: This function will take in data to be packed into a 32-bit
 * word and calls on functions within this bitpack.c to excecute
 * the compression.
 */
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        if (width == 0) {
                return 0;
        }

        unsigned total_bits = 64;

        assert(width <= total_bits);
        assert(width + lsb <= total_bits);

        uint64_t mask = ~0;

        mask = shift_left(mask, total_bits - width);

        mask = shift_right_unsigned(mask, (total_bits - width - lsb));

        word = (mask & word);

        word = shift_left(word, total_bits - width - lsb);

        int64_t temp_word = (int64_t) word;

        temp_word = shift_right_signed(temp_word, total_bits - width);

        return temp_word;
}


/*
 * Name: shift_right_signed
 *
 * return a new word which is identical to the original word, except that 
 * the field of width width with lsb at lsb will have been replaced by a 
 * width-bit representation of value. 
 */
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, 
        uint64_t value)
{
        if (width == 0) {
                return word;
        }
        unsigned total_bits = 64;
        assert(width <= total_bits);
        assert(width + lsb <= total_bits);

        /* raise exception Bitpack_Overflow if given value that doesn't fit 
        in width unsigned bits */
        TRY             
                if (Bitpack_fitsu(value, width) == false) {
                
                        RAISE (Bitpack_Overflow);
                }
        EXCEPT (Bitpack_Overflow)               
        END_TRY;

        uint64_t mask = ~0;
        mask = shift_left(mask, total_bits - width);
        mask = shift_right_unsigned(mask, (total_bits - width - lsb));
        word = word | mask;
        mask = ~mask;
        mask = mask | (value << lsb);
        word = word & mask;

        return word;
}
/*
 * Name: shift_right_signed
 *
 * Purpose: return a new word which is identical to the original word, except 
 * that the field of width width with lasb at lasb will have been replaced by a 
 * width-bit representation of value.
 */
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, 
        int64_t value)
{
        if (width == 0) {
                return word;
        }
        unsigned total_bits = 64;

        assert(width <= total_bits);
        assert(width + lsb <= total_bits);

        /* raise exception Bitpack_Overflow if given value that doesn't fit 
        in width unsigned bits */
        TRY     
                if (Bitpack_fitsu(value, width) == false) {
                
                        RAISE (Bitpack_Overflow);
                }
        EXCEPT (Bitpack_Overflow)               
        END_TRY;

        uint64_t mask = ~0;
        mask = shift_left(mask, total_bits - width);
        mask = shift_right_unsigned(mask, (total_bits - width - lsb));
        word = word | mask;
        mask = ~mask;
        mask = mask | (value << lsb);
        word = word & mask;

        return word;
}

/*
 * Name: shift_right_signed
 *
 * Purpose: This function is used to perform "value"-wide left shift on a word.
 */
uint64_t shift_left(uint64_t word, unsigned value)
{
        return word << value;

}

/*
 * Name: shift_right_signed
 *
 * Purpose: This function is used to perform an unsigned "value"-wide right 
 * shift on a word. Propagating 0s on the left side.
 */
uint64_t shift_right_unsigned(uint64_t word, unsigned value)
{
        return word >> value;
}

/*
 * Name: shift_right_signed
 *
 * Purpose: This function is used to perform a signed "value"-wide right shift 
 * on a word. Propagating either 0s or 1s depending on the value's leading 
 * bit.
 */
uint64_t shift_right_signed(uint64_t word, unsigned value)
{
        int64_t temp_word = (int64_t) word;

        temp_word >>= value;

        return (uint64_t) temp_word;

}







