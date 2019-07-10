#include "helpers.h"
#include "patterns/decodeFailedPattern.h"

void DecodeFailedPattern::process(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w)
{
    if (on_)
        *w = 255;
    else
        *w = 0;

    *r = 0;
    *g = 0;
    *b = 0;

    ledIndex_++;

    if (ledIndex_ >= numLed_)
    {
        ledIndex_ = 0;

        on_ = !on_;
    }
}

bool DecodeFailedPattern::updateData(void *data,  size_t dataLength)
{
    return true;
}

