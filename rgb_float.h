/*
 * Name: rgb_float.h
 *
 * By: Jared Lieberman and Eric Magalhaes
 *
 * Logins: jliebe02 | emagal02
 *
 * Date: 10/23/16
 *
 * Purpose: This header file holds the interface for rgb_float
 */
#ifndef RGB_FLOAT
#define RGB_FLOAT

#include <stdio.h>
#include <stdlib.h>
#include "pnm.h"


typedef struct RGB_float {
        float r;
        float g;
        float b;
} *RGB_float; 

RGB_float convert_to_float(Pnm_rgb pixel, unsigned denominator);

RGB_float RGB_float_new(float r, float g, float b);

void initialize_rgb_vals(RGB_float pixel_float, float denominator);

Pnm_rgb convert_to_unsigned(RGB_float pixel_float);

void initialize_rgb_vals_d(RGB_float pixel_float);

float rgb_in_bounds(float color);

#endif