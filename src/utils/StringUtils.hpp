/**
 * @file   StringUtils.hpp
 * @author see AUTHORS
 * @brief  StringUtils header file.
 */

#ifndef STRINGUTILS_HPP
#define STRINGUTILS_HPP

#include <string>
#include <sstream>
#include <vector>

#include <boost/algorithm/string.hpp>

/**
 * @brief split a string on every occurance of a given delimiter.
 * @param s the string.
 * @param delim the delimiter.
 * @return a vector of strings, delimiter characters removed.
 */
std::vector<std::string> split(const std::string & s, const char delim);

/**
 * @brief replace every occurance of a string with another string.
 * @param str the string to be modified.
 * @param from the string to be replaced.
 * @param to the string to replace from.
 * @return the modified string.
 */
std::string replace(const std::string & str, const std::string & from, const std::string & to);

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
