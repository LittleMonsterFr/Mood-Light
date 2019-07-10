#include <iostream>
#include <cmath>

#include "helpers.h"
#include "myLogger.h"
#include "patterns/hsvPattern.h"

void HsvPattern::process(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w)
{
    if ((patternType == PATTERN_TYPE_UNIFORM && ledIndex_ == numLed_) ||
        patternType == PATTERN_TYPE_CIRCULAR)
    {
        uint16_t hue = (ledIndex_ * maxIndex_ / numLed_ + index_) % maxIndex_;
        float denom = maxIndex_ / 6.0f;
        float h_p = hue / denom;
        float x = c_ * (1.0f - fabsf(fmodf(h_p, 2.0f) - 1.0f));

        float r1 = 0;
        float b1 = 0;
        float g1 = 0;


        if (hue <= denom)
        {
            r1 = c_;
            g1 = x;
        }
        else if (denom <= hue && hue <= 2 * denom)
        {
            r1 = x;
            g1 = c_;
        }
        else if (2 * denom <= hue && hue <= 3 * denom)
        {
            g1 = c_;
            b1 = x;
        }
        else if (3 * denom <= hue && hue <= 4 * denom)
        {
            g1 = x;
            b1 = c_;
        }
        else if (4 * denom <= hue && hue <= 5 * denom)
        {
            r1 = x;
            b1 = c_;
        }
        else if (5 * denom <= hue && h_p < 6 * denom)
        {
            r1 = c_;
            b1 = x;
        }

        r1 += m_;
        g1 += m_;
        b1 += m_;

        *r = clamp_255(r1 * 255.0f);
        *g = clamp_255(g1 * 255.0f);
        *b = clamp_255(b1 * 255.0f);

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

bool HsvPattern::updateData(void *data, size_t dataLength)
{
    if (dataLength != 3)
    {
        MY_LOGGER_LOG("HSV pattern : incorrect data length. Skipping update.\n\n")
        return false;
    }

    auto *tmp_data = (uint8_t *) data;

    patternType = (PatternType) tmp_data[0];
    saturation_ = tmp_data[1] / 100.0f;
    value_ = tmp_data[2] / 100.0f;

    c_ = saturation_ * value_;
    m_ = value_ - c_;

    MY_LOGGER_LOG("HSV pattern : saturation = %f, value = %f, c = %f, m = %f\n\n",
                  (double) saturation_, (double) value_, (double) c_, (double) m_);

    return true;
}
