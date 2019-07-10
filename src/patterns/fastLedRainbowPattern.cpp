/**
 * Code adapted (not cleaned !) from https://github.com/FastLED/FastLED.
 * Same idea as HSV but supposed to have a better yellow.
 */

#include <iostream>
#include <cmath>

#include "helpers.h"
#include "myLogger.h"
#include "patterns/fastLedRainbowPattern.h"

uint8_t scale8(uint8_t i, uint8_t scale)
{
    return (((uint16_t)i) * ((uint8_t) 1 + (uint16_t)(scale))) >> 8U;
}

uint8_t scale8_LEAVING_R1_DIRTY( uint8_t i, uint8_t scale)
{
    return (((uint16_t)i) * ((uint16_t)(scale)+1)) >> 8;
}

uint8_t scale8_video_LEAVING_R1_DIRTY(uint8_t i, uint8_t scale)
{
    uint8_t j = (((int)i * (int)scale) >> 8) + ((i&&scale)?1:0);
    // uint8_t nonzeroscale = (scale != 0) ? 1 : 0;
    // uint8_t j = (i == 0) ? 0 : (((int)i * (int)(scale) ) >> 8) + nonzeroscale;
    return j;
}


void FastLedRainbowPattern::process(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w)
{
    if ((patternType == PATTERN_TYPE_UNIFORM && ledIndex_ == numLed_) ||
        patternType == PATTERN_TYPE_CIRCULAR)
    {
        uint8_t hue = (ledIndex_ * maxIndex_ / numLed_ + index_) % maxIndex_;

        // Yellow has a higher inherent brightness than
        // any other color; 'pure' yellow is perceived to
        // be 93% as bright as white.  In order to make
        // yellow appear the correct relative brightness,
        // it has to be rendered brighter than all other
        // colors.
        // Level Y1 is a moderate boost, the default.
        // Level Y2 is a strong boost.
        const uint8_t Y1 = 1;
        const uint8_t Y2 = 0;

        // G2: Whether to divide all greens by two.
        // Depends GREATLY on your particular LEDs
        const uint8_t G2 = 0;

        // Gscale: what to scale green down by.
        // Depends GREATLY on your particular LEDs
        const uint8_t Gscale = 0;

        uint8_t offset = hue & 0x1F; // 0..31

        // offset8 = offset * 8
        uint8_t offset8 = offset * 8U;

        uint8_t third = scale8(offset8, 256 / 3); // max = 85

        uint8_t r1, g1, b1;

        if( ! (hue & 0x80) ) {
            // 0XX
            if( ! (hue & 0x40) ) {
                // 00X
                //section 0-1
                if( ! (hue & 0x20) ) {
                    // 000
                    //case 0: // R -> O
                    r1 = 255 - third;
                    g1 = third;
                    b1 = 0;
                } else {
                    // 001
                    //case 1: // O -> Y
                    if( Y1 ) {
                        r1 = 171;
                        g1 = 85 + third ;
                        b1 = 0;
                    }
                    if( Y2 ) {
                        r1 = 170 + third;
                        // uint8_t twothirds = (third << 1);
                        uint8_t twothirds = scale8( offset8, ((256 * 2) / 3)); // max=170
                        g1 = 85 + twothirds;
                        b1 = 0;
                    }
                }
            } else {
                //01X
                // section 2-3
                if( !  (hue & 0x20) ) {
                    // 010
                    //case 2: // Y -> G
                    if( Y1 ) {
                        //uint8_t twothirds = (third << 1);
                        uint8_t twothirds = scale8( offset8, ((256 * 2) / 3)); // max=170
                        r1 = 171 - twothirds;
                        g1 = 170 + third;
                        b1 = 0;
                    }
                    if( Y2 ) {
                        r1 = 255 - offset8;
                        g1 = 255;
                        b1 = 0;
                    }
                } else {
                    // 011
                    // case 3: // G -> A
                    r1 = 0;
                    g1 = 255 - third;
                    b1 = third;
                }
            }
        } else {
            // section 4-7
            // 1XX
            if( ! (hue & 0x40) ) {
                // 10X
                if( ! ( hue & 0x20) ) {
                    // 100
                    //case 4: // A -> B
                    r1 = 0;
                    //uint8_t twothirds = (third << 1);
                    uint8_t twothirds = scale8( offset8, ((256 * 2) / 3)); // max=170
                    g1 = 171 - twothirds; //170?
                    b1 = 85  + twothirds;

                } else {
                    // 101
                    //case 5: // B -> P
                    r1 = third;
                    g1 = 0;
                    b1 = 255 - third;

                }
            } else {
                if( !  (hue & 0x20)  ) {
                    // 110
                    //case 6: // P -- K
                    r1 = 85 + third;
                    g1 = 0;
                    b1 = 171 - third;

                } else {
                    // 111
                    //case 7: // K -> R
                    r1 = 170 + third;
                    g1 = 0;
                    b1 = 85 - third;

                }
            }
        }

        // This is one of the good places to scale the green down,
        // although the client can scale green down as well.
        if( G2 ) g1 = g1 >> 1;
        if( Gscale ) g1 = scale8_video_LEAVING_R1_DIRTY( g1, Gscale);

        // Scale down colors if we're desaturated at all
        // and add the brightness_floor to r1, g1, and b1.
        if( saturation_ != 255 ) {
            if( saturation_ == 0) {
                r1 = 255; b1 = 255; g1 = 255;
            } else {
                //nscale8x3_video( r1, g1, b1, saturation_);
                if( r1 ) r1 = scale8_LEAVING_R1_DIRTY( r1, saturation_);
                if( g1 ) g1 = scale8_LEAVING_R1_DIRTY( g1, saturation_);
                if( b1 ) b1 = scale8_LEAVING_R1_DIRTY( b1, saturation_);

                uint8_t desaturation_ = 255 - saturation_;
                desaturation_ = scale8( desaturation_, desaturation_);

                uint8_t brightness_floor = desaturation_;
                r1 += brightness_floor;
                g1 += brightness_floor;
                b1 += brightness_floor;
            }
        }

        // Now scale everything down if we're at value < 255.
        if( value_ != 255 ) {

            value_ = scale8_video_LEAVING_R1_DIRTY( value_, value_);
            if( value_ == 0 ) {
                r1=0; g1=0; b1=0;
            } else {
                // nscale8x3_video( r1, g1, b1, value_);
                if( r1 ) r1 = scale8_LEAVING_R1_DIRTY( r1, value_);
                if( g1 ) g1 = scale8_LEAVING_R1_DIRTY( g1, value_);
                if( b1 ) b1 = scale8_LEAVING_R1_DIRTY( b1, value_);
            }
        }

        // Here we have the old AVR "missing std X+n" problem again
        // It turns out that fixing it winds up costing more than
        // not fixing it.
        // To paraphrase Dr Bronner, profile! profile! profile!
        //asm volatile(  ""  :  :  : "r26", "r27" );
        //asm volatile (" movw r30, r26 \n" : : : "r30", "r31");
        *r = r1;
        *g = g1;
        *b = b1;

    }

    ledIndex_++;

    if ((patternType == PATTERN_TYPE_UNIFORM && ledIndex_ > numLed_) ||
        (patternType == PATTERN_TYPE_CIRCULAR && ledIndex_ >= numLed_))
    {
        ledIndex_ = 0;
        index_++;

        if (index_ >= maxIndex_)
        {
            index_ = 0;
        }
    }
}

bool FastLedRainbowPattern::updateData(void *data, size_t dataLength)
{
    if (dataLength != 3)
    {
        MY_LOGGER_LOG("Fast LED Rainbow pattern : incorrect data length. Skipping update.\n\n")
        return false;
    }

    auto *tmp_data = (uint8_t *) data;

    patternType = (PatternType) tmp_data[0];
    saturation_ = tmp_data[1];
    value_ = tmp_data[2];

    MY_LOGGER_LOG("Fast LED Rainbow pattern : saturation = %d, value = %d\n\n", saturation_, value_);

    return true;
}
