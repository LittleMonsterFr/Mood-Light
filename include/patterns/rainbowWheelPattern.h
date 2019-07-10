#ifndef MOOD_LIGHT_RAINBOW_WHEEL_PATTERN_H
#define MOOD_LIGHT_RAINBOW_WHEEL_PATTERN_H

#include "pattern.h"

class RainbowWheelPattern : public Pattern
{
public:
    RainbowWheelPattern(uint8_t id, uint32_t sleep, float intensity, void *data)
            : Pattern(id, sleep, intensity, data) {};

    void process(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w) override;

    bool updateData(void *data, size_t dataLength) override;

private:
    uint16_t index_ = 0;
    // Always needs to be a multiple of 360
    uint16_t maxIndex_ = 360 * 3;
    uint8_t ledIndex_ = 0;
    PatternType patternType = PATTERN_TYPE_CIRCULAR;
};

#endif /* !MOOD_LIGHT_RAINBOW_WHEEL_PATTERN_H */
