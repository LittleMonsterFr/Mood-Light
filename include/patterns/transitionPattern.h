#ifndef MOOD_LIGHT_TRANSITION_PATTERN_H
#define MOOD_LIGHT_TRANSITION_PATTERN_H

#include "pattern.h"

// Forward declaration of the PatternHandler class to solve include loop issue
class PatternHandler;

class TransitionPattern : public Pattern
{
public:
    explicit TransitionPattern(int8_t id, uint32_t sleep, float intensity, void *data)
            : Pattern(id, sleep, intensity, data)
    { };

    void process(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w) override;

    bool updateData(void *data, size_t dataLength) override;

    void setPatternHandler(PatternHandler *patternHandler);

    void setToSelectPattern(Pattern *toSelectPattern);

private:
    uint8_t count_ = 0;
    uint8_t ledIndex_ = 0;
    PatternHandler *patternHandler_ = nullptr;
    Pattern *toSelectPattern_ = nullptr;
    bool on_ = false;
};

#endif /* !MOOD_LIGHT_TRANSITION_PATTERN_H */
