#include "helpers.h"
#include "patterns/errorPattern.h"

void ErrorPattern::process(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w)
{
    float subIntensity = 1.0f / numLed_ * (ledIndex_ + 1);

    if (ledIndex_ <= ledMod_)
    {

        *r = clamp_255(255 * subIntensity);
        *g = 0;
        *b = 0;
    }
    else
    {
        *r = 0;
        *g = 0;
        *b = 0;
    }

    ledIndex_++;

    if (ledIndex_ >= numLed_)
    {
        ledIndex_ = 0;
        ledMod_++;
        sleep_ -= 1000000U / numLed_;

        if (ledMod_ >= numLed_)
        {
            ledMod_ = 0;
            sleep_ = 1000000;
        }
    }
}

bool ErrorPattern::updateData(void *data, size_t dataLength)
{
    return true;
}
