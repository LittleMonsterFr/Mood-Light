#include <iostream>

#include "helpers.h"
#include "myLogger.h"
#include "patterns/duetPattern.h"

void DuetPattern::process(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w)
{
    if (ledIndex_ == numLed_)
    {
        hue_ = (ledIndex_ * maxIndex_ / numLed_ + index_) % maxIndex_;
        float denom = maxIndex_ / 6.0f;
        float h_p = hue_ / denom;
        float x = c_ * (1.0f - fabsf(fmodf(h_p, 2.0f) - 1.0f));

        float r1 = 0;
        float b1 = 0;
        float g1 = 0;

        if (hue_ <= denom)
        {
            r1 = c_;
            g1 = x;
        }
        else if (denom <= hue_ && hue_ <= 2 * denom)
        {
            r1 = x;
            g1 = c_;
        }
        else if (2 * denom <= hue_ && hue_ <= 3 * denom)
        {
            g1 = c_;
            b1 = x;
        }
        else if (3 * denom <= hue_ && hue_ <= 4 * denom)
        {
            g1 = x;
            b1 = c_;
        }
        else if (4 * denom <= hue_ && hue_ <= 5 * denom)
        {
            r1 = x;
            b1 = c_;
        }
        else if (5 * denom <= hue_ && h_p < 6 * denom)
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

    if (ledIndex_ > numLed_)
    {
        if (hue_ == hue1_ || hue_ == hue2_)
        {
            if (lastColor_ != NONE)
                direction_ *= -1;

            if (hue_ == hue1_ && (lastColor_ == NONE || lastColor_ == COLOR2))
                lastColor_ = COLOR1;

            if (hue_ == hue2_ && lastColor_ == COLOR1)
                lastColor_ = COLOR2;
        }

        ledIndex_ = 0;
        index_ += direction_;
    }
}

bool DuetPattern::updateData(void *data, size_t dataLength)
{
    if (dataLength != 6)
    {
        MY_LOGGER_LOG("Duet pattern : Incorrect data length. Skipping update.\n\n")
        return false;
    }

    auto *tmp_data = (uint8_t *) data;

    targetR1_ = tmp_data[0];
    targetG1_ = tmp_data[1];
    targetB1_ = tmp_data[2];
    targetR2_ = tmp_data[3];
    targetG2_ = tmp_data[4];
    targetB2_ = tmp_data[5];

    MY_LOGGER_LOG("Duet pattern : target values : R1=%d, G1=%d, B1=%d, R2=%d, G2=%d, B2=%d.\n\n",
                  targetR1_, targetG1_, targetB1_, targetR2_, targetG2_, targetB2_);

    lastColor_ = NONE;
    hue_ = 0;
    index_ = 0;
    ledIndex_ = 0;
    direction_ = 1;

    while(lastColor_ != COLOR2)
    {
        float rp;
        float gp;
        float bp;

        if (lastColor_ == NONE)
        {
            rp = (float) targetR1_ / 255.0f;
            gp = (float) targetG1_ / 255.0f;
            bp = (float) targetB1_ / 255.0f;
        }
        else
        {
            rp = (float) targetR2_ / 255.0f;
            gp = (float) targetG2_ / 255.0f;
            bp = (float) targetB2_ / 255.0f;
        }

        float cmax = rp > gp ? rp > bp ? rp : bp : gp > bp ? gp : bp;
        float cmin = rp < gp ? rp < bp ? rp : bp : gp < bp ? gp : bp;

        float delta = cmax - cmin;
        float hp;

        if (cmax == rp)
        {
            float q = (gp - bp) / delta;
            while(q < 0)
                q += 6.0f;
            hp = fmodf(q, 6.0f);
        }
        else if (cmax == gp)
        {
            hp = (bp - rp) / delta + 2;
        }
        else if (cmax == bp)
        {
            hp = (rp -gp) / delta + 4;
        }
        else
        {
            MY_LOGGER_LOG("Duet pattern : unknown chroma\n\n");
            return false;
        }

        float h = 60.0f * hp;

        if (lastColor_ == NONE)
        {
            hue1_ = (uint16_t) ((uint16_t)roundf(h) % 360);
            lastColor_ = COLOR1;
        }
        else
        {
            hue2_ = (uint16_t) ((uint16_t)roundf(h) % 360);
            lastColor_ = COLOR2;
        }
    }

    lastColor_ = NONE;

    MY_LOGGER_LOG("Duet pattern : HUE1 = %d and HUE2 = %d\n\n", hue1_, hue2_);

    return true;
}
