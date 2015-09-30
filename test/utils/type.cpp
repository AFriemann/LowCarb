/* type.cpp
 * -*- coding: utf-8 -*-
 *
 */

#include "type.hpp"

double cast_hex_to_double(std::string & hex_string) {
    unsigned long buffer;
    std::stringstream ss;
    ss << std::hex << hex_string;
    ss >> buffer;
    return reinterpret_cast<double&>(buffer);
}

double cast_hex_to_double(const std::string& hex_string) {
    unsigned long buffer;
    std::stringstream ss;
    ss << std::hex << hex_string;
    ss >> buffer;
    return reinterpret_cast<double&>(buffer);
}


// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
