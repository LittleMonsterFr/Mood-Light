#ifndef MOOD_LIGHT_DECODE_FAILED_PATTERN_H
#define MOOD_LIGHT_DECODE_FAILED_PATTERN_H

#include "pattern.h"

class DecodeFailedPattern : public Pattern
{
public:
    explicit DecodeFailedPattern(int8_t id, uint32_t sleep, float intensity, void *data)
            : Pattern(id, sleep, intensity, data)
    {};

    void process(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w) override;

    bool updateData(void *data,  size_t dataLength) override;

private:
    uint8_t ledIndex_ = 0;
    bool on_ = true;
};

#endif /* !MOOD_LIGHT_DECODE_FAILED_PATTERN_H */
