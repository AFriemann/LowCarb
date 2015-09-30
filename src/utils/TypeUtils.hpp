/* TypeUtils.hpp
 * -*- coding: utf-8 -*-
 *
 *
 */

#ifndef TYPEUTILS_HPP
#define TYPEUTILS_HPP

/**
* @brief Converts a char array to an float.
* @param memory_block the memory block that is to be converted
* @param memory_block_length the size of the memory block; should be 4
* @return an ieee float corresponding to the layout of the memory block
*/
double char_to_double(const char * const memory_block,
                     const bool little_endian);


float char_to_float(const char * const memory_block,
                     const bool little_endian);

/**
* @brief Converts a char array to an int.
* @param memory_block the memory block that is to be converted
* @param memory_block_length the size of the memory block; should be 4
* @param little_endian
* @return a signed int corresponding to the layout of the memory block
*/
int char_to_int(const char * const memory_block,
                const bool little_endian);
#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
