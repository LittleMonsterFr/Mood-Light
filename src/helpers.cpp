#include <cmath>

#include "helpers.h"

uint8_t clamp_255(float val)
{
    if (val > 255.0f)
        return 255;
    else if (val < 0.0f)
        return 0;
    else
        return (uint8_t) roundf(val);
}