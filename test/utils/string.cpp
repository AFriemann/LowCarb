/* string.cpp
 * -*- coding: utf-8 -*-
 *
 */

#include "string.hpp"

int get_line_count(std::string const & filename) {
    std::ifstream f(filename);
    std::string line;
    int i;
    for (i = 0; std::getline(f, line); ++i) {
    }
    return i;
}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
