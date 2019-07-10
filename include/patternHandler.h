#ifndef MOOD_LIGHT_PATTERN_HANDLER_H
#define MOOD_LIGHT_PATTERN_HANDLER_H

#include <vector>
#include "pattern.h"
#include "patterns/transitionPattern.h"

class PatternHandler
{
public:
    PatternHandler(Pattern *errorPattern, Pattern *decodeFailedPattern, TransitionPattern *transitionPattern)
            : errorPattern_(errorPattern), decodeFailedPattern_(decodeFailedPattern)
    {
        transitionPattern_ = transitionPattern;
        transitionPattern_->setPatternHandler(this);
    };

    ~PatternHandler()
    {
        for (Pattern *pattern : vector_)
        {
            delete pattern;
        }
        vector_.clear();
    }

    /**
     * Add a pattern to the pattern handler.
     *
     * @param pattern Pattern to add.
     */
    bool add(Pattern *pattern);

    /**
     * Create the patterns from  a file
     *
     * @param filePath Path to the json file containing the patterns to create.
     * @return         True if everything goes well, false otherwise.
     */
    bool addFromFile(const char *filePath);

    /**
     * Select the pattern corresponding with the given ID and use the last
     * parameters set for this pattern
     *
     * @param   id Identifier of the pattern to select.
     * @return     True if everything goes well, false otherwise.
     */
    bool select(uint8_t id);

    /**
     * Update the current pattern with the following parameters.
     *
     * @param id         The ID of the pattern we want to set.
     * @param sleep      The time, in millisecond, between two update of the
     *                   full LED array.
     * @param intensity  The global intensity of the light, in percentage.
     * @param data       Additional data needed by the pattern.
     * @param dataLength Length of the additional data.
     * @return           True if everything goes well, false otherwise.
     */
    bool update(uint8_t id, uint8_t sleep, uint8_t intensity, void *data, size_t dataLength);

    /**
     * Get the current pattern used
     *
     * @return The current pattern.
     */
    Pattern *getCurrentPattern();

    /**
     * When this function is called, the pattern signifying a failed decode is
     * set.
     */
    void decodeFailed();

    void startTransition(Pattern *toSelectPattern);

    void endTransition(Pattern *toSelectPattern);

private:
    // Store the pattern pointers
    std::vector<Pattern *> vector_;

    // Current pattern pointer
    Pattern *currentPattern_ = nullptr;

    // Pointer to the error pattern
    Pattern *errorPattern_ = nullptr;

    // Pointer to the decode failed pattern
    Pattern *decodeFailedPattern_ = nullptr;

    // Pointer to the transition pattern
    TransitionPattern *transitionPattern_ = nullptr;
};

#endif /* !PATTERN_HANDLER_H */
