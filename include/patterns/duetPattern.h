#ifndef MOOD_LIGHT_DUET_PATTERN_H
#define MOOD_LIGHT_DUET_PATTERN_H

#include <cmath>
#include <cstdlib>
#include <cfloat>

#include "pattern.h"

typedef enum {
    NONE,
    COLOR1,
    COLOR2
} color;

class DuetPattern : public Pattern
{
public:
    DuetPattern(uint8_t id, uint32_t sleep, float intensity, void *data)
            : Pattern(id, sleep, intensity, data)
    {};

    void process(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w) override;

    bool updateData(void *data, size_t dataLength) override;

private:
    float saturation_ = 1.0f;
    float value_ = 1.0f;
    float c_ = saturation_ * value_;
    float m_ = value_ - c_;
    uint16_t hue_ = 0;

    uint16_t index_ = 0;
    uint16_t maxIndex_ = 360;
    uint8_t ledIndex_ = 0;

    color lastColor_ = NONE;
    uint8_t targetR1_ = 0;
    uint8_t targetG1_ = 0;
    uint8_t targetB1_ = 0;
    uint8_t targetR2_ = 0;
    uint8_t targetG2_ = 0;
    uint8_t targetB2_ = 0;
    int8_t direction_ = 1;
    uint16_t hue1_ = 0;
    uint16_t hue2_ = 0;
};

#endif /* !MOOD_LIGHT_DUET_PATTERN_H */
