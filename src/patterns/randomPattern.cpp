#include <iostream>
#include <random>

#include "patterns/randomPattern.h"

void RandomPattern::process(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w)
{
    /*
     * We process colors only if we've passed all the leds to ensure there are
     * all at the same color at the same time.
     */
    if (ledIndex_ == numLed_)
    {
        if (*r != targetRed_)
            *r += *r < targetRed_ ? 1 : -1;

        if (*g != targetGreen_)
            *g += *g < targetGreen_ ? 1 : -1;

        if (*b != targetBlue_)
            *b += *b < targetBlue_ ? 1 : -1;

        if (*r == targetRed_ && *g == targetGreen_&& *b == targetBlue_)
        {
            targetRed_ = dis_(gen_);
            targetGreen_ = dis_(gen_);
            targetBlue_ = dis_(gen_);
        }

        ledIndex_ = 0;
    }

    ledIndex_++;
}

bool RandomPattern::updateData(void *data, size_t dataLength)
{
    return true;
}
