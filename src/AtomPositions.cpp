/**
 * @file   AtomPositions.cpp
 * @author see AUTHORS
 * @brief  AtomPositions definitions file.
 */

#include "AtomPositions.hpp"

AtomPositions::AtomPositions(const std::vector<double> & x,
                             const std::vector<double> & y,
                             const std::vector<double> & z) : x(x), y(y), z(z) {
    if (x.size() != y.size() || y.size() != z.size()) {
        throw std::runtime_error("could not create Frame because the coordainate vectors do not have the same size");
    }
}

Eigen::Vector3d AtomPositions::operator () (const int atom_number) const {
    Eigen::Vector3d position(
        this->x[atom_number - 1],
        this->y[atom_number - 1],
        this->z[atom_number - 1]);
    return position;
}

//LCOV_EXCL_START
AtomPositions::AtomPositions() {}

AtomPositions::~AtomPositions() {}

std::vector<double> AtomPositions::get_x() const {
    return this->x;
}

std::vector<double> AtomPositions::get_y() const {
    return this->y;
}

std::vector<double> AtomPositions::get_z() const {
    return this->z;
}
//LCOV_EXCL_STOP

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
