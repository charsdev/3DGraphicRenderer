#ifndef CAMERA_H
#define CAMERA_H

#include "Vector.h"

typedef struct {
    vec3_t position;
    vec3_t rotation;
    float fov_angle;
} camera_t;

extern camera_t camera = {
    {0,0,0},
    {0.3, -2.0, 0},
    0.78
};

#endif // CAMERA_H
