#include <iostream>

#include "helpers.h"
#include "myLogger.h"
#include "patterns/uniformPattern.h"

void UniformPattern::process(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w)
{
    *r = targetRed_;
    *g = targetGreen_;
    *b = targetBlue_;
}

bool UniformPattern::updateData(void *data, size_t dataLength)
{
    if (dataLength != 3)
    {
        MY_LOGGER_LOG("Uniform pattern : incorrect data length. Skipping update.\n\n")
        return false;
    }

    auto *tmp_data = (uint8_t *) data;

    targetRed_ = tmp_data[0];
    targetGreen_ = tmp_data[1];
    targetBlue_ = tmp_data[2];

    return true;
}
