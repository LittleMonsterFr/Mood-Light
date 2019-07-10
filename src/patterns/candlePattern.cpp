#include <iostream>
#include <random>

#include "patterns/candlePattern.h"
#include "helpers.h"
#include "myLogger.h"

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} fire_led;

fire_led fire = { 80, 35, 0};

void CandlePattern::process(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w)
{
    *r = fire.r;
    *g = fire.g;
    *b = fire.b;

    uint8_t rand = random_(gen_);

    *r = clamp_255(*r - rand);
    *g = clamp_255(*g - (float) rand / 2);

    ledIndex_++;

    if (ledIndex_ >= numLed_)
    {
        ledIndex_ = 0;
        sleep_ = sleep_generator_(gen_) * 1000U;
    }
}

bool CandlePattern::updateData(void *data, size_t dataLength)
{
    return false;
}
