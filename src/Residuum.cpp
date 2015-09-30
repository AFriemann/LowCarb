/**
 * @file   Residuum.cpp
 * @author see AUTHORS
 * @brief  Residuum definitions file.
 */

#include "Residuum.hpp"

Residuum::Residuum(const Atom & c_alpha, const std::vector<Atom> & atoms)
    : c_alpha(c_alpha), atoms(atoms), Atom(c_alpha) {}

Atom Residuum::get_c_alpha() const {
    return this->c_alpha;
}

double Residuum::get_mass() const {
    double mass = 0;
    for (Atom const & atom : this->atoms) {
        mass += atom.get_mass();
    }
    return mass;
}

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
