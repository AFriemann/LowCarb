/**
 * @file   Frame.hpp
 * @author see AUTHORS
 * @brief  Frame header file.
 */

#ifndef FRAME_HPP
#define FRAME_HPP

#include <vector>
#include <iostream>

#include "AtomPositions.hpp"

/**
 * @class Frame
 * @brief a data point in the protein trajectory, storing atom positions.
 */
class Frame {
 public:
    /**
     * @param x dimension x positions as std::vector of doubles.
     * @param y dimension y positions as std::vector of doubles.
     * @param z dimension z positions as std::vector of doubles.
     */
    Frame(const std::vector<double> & x,
          const std::vector<double> & y,
          const std::vector<double> & z);

    /**
     * @return the atom positions.
     */
     AtomPositions get_atom_positions() const;

 private:
    AtomPositions atom_positions;
};
#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
