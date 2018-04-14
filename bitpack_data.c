/*
 * Name: bitpack_data.c
 *
 * By: Jared Lieberman and Eric Magalhaes
 *
 * Logins: jliebe02 | emagal02
 *
 * Date: 10/21/16
 *
 * Purpose: This module is responsible for preparing given values for 
 *       bitpacking. This involves discrete cosine transforms, 
 *       quantization, and scaling of integers in order to ensure that
 *       the right number of bits per variable will be packed into
 *       our 32-bit word.
 */


#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "bitpack_data.h"
#include "compute_data.h"
#include "arith40.h"
#include <math.h>

/*
 * Name: bitpack_data_compress
 *
 * Purpose: This function will employ other functions within this module to
 *      convert PB and PR into four-bit values, and then perform a discrete
 *      cosine transform on the four values of luma in preparation for 
 *      bitpack.
 */
UArray_T bitpack_data_c(UArray_T compute_data)
{
        int array_length = 6;
        UArray_T bitpack_data = UArray_new(array_length, sizeof(int));
        assert(bitpack_data != NULL);

        *(unsigned *) UArray_at(bitpack_data, 4) = 
                Arith40_index_of_chroma(*(float *) UArray_at(compute_data, 4));
                
        
        *(unsigned *) UArray_at(bitpack_data, 5) = 
                Arith40_index_of_chroma(*(float *) UArray_at(compute_data, 5));


        bitpack_data = dc_transform(compute_data, bitpack_data);

        return bitpack_data;
}

/*
 * Name: dc_transform
 *
 * Purpose: This function will perform a discrete cosine transform on four
 *      values of luma, creating values of a (9 bits), b, c, and d (5 bits)
 */
UArray_T dc_transform(UArray_T compute_data, UArray_T bitpack_data)
{
        float y1 = *(float *) UArray_at(compute_data, 0);
        float y2 = *(float *) UArray_at(compute_data, 1);
        float y3 = *(float *) UArray_at(compute_data, 2);
        float y4 = *(float *) UArray_at(compute_data, 3);


        /* quantize a */
        float temp = (y4 + y3 + y2 + y1)/4.0;
        float a_range = 63.0;
        *(unsigned *) UArray_at(bitpack_data, 0) = 
                (unsigned) round(temp * a_range);

        /* quantize b, c, and d */
        *(int *) UArray_at(bitpack_data, 1) = 
                quantize_coefficients((y4 + y3 - y2 - y1) / 4.0);
        *(int *) UArray_at(bitpack_data, 2) = 
                quantize_coefficients((y4 - y3 + y2 - y1) / 4.0);
        *(int *) UArray_at(bitpack_data, 3) = 
                quantize_coefficients((y4 - y3 - y2 + y1) / 4.0);

        return bitpack_data;
}

/*
 * Name: quantize_coefficients
 *
 * Purpose: This function will guarentee that our luminance values of b, c
 *      and d are within a suitable range (-15 - 15).
 */
int quantize_coefficients(float val)
{
        float lower_bound = -0.3;
        float upper_bound = 0.3;
        /* check within range, if not then set as extreme */
        if (val > upper_bound) {
                val = upper_bound;
        } 

        if (val < lower_bound){
                val = lower_bound;
        }

        /* multiply by 50 in order to get in correct range */
        float bcd_range = 103.3;
        return (int) round(val * bcd_range);
}

/*
 * Name: bitpack_data_d
 *
 * Purpose: This function will convert the four bit codes of PB and PR, as
 *      well as perform a discrete cosine transform on the values of a,
 *      b, c, and d in order to obtain values of luma..
 */
UArray_T bitpack_data_d(UArray_T compute_data, UArray_T bitpack_data)
{
        *((float *) UArray_at(compute_data, 4)) = 
                Arith40_chroma_of_index(*((unsigned *)UArray_at(bitpack_data, 
                        4)));

        *((float *) UArray_at(compute_data, 5)) = 
                Arith40_chroma_of_index(*((unsigned *)UArray_at(bitpack_data,
                        5)));

        compute_data = inverse_dc_transform(compute_data, bitpack_data);

        return compute_data;
}

/*
 * Name: dequantize_coefficients
 *
 * Purpose: This function will scale inputed values of b, c, and d in order
 *      to guarentee that they are within the right range..
 */
float dequantize_coefficients(int value)
{
        float scaled_val;

        scaled_val = (float) value;

        float lower_bound = -31.0;
        float upper_bound = 31.0;

        if (scaled_val > upper_bound) {
                scaled_val = upper_bound;
        }       

        if (scaled_val < lower_bound){
                scaled_val = lower_bound;
        }

        float bcd_range = 103.3;
        scaled_val /= bcd_range;

        return scaled_val;
}

/*
 * Name: inverse_dc_transform
 *
 * Purpose: This function will perform an inverse discrete cosine transform
 *      on the values of a, b, c, and d in order to obtain luminance values
 *      Y1-Y4.
 */
UArray_T inverse_dc_transform(UArray_T compute_data, UArray_T bitpack_data)
{
        unsigned a_u = *((unsigned *) UArray_at(bitpack_data, 0));
        float a = (float) a_u / 63.0;
        float b = dequantize_coefficients(*(int *) UArray_at(bitpack_data, 1));
        float c = dequantize_coefficients(*(int *) UArray_at(bitpack_data, 2));
        float d = dequantize_coefficients(*(int *) UArray_at(bitpack_data, 3));

        *(float *) UArray_at(compute_data, 0) = a - b - c + d;
        *(float *) UArray_at(compute_data, 1) = a - b + c - d;
        *(float *) UArray_at(compute_data, 2) = a + b - c - d;
        *(float *) UArray_at(compute_data, 3) = a + b + c + d;

        return compute_data;
}

/*
 * Name: in_bounds
 *
 * Purpose: This function ensures that the inputted values of b, c, and d are
 *      within the bounds of .5 and -.5, forcing them into those bounds if 
 *      necessary.
 */
float in_bounds(float value)
{
        float lower_bound = -0.5;
        float upper_bound = 0.5;
        
        if (value < lower_bound) {
                value = lower_bound;
        }
        if (value > upper_bound) {
                value = upper_bound;
        }
        return value;

}

/*
 * Name: pixel_num
 *
 * Purpose: This function will compute the specific "index" of a pixel within
 *      a block. The pixels are indexed in row-major order, with the top-left
 *      pixel as the 1st pixel and the bottom-right pixel and the 4th pixel.
 */
int pixel_num(int col, int row)
{
        if ((col + 1) % 2 == 1 && (row + 1) % 2 == 1) {
                return 1;
        }
        if ((col + 1) % 2 == 0 && (row + 1) % 2 == 1) {
                return 2;
        }
        if ((col + 1) % 2 == 1 && (row + 1) % 2 == 0) {
                return 3;
        }
        return 4;
}