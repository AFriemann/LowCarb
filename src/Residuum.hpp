/**
 * @file   Residuum.hpp
 * @author see AUTHORS
 * @brief  Residuum header file.
 */

#ifndef RESIDUUM_HPP
#define RESIDUUM_HPP

#include <string>
#include <vector>

#include "Atom.hpp"

/**
 * @class Residuum
 * @brief Holds the information of a set of atoms that are a residuum
 */
class Residuum : public Atom {
public:
    Residuum() {};//LCOV_EXCL_LINE

    /**
    * @param c_alpha The c_alpha atom central to this residuum.
    * @param atoms std:vector of Atoms associated to the c_alpha atom.
    */
    Residuum(const Atom & c_alpha, const std::vector<Atom> & atoms);

    /**
    * @return the c_alpha Atom.
    */
    Atom get_c_alpha() const;

    /**
    * @return the summarized atom mass.
    */
    double get_mass() const;

private:
    Atom c_alpha;
    std::vector<Atom> atoms;
};

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
