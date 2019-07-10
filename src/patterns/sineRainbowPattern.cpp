#include <iostream>

#include "helpers.h"
#include "myLogger.h"
#include "patterns/sineRainbowPattern.h"

void SineRainbowPattern::process(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w)
{

    if ((patternType_ == PATTERN_TYPE_UNIFORM && ledIndex_ == numLed_) ||
        patternType_ == PATTERN_TYPE_CIRCULAR)
    {
        float rf = sine_array_[(index_ + offset_ / 2 + ledIndex_ * maxIndex_ / numLed_) % maxIndex_];
        float gf = sine_array_[(index_ + ledIndex_ * maxIndex_ / numLed_) % maxIndex_];
        float bf = sine_array_[(index_ + offset_ + ledIndex_ * maxIndex_ / numLed_) % maxIndex_];

        *r = clamp_255(rf * 127.5f);
        *g = clamp_255(gf * 127.5f);
        *b = clamp_255(bf * 127.5f);
    }

    ledIndex_++;

    if ((patternType_ == PATTERN_TYPE_UNIFORM && ledIndex_ > numLed_) ||
        (patternType_ == PATTERN_TYPE_CIRCULAR && ledIndex_ >= numLed_))
    {
        ledIndex_ = 0;
        index_++;

        if (index_ >= maxIndex_)
        {
            index_ = 0;
        }
    }
}

bool SineRainbowPattern::updateData(void *data, size_t dataLength)
{
    if (dataLength != 1)
    {
        MY_LOGGER_LOG("Sine Rainbow pattern : incorrect data length. Skipping update.\n\n")
        return false;
    }
    auto *tmp_data = (uint8_t *) data;

    patternType_ = (PatternType) tmp_data[0];

    return true;
}
