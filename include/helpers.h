#ifndef MOOD_LIGHT_HELPERS_H
#define MOOD_LIGHT_HELPERS_H

#include <cstdint>

/**
 * Covert a float to an uin8_t. If the input is bigger or lower than the allowed
 * bounds of the type the bounds are returned otherwise the value is rounded.
 *
 * @param val Float to clamp.
 * @return    Clamped value.
 */
uint8_t clamp_255(float val);

#endif /* !MOOD_LIGHT_HELPERS_H */
