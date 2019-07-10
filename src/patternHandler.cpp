#include <iostream>

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "boost/iterator.hpp"
namespace pt = boost::property_tree;

#include "myLogger.h"
#include "patternHandler.h"

#include "patterns/emptyPattern.h"
#include "patterns/hsvPattern.h"
#include "patterns/rainbowWheelPattern.h"
#include "patterns/sineRainbowPattern.h"
#include "patterns/randomPattern.h"
#include "patterns/wavePattern.h"
#include "patterns/uniformPattern.h"
#include "patterns/fastLedRainbowPattern.h"
#include "patterns/whiteLightPattern.h"
#include "patterns/duetPattern.h"
#include "patterns/candlePattern.h"

bool PatternHandler::add(Pattern *pattern)
{
    if (pattern->getId() < 0)
    {
        MY_LOGGER_LOG("Pattern id %d is forbidden. The pattern won't be added.\n", pattern->getId());
        return false;
    }

    for (Pattern *p : vector_)
    {
        if (p->getId() == pattern->getId())
        {
            MY_LOGGER_LOG("Pattern id %d is already used. The pattern won't be added.\n", pattern->getId());
            return false;
        }
    }

    vector_.push_back(pattern);
    return true;
}

bool PatternHandler::select(uint8_t id)
{
    for (Pattern *pattern : vector_)
    {
        if (pattern->getId() == id)
        {
            startTransition(pattern);
            return true;
        }
    }

    // If the id is not recognized, set the error pattern
    currentPattern_ = errorPattern_;
    return false;
}

Pattern *PatternHandler::getCurrentPattern()
{
    return currentPattern_;
}

bool PatternHandler::update(uint8_t id, uint8_t sleep, uint8_t intensity,
                            void *data, size_t dataLength)
{
    for (Pattern *pattern : vector_)
    {
        if (pattern->getId() == id)
        {
            // Multiplication by 1000 to get a millisecond base time
            pattern->setSleep(sleep * 1000U);

            if (intensity > 100U)
            {
                MY_LOGGER_LOG("Intensity received is bigger than 100. The value will be skipped.\n");
            }
            else
            {
                // Division by 100 to be between 0 and 1
                pattern->setIntensity(intensity / 100.0f);
            }

            // Only update the data if there is some data to update
            if (data)
            {
                /*
                 * Update the internal data of the pattern if the data length is
                 * correct. Otherwise the loop is broken and the error pattern
                 * is set
                 */
                if (!pattern->updateData(data, dataLength))
                    break;
            }

            startTransition(pattern);
            return true;
        }
    }

    // If the id is not recognized, set the error pattern
    currentPattern_ = errorPattern_;
    return false;
}

bool PatternHandler::addFromFile(const char *filePath)
{
    // Create a root
    pt::ptree root;

    // Load the json file in this ptree
    pt::read_json(filePath, root);

    int8_t emptyPatternId = -1;

    for (pt::ptree::value_type &patternRoot : root)
    {
        auto id = patternRoot.second.get<uint8_t>("id");
        auto sleep = patternRoot.second.get<uint32_t>("sleep");
        auto intensity = patternRoot.second.get<float>("intensity");
        Pattern *pattern = nullptr;

        if (patternRoot.first == "Empty")
        {
            pattern = new EmptyPattern(id, sleep, intensity, nullptr);
            emptyPatternId = id;
        }
        else if(patternRoot.first == "RainbowWheel")
            pattern = new RainbowWheelPattern(id, sleep, intensity, nullptr);
        else if(patternRoot.first == "Hsv")
            pattern = new HsvPattern(id, sleep, intensity, nullptr);
        else if(patternRoot.first == "SineRainbow")
            pattern = new SineRainbowPattern(id, sleep, intensity, nullptr);
        else if(patternRoot.first == "Random")
            pattern = new RandomPattern(id, sleep, intensity, nullptr);
        else if(patternRoot.first == "Wave")
            pattern = new WavePattern(id, sleep, intensity, nullptr);
        else if(patternRoot.first == "Uniform")
            pattern = new UniformPattern(id, sleep, intensity, nullptr);
        else if(patternRoot.first == "FastLED")
            pattern = new FastLedRainbowPattern(id, sleep, intensity, nullptr);
        else if(patternRoot.first == "WhiteLight")
            pattern = new WhiteLightPattern(id, sleep, intensity, nullptr);
        else if(patternRoot.first == "Duet")
            pattern = new DuetPattern(id, sleep, intensity, nullptr);
        else if(patternRoot.first == "Candle")
            pattern = new CandlePattern(id, sleep, intensity, nullptr);
        else
        {
            MY_LOGGER_LOG("Pattern class not recognised. Stopping adding patterns.\n")
            return false;
        }

        add(pattern);
        select((uint8_t) pattern->getId());
    }

    if (emptyPatternId != -1)
        select((uint8_t) emptyPatternId);

    return true;
}

void PatternHandler::decodeFailed()
{
    currentPattern_ = decodeFailedPattern_;
}

void PatternHandler::startTransition(Pattern *toSelectPattern)
{
    transitionPattern_->setToSelectPattern(toSelectPattern);
    currentPattern_ = transitionPattern_;
}

void PatternHandler::endTransition(Pattern *toSelectPattern)
{
    currentPattern_ = toSelectPattern;
}

