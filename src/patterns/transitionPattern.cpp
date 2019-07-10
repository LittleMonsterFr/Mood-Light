#include "myLogger.h"
#include "helpers.h"
#include "patterns/transitionPattern.h"
#include "patternHandler.h"

void TransitionPattern::process(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w)
{
    if (ledIndex_ == 0)
    {
        count_++;
        on_ ? *w = 255 : *w = 0;
        *r = 0;
        *g = 0;
        *b = 0;
        on_ = !on_;
    }

    ledIndex_++;

    if (ledIndex_ == numLed_)
    {
        ledIndex_ = 0;

        if (count_ == 5)
        {
            count_ = 0;
            on_ = false;
            patternHandler_->endTransition(toSelectPattern_);
            return;
        }
    }

}

bool TransitionPattern::updateData(void *data, size_t dataLength)
{
    return true;
}

void TransitionPattern::setPatternHandler(PatternHandler *patternHandler)
{
    patternHandler_ = patternHandler;
}

void TransitionPattern::setToSelectPattern(Pattern *toSelectPattern)
{
    toSelectPattern_ = toSelectPattern;
}
