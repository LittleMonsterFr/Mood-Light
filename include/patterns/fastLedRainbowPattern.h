#ifndef MOOD_LIGHT_FAST_LED_RAINBOW_PATTERN_H
#define MOOD_LIGHT_FAST_LED_RAINBOW_PATTERN_H

#include "pattern.h"

class FastLedRainbowPattern : public Pattern
{
public:
    FastLedRainbowPattern(uint8_t id, uint32_t sleep, float intensity, void *data)
            : Pattern(id, sleep, intensity, data) {};

    void process(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w) override;

    bool updateData(void *data, size_t dataLength) override;

private:
    uint8_t saturation_ = 255;
    uint8_t value_ = 255;

    uint8_t index_ = 0;
    uint8_t maxIndex_ = 255;
    uint8_t ledIndex_ = 0;
    PatternType patternType = PATTERN_TYPE_UNIFORM;
};

#endif /* !MOOD_LIGHT_FAST_LED_RAINBOW_PATTERN_H */
