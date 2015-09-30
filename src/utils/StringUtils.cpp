/**
 * @file   StringUtils.cpp
 * @author see AUTHORS
 * @brief  StringUtils definitions file.
 */

#include "StringUtils.hpp"

std::vector<std::string> & split(const std::string & s, const char delim, std::vector<std::string> & elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        boost::algorithm::trim(item);
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string & s, const char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

std::string replace(const std::string & str, const std::string & from, const std::string & to) {
    std::string copy(str);

    size_t start_pos = copy.find(from);

    if (start_pos != std::string::npos) {
        copy.replace(start_pos, from.length(), to);
    }

    return copy;
}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
