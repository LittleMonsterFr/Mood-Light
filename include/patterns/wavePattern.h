#ifndef MOOD_LIGHT_WAVE_PATTERN_H
#define MOOD_LIGHT_WAVE_PATTERN_H

#include <random>
#include "pattern.h"

class WavePattern : public Pattern
{
public:
    WavePattern(uint8_t id, uint32_t sleep, float intensity, void *data)
            : Pattern(id, sleep, intensity, data)
    {
        auto seed = (unsigned int) time(nullptr);
        gen_ = std::mt19937(seed);
        dis_ = std::uniform_int_distribution<uint8_t>(0, UINT8_MAX);
    };

    void process(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w) override;

    bool updateData(void *data,  size_t dataLength) override;

private:
    uint8_t ledIndex_ = 0;

    uint8_t targetRed_ = 255;
    uint8_t targetGreen_ = 100;
    uint8_t targetBlue_ = 0;
    uint8_t random_ = 0;

    float subIntensity_ = 0.2f;
    float increment_ = 0.01f;
    float minIntensity_ = 0.02f;
    float maxIntensity_ = 1.0f;

    std::mt19937 gen_;
    std::uniform_int_distribution<uint8_t> dis_;
};

#endif /* !MOOD_LIGHT_WAVE_PATTERN_H */
