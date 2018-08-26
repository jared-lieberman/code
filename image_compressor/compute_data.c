/*
 * Name: compute_data.c
 *
 * By: Jared Lieberman and Eric Magalhaes
 *
 * Logins: jliebe02 | emagal02
 *
 * Date: 10/23/16
 *
 * Purpose: This module is responsible for converting between compute_data 
 *       and component video.
 */

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "component_video.h"
#include "compute_data.h"
#include "uarray.h"

/*
 * Name: retrieve_data
 *
 * Purpose: This function will retrieve the data from the Component_video
 *      struct and initialize them inside a UArray_T. If the function receives
 *      the fourth and final pixel, it takes the average of the pb and pr 
 *      variables.
 */
UArray_T retrieve_data(int pixel_num, UArray_T compute_data, Component_video 
        video_data)
{
        *(float *) UArray_at(compute_data, pixel_num - 1) = video_data -> y;

        *(float *) UArray_at(compute_data, 4) += video_data -> pb;

        *(float *) UArray_at(compute_data, 5) += video_data -> pr;

        if (pixel_num == 4) {
                compute_data = average_chroma(compute_data);
        }

        free(video_data);

        return compute_data;
}

/*
 * Name: average_chroma
 *
 * Purpose: This function will take the average of the four Pb and Pr values. 
 */
UArray_T average_chroma(UArray_T compute_data)
{
        *((float *) UArray_at(compute_data, 4)) /= 4.0;
        *((float *) UArray_at(compute_data, 5)) /= 4.0;

        float lower_bound = -0.5;
        float upper_bound = 0.5;

        if (*((float *) UArray_at(compute_data, 4)) < lower_bound) {
                *((float *) UArray_at(compute_data, 4)) = lower_bound;
        }
        if (*((float *) UArray_at(compute_data, 4)) > upper_bound) {
                *((float *) UArray_at(compute_data, 4)) = upper_bound;
        }

        return compute_data;    
}

/*
 * Name: convert_to_component_video_d
 *
 * Purpose: This function will convert specific data from a UArray_T to
 *      Component video colorspace.
 */
Component_video to_cv_d(int pixel_num, UArray_T 
        compute_data)
{
        Component_video video_data = malloc(sizeof(*video_data));
        assert(video_data != NULL);

        video_data -> y  = *(float *) UArray_at(compute_data, pixel_num - 1);
        video_data -> pb = *(float *) UArray_at(compute_data, 4);
        video_data -> pr = *(float *) UArray_at(compute_data, 5);

        return video_data; 
}

