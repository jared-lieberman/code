/*
 * Name: component_video.c
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
#include "assert.h"
#include "rgb_float.h"
#include "component_video.h"

/*
 * Name: convert_to_component_video
 *
 * Purpose: This function will execute the process of converting an RGB_float
 *      to the Component video colorpsace. This is done using specific 
 *      mathematical conversions.
 */
Component_video convert_to_component_video(RGB_float pixel_float)
{
        Component_video video_data = malloc(sizeof(*video_data));
        assert(video_data != NULL);

        float red   = pixel_float -> r;
        float green = pixel_float -> g;
        float blue  = pixel_float -> b;

        video_data -> y  = (0.299 * red) + (0.587 * green) + (0.114 * blue);
        video_data -> pb = (-0.168736 * red) - (0.331264 * green) + 
                (0.5 * blue);
        video_data -> pr = (0.5 * red) - (0.418688 * green) - 
                (0.081312 * blue);

        free(pixel_float);
        return video_data;
}

/*
 * Name: convert_to_rgb
 *
 * Purpose: This function will convert data from a Component video colorspace
 *      to the RGB colorspace.
 */
RGB_float convert_to_rgb(Component_video video_data)
{
        RGB_float pixel_float = malloc(sizeof(*pixel_float));
        assert(pixel_float != NULL);

        float y  = video_data -> y;
        float pb = video_data -> pb;
        float pr = video_data -> pr; 

        pixel_float -> r = (1.0 * y) + (0.0 * pb)      + (1.402 * pr);
        pixel_float -> g = (1.0 * y) - (0.344136 * pb) - (0.714136 * pr);
        pixel_float -> b = (1.0 * y) + (1.772 * pb)    + (0.0 * pr);

        free(video_data);

        return pixel_float;
}

