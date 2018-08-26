/*
 * Name: rgb_float.c
 *
 * By: Jared Lieberman and Eric Magalhaes
 *
 * Logins: jliebe02 | emagal02
 *
 * Date: 10/23/16
 *
 * Purpose: This source file will execute the process of converting a pixel's
 *      data between unsigned and float values.
 */

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "rgb_float.h"
#include "pnm.h"
#include <math.h>

/*
 * Name: convert_to_float
 *
 * Purpose: This function will convert unsigned values in a Pnm_rgb to 
 *      float values.
 */
RGB_float convert_to_float(Pnm_rgb pixel, unsigned denominator)
{
        RGB_float pixel_float = RGB_float_new((float) (pixel -> red), 
                (float) (pixel -> green), (float) (pixel -> blue));
        
        initialize_rgb_vals(pixel_float, (float) denominator);

        return pixel_float;
}

/*
 * Name: RGB_float_new
 *
 * Purpose: This function will create a new instance of the struct RGB_float.
 */
RGB_float RGB_float_new(float r, float g, float b)
{
        RGB_float pixel_float = malloc(sizeof(*pixel_float));
        assert(pixel_float != NULL);

        pixel_float -> r = r;
        pixel_float -> g = g;
        pixel_float -> b = b;

        return pixel_float;
}

/*
 * Name: initialize_rgb_vals
 *
 * Purpose: This function will compute the correct values of the variables of
 *      the RGB_float struct using the pimap's denominator.
 */
void initialize_rgb_vals(RGB_float pixel_float, float denominator)
{
        pixel_float -> r /= denominator; 
        pixel_float -> g /= denominator; 
        pixel_float -> b /= denominator; 
}

/*
 * Name: convert_to_unsigned
 *
 * Purpose: This function will convert float values in an RGB_float to
 *      unsigned values in a Pnm_rgb.
 */
Pnm_rgb convert_to_unsigned(RGB_float pixel_float)
{
        initialize_rgb_vals_d(pixel_float);

        Pnm_rgb pixel = malloc(sizeof(*pixel));
        assert(pixel != NULL);

        pixel -> red   = (unsigned) round(pixel_float -> r);
        pixel -> green = (unsigned) round(pixel_float -> g);
        pixel -> blue  = (unsigned) round(pixel_float -> b);

        free(pixel_float);
        return pixel;
}

/*
 * Name: initialize_rgb_vals_d
 *
 * Purpose: This function will compute the correct values of the variables of
 *      the RGB_float struct using the pixmap's denominator.
 */
void initialize_rgb_vals_d(RGB_float pixel_float)
{
        float denominator = 255.0;
        pixel_float -> r *= denominator; 

        pixel_float -> g *= denominator; 
        pixel_float -> b *= denominator; 

        pixel_float -> r  = rgb_in_bounds(pixel_float -> r);
        pixel_float -> g  = rgb_in_bounds(pixel_float -> g);
        pixel_float -> b  = rgb_in_bounds(pixel_float -> b);
}

/*
 * Name: rgb_in_bounds
 *
 * Purpose: This function will ensure a float value is within the range
 *      0.0 - 255.0.
 */
float rgb_in_bounds(float color)
{
        float lower_bound = 0.0;
        float upper_bound = 255.0;
        
        if (color < lower_bound) {
                color = lower_bound;
        }
        if (color > upper_bound) {
                color = upper_bound;
        }
        return color;
}

