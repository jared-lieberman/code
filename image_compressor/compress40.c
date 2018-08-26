/*
 * Name: compress40.c
 *
 * By: Jared Lieberman and Eric Magalhaes
 *
 * Logins: jliebe02 | emagal02
 *
 * Date: 10/23/16
 *
 * Purpose: This source file will execute the general function of compressing
 *      and decompressing an image.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "assert.h"
#include "a2methods.h"
#include "a2blocked.h"
#include "pnm.h"
#include "compress40.h"
#include "uarray.h"
#include "uarray2b.h"
#include "rgb_float.h"
#include "component_video.h"
#include "compute_data.h"
#include "bitpack_data.h"
#include "pnm.h"
#include "pack.h"

static void process_block(int col, int row, Pnm_ppm pixmap);
static void process_block_d(int col, int row, Pnm_ppm pixmap, 
        A2Methods_T methods, FILE *input);
static bool is_even(int num);
static void trim(Pnm_ppm pixmap);

/*
 * Name: compress40
 *
 * Purpose: This function will initialize a Pnm_ppm and write it to a 
 *      compressed image. 
 */
void compress40(FILE *input)
{       
        A2Methods_T methods = uarray2_methods_blocked;
        assert(methods);
        
        Pnm_ppm pixmap = Pnm_ppmread(input, methods);
        assert(pixmap);
        trim(pixmap);

        printf("COMP40 Compressed image format 2\n%u %u", pixmap -> width, 
                pixmap -> height);
        printf("\n");

        for (int r = 0; r < (int) (pixmap -> height); r += 2) {
                for (int c = 0; c < (int) (pixmap -> width); c += 2) {
                        process_block(c, r, pixmap);
                }
        }
        Pnm_ppmfree(&pixmap);
}

/*
 * Name: process_block
 *
 * Purpose: This function will handle an entire block of data to compress.
 */
void process_block(int col, int row, Pnm_ppm pixmap)
{
        UArray_T bitpack_data;
        int array_length = 6;
        UArray_T compute_data = UArray_new(array_length, sizeof(float));
        for (int i = row; i < row + 2; i++) {
                for (int j = col; j < col + 2; j++) {
                        Pnm_rgb p = (Pnm_rgb) UArray2b_at(pixmap->pixels, j, i);

                        RGB_float r = convert_to_float(p, pixmap->denominator);

                        Component_video video_data = 
                                convert_to_component_video(r);

                        compute_data = retrieve_data(pixel_num(j, i), 
                                compute_data, video_data);

                        if (pixel_num(j, i) == 4) {
                                bitpack_data = bitpack_data_c(compute_data);

                                compress_pack(bitpack_data);
                                                
                                UArray_free(&compute_data);
                                UArray_free(&bitpack_data);
                        }
                }
        }
}

/*
 * Name: decompress40
 *
 * Purpose: This function will perform the the general process of reading
 *      a compressed image and writing it to PPM.
 */
void decompress40(FILE *input)
{
        A2Methods_T methods = uarray2_methods_blocked;
        assert(methods);

        unsigned height, width;
        int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u", 
                &width, &height);
        assert(read == 2);
        read = fscanf(input, "\n");

        Pnm_ppm pixmap = malloc(sizeof(*pixmap));
        assert(pixmap);

        pixmap -> width       = width; 
        pixmap -> height      = height;
        pixmap -> denominator = 255;
        pixmap -> pixels      = methods -> new((int) width, (int) height, 
                sizeof(struct Pnm_rgb));
        pixmap -> methods     = methods;

        for (int r = 0; r < (int) (pixmap -> height); r += 2) {
                for (int c = 0; c < (int) (pixmap -> width); c += 2) {
                        process_block_d(c, r, pixmap, methods, input);
                }
        }
        Pnm_ppmwrite(stdout, pixmap);
        Pnm_ppmfree(&pixmap);   
}

/*
 * Name: process_block_d
 *
 * Purpose: This function will handle an entire block of data to decompress.
 */
void process_block_d(int col, int row, Pnm_ppm pixmap, A2Methods_T methods, 
        FILE *input)
{
        UArray_T bitpack_data;
        
        int array_length = 6;
        
        Component_video video_data;

        bitpack_data = decompress_pack(input);
        UArray_T compute_data = UArray_new(array_length, sizeof(float));
        for (int i = row; i < row + 2; i++) {
                for (int j = col; j < col + 2; j++) {
                        compute_data = bitpack_data_d(compute_data, 
                                bitpack_data);                          
                                        
                        video_data = to_cv_d(pixel_num(j, i), compute_data);

                        RGB_float pixel_float = convert_to_rgb(video_data);
                        Pnm_rgb p = convert_to_unsigned(pixel_float);
                        *((Pnm_rgb) methods -> at(pixmap->pixels, j, i)) = *p;
                        free(p);
                }
        }
        UArray_free(&bitpack_data);
        UArray_free(&compute_data);
}

/*
 * Name: trim
 *
 * Purpose: This function will trim the Pnm_ppm's width and height if they
 *      are not even values.
 */
void trim(Pnm_ppm pixmap)
{
        if (is_even(pixmap -> width) == false) {
                pixmap -> width = (pixmap -> width) - 1;
        }
        if (is_even(pixmap -> height) == false) {
                pixmap -> height = (pixmap -> height) - 1;
        }
}

/*
 * Name: is_even
 *
 * Purpose: This function will determine if a number is even or odd.
 */
bool is_even(int num)
{
        if ((num % 2) == 1) {
                return false;
        }
        return true;
}
