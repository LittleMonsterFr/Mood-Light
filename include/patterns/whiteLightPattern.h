#ifndef MOOD_LIGHT_WHITE_LIGHT_PATTERN_H
#define MOOD_LIGHT_WHITE_LIGHT_PATTERN_H

#include "pattern.h"

class WhiteLightPattern : public Pattern
{
public:
    WhiteLightPattern(uint8_t id, uint32_t sleep, float intensity, void *data)
            : Pattern(id, sleep, intensity, data)
    {};

    void process(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w) override;

    bool updateData(void *data,  size_t dataLength) override;
};

#endif /* !MOOD_LIGHT_WHITE_LIGHT_PATTERN_H */
