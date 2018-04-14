/*
 * Name: compute_data.h
 *
 * By: Jared Lieberman and Eric Magalhaes
 *
 * Logins: jliebe02 | emagal02
 *
 * Date: 10/21/16
 *
 * Purpose: This header file holds the interface for compute_data 
 */
#ifndef COMPUTE_DATA
#define COMPUTE_DATA

#include <stdio.h>
#include <stdlib.h>
#include "component_video.h"
#include "uarray.h"

UArray_T retrieve_data(int pixel_num, UArray_T compute_data, Component_video 
        video_data);

UArray_T average_chroma(UArray_T compute_data);

Component_video to_cv_d(int pixel_num, UArray_T 
        compute_data);

#endif