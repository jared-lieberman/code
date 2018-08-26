/*
 * Name: bitpack_data.h
 *
 * By: Jared Lieberman and Eric Magalhaes
 *
 * Logins: jliebe02 | emagal02
 *
 * Date: 10/21/16
 *
 * Purpose: This header file holds the interface for bitpack_data
 */

#ifndef BITPACK_DATA
#define BITPACK_DATA

#include <stdio.h>
#include <stdlib.h>
#include "uarray.h"

UArray_T bitpack_data_c(UArray_T compute_data);

UArray_T dc_transform(UArray_T compute_data, UArray_T bitpack_data);

int quantize_coefficients(float val);

UArray_T bitpack_data_d(UArray_T compute_data, UArray_T bitpack_data);

float dequantize_coefficients(int value);

UArray_T inverse_dc_transform(UArray_T compute_data, UArray_T bitpack_data);

float in_bounds(float value);

int pixel_num(int col, int row);

#endif
