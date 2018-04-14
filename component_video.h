/*
 * Name: component_video.h
 *
 * By: Jared Lieberman and Eric Magalhaes
 *
 * Logins: jliebe02 | emagal02
 *
 * Date: 10/21/16
 *
 * Purpose: This header file holds the interface for component_video
 */
#ifndef COMPONENT_VIDEO
#define COMPONENT_VIDEO

#include <stdio.h>
#include <stdlib.h>
#include "rgb_float.h"

typedef struct Component_video {
        float y;
        float pb;
        float pr;
} *Component_video;

Component_video convert_to_component_video(RGB_float pixel_float);

RGB_float convert_to_rgb(Component_video video_data);

#endif