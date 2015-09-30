/**
 * @file   Frame.cpp
 * @author see AUTHORS
 * @brief  Frame definitions file.
 */

#include "Frame.hpp"

Frame::Frame(const std::vector<double> & x,
             const std::vector<double> & y,
             const std::vector<double> & z) : atom_positions(x, y, z) {}

//LCOV_EXCL_START
AtomPositions Frame::get_atom_positions() const {
    return this->atom_positions;
}
//LCOV_EXCL_STOP

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
