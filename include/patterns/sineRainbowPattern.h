#ifndef MOOD_LIGHT_SINE_RAINBOW_PATTERN_H
#define MOOD_LIGHT_SINE_RAINBOW_PATTERN_H

#include <cmath>
#include <cstdlib>

#include "pattern.h"

class SineRainbowPattern : public Pattern
{
public:
    SineRainbowPattern(uint8_t id, uint32_t sleep, float intensity, void *data)
            : Pattern(id, sleep, intensity, data)
    {
        sine_array_ = (float *) calloc(maxIndex_, sizeof(float));
        for (uint16_t i = 0; i < offset_; i++)
        {
            sine_array_[i] = 1.0f + sinf(i * 2 * (float) M_PI / offset_ - (float) M_PI_2);
        }
    };

    ~SineRainbowPattern() override
    {
        free(sine_array_);
    }

    void process(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w) override;

    bool updateData(void *data, size_t dataLength) override;

private:
    uint16_t index_ = 0;
    uint16_t maxIndex_ = 360 * 3;
    uint8_t ledIndex_ = 0;
    uint16_t offset_ = (uint16_t) (2U * maxIndex_ / 3U);
    PatternType patternType_ = PATTERN_TYPE_CIRCULAR;
    float *sine_array_ = nullptr;
};

#endif /* !MOOD_LIGHT_SINE_RAINBOW_PATTERN_H */
