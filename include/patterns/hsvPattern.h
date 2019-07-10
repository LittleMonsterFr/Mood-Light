#ifndef MOOD_LIGHT_HSV_PATTERN_H
#define MOOD_LIGHT_HSV_PATTERN_H

#include "pattern.h"

class HsvPattern : public Pattern
{
public:
    HsvPattern(uint8_t id, uint32_t sleep, float intensity, void *data)
            : Pattern(id, sleep, intensity, data) {};

    void process(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w) override;

    bool updateData(void *data, size_t dataLength) override;

private:
    float saturation_ = 1.0f;
    float value_ = 1.0f;
    float c_ = saturation_ * value_;
    float m_ = value_ - c_;

    uint16_t index_ = 0;
    uint16_t maxIndex_ = 360 * 4;
    uint8_t ledIndex_ = 0;
    PatternType patternType = PATTERN_TYPE_UNIFORM;
};

#endif /* !MOOD_LIGHT_HSV_PATTERN_H */
