#include <iostream>

#include "helpers.h"
#include "myLogger.h"
#include "patterns/whiteLightPattern.h"

void WhiteLightPattern::process(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w)
{
    *w = 255;
}

bool WhiteLightPattern::updateData(void *data, size_t dataLength)
{
    return true;
}
