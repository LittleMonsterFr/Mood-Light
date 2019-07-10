#include "patterns/emptyPattern.h"

void EmptyPattern::process(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w)
{
    *r = 0;
    *g = 0;
    *b = 0;
}

bool EmptyPattern::updateData(void *data, size_t dataLength)
{
    return true;
}
