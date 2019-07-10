#include <iostream>

#include "helpers.h"
#include "myLogger.h"
#include "patterns/wavePattern.h"

void WavePattern::process(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w)
{
    /*
     * We process colors only if we've passed all the leds to ensure there are
     * all at the same color at the same time.
     */
    if (ledIndex_ == numLed_)
    {
        *r = clamp_255(targetRed_ * subIntensity_);
        *g = clamp_255(targetGreen_ * subIntensity_);
        *b = clamp_255(targetBlue_ * subIntensity_);

        ledIndex_ = 0;

        subIntensity_ += increment_;

        if (subIntensity_ > maxIntensity_ || subIntensity_ < minIntensity_)
        {
            increment_ *= -1.0f;

            if (random_ == 1 && increment_ >= 0)
            {
                targetRed_ = dis_(gen_);
                targetGreen_ = dis_(gen_);
                targetBlue_ = dis_(gen_);
            }
        }

    }

    ledIndex_++;
}

bool WavePattern::updateData(void *data, size_t dataLength)
{
    if (dataLength != 4)
    {
        MY_LOGGER_LOG("Wave pattern : incorrect data length. Skipping update.\n\n")
        return false;
    }

    auto *tmp_data = (uint8_t *) data;

    random_ = tmp_data[0];
    targetRed_ = tmp_data[1];
    targetGreen_ = tmp_data[2];
    targetBlue_ = tmp_data[3];


    return true;
}
