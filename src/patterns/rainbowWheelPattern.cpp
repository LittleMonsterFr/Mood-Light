#include <iostream>

#include "helpers.h"
#include "myLogger.h"
#include "patterns/rainbowWheelPattern.h"

void RainbowWheelPattern::process(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w)
{
    if ((patternType == PATTERN_TYPE_UNIFORM && ledIndex_ == numLed_) ||
        patternType == PATTERN_TYPE_CIRCULAR)
    {
        uint16_t val = (ledIndex_ * maxIndex_ / numLed_ + index_) % maxIndex_;

        float r1 = 0.0f;
        float g1 = 0.0f;
        float b1 = 0.0f;

        if (val <= maxIndex_ / 3)
        {
            r1 = maxIndex_ - 3.0f * val;
            g1 = 3.0f * val;
        }
        else if (val <= 2 * maxIndex_ / 3)
        {
            val -= maxIndex_ / 3;
            g1 = maxIndex_ - 3.0f * val;
            b1 = 3.0f * val;
        }
        else
        {
            val -= 2 * maxIndex_ / 3;
            r1 = 3.0f * val;
            b1 = maxIndex_ - 3.0f * val;
        }

        *r = clamp_255(r1 * 255.0f / maxIndex_);
        *g = clamp_255(g1 * 255.0f / maxIndex_);
        *b = clamp_255(b1 * 255.0f / maxIndex_);
    }

    ledIndex_++;

    if ((patternType == PATTERN_TYPE_UNIFORM && ledIndex_ > numLed_) ||
        (patternType == PATTERN_TYPE_CIRCULAR && ledIndex_ >= numLed_))
    {
        ledIndex_ = 0;
        index_++;

        if (index_ >= maxIndex_)
        {
            index_ = 0;
        }
    }
}

bool RainbowWheelPattern::updateData(void *data, size_t dataLength)
{
    if (dataLength != 1)
    {
        MY_LOGGER_LOG("Rainbow Wheel pattern : incorrect data length. Skipping update.\n\n")
        return false;
    }

    auto *tmp_data = (uint8_t *) data;

    patternType = (PatternType) tmp_data[0];

    return true;
}
