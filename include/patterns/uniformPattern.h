#ifndef MOOD_LIGHT_UNIFORM_PATTERN_H
#define MOOD_LIGHT_UNIFORM_PATTERN_H

#include "pattern.h"

class UniformPattern : public Pattern
{
public:
    UniformPattern(uint8_t id, uint32_t sleep, float intensity, void *data)
            : Pattern(id, sleep, intensity, data)
    {};

    void process(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w) override;

    bool updateData(void *data,  size_t dataLength) override;

private:
    uint8_t targetRed_ = 255;
    uint8_t targetGreen_ = 100;
    uint8_t targetBlue_ = 0;
};

#endif /* !MOOD_LIGHT_UNIFORM_PATTERN_H */
