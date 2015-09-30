/* TypeUtils.cpp
 * -*- coding: utf-8 -*-
 *
 */

#include "TypeUtils.hpp"

double char_to_double(const char * const memory_block,
                     const bool little_endian) {
    float tmp = char_to_float(memory_block, little_endian);
    return static_cast<double>(tmp);
}

float char_to_float(const char * const memory_block,
                     const bool little_endian) {

    int tmp = char_to_int(memory_block, little_endian);
    return reinterpret_cast<float&>(tmp);
}

int char_to_int(const char * const memory_block,
                const bool little_endian) {

    unsigned int integer_value = 0;

    if (little_endian) {
        for (int i = 0; i < 4; ++i) {
            integer_value |=
                ((unsigned int)(memory_block[i] << i*8) &
                (unsigned int) (0x000000ff << i*8));
        }
    } else {
        for (int i = 0; i < 4; ++i) {
            integer_value |=
                ((unsigned int)(memory_block[i] << (3-i)*8) &
                (unsigned int) (0x000000ff << (3-i)*8));
        }
    }

    return integer_value;
}



// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
