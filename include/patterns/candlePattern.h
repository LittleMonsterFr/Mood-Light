#ifndef MOOD_LIGHT_CANDLE_PATTERN_H
#define MOOD_LIGHT_CANDLE_PATTERN_H

#include <random>
#include "pattern.h"

class CandlePattern : public Pattern
{
public:
    CandlePattern(uint8_t id, uint32_t sleep, float intensity, void *data)
            : Pattern(id, sleep, intensity, data)
    {
        std::random_device rd;
        gen_ = std::mt19937(rd());
        random_ = std::uniform_int_distribution<uint8_t>(0, 79);
        sleep_generator_ = std::uniform_int_distribution<uint8_t>(50, 150);
    };

    void process(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w) override;

    bool updateData(void *data, size_t dataLength) override;

private:
    uint8_t ledIndex_ = 0;

    std::mt19937 gen_;
    std::uniform_int_distribution<uint8_t> random_;
    std::uniform_int_distribution<uint8_t> sleep_generator_;
};

#endif /* !MOOD_LIGHT_CANDLE_PATTERN_H */
