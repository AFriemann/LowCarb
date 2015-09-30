/**
 * @file   AtomGroup.cpp
 * @author see AUTHORS
 * @brief  AtomGroup definitions file.
 */

#include "AtomGroup.hpp"

std::vector<Eigen::Vector3d> AtomGroup::get_coordinates() const {
    std::vector<Eigen::Vector3d> coordinates(this->atoms.size());
    for (int i = 0; i < this->atoms.size(); ++i){
        coordinates[i] = this->atoms[i].get_position();
    }
    return coordinates;
}

double AtomGroup::get_total_mass() const {
    double mass = 0;
    for (int i = 0; i < this->atoms.size(); ++i) {
        mass += this->atoms[i].get_mass();
    }
    return mass;
}

Eigen::Vector3d AtomGroup::get_center_of_mass() const {
    Eigen::Vector3d center(0,0,0);
    for (int i = 0; i < this->atoms.size(); ++i) {
        Eigen::Vector3d position = atoms[i].get_position();
        position *= atoms[i].get_mass();
        center += position;
    }
    center /= get_total_mass();
    return center;
}

Eigen::Matrix3Xd AtomGroup::get_relative_positions() const {
    Eigen::Matrix3Xd centered_positions(3, atoms.size());
    Eigen::Vector3d center_of_mass = get_center_of_mass();
    for(int i = 0; i < this->atoms.size(); i++) {
        centered_positions.col(i) = this->atoms[i].get_position() - center_of_mass;
    }
    return centered_positions;
}

//LCOV_EXCL_START
AtomGroup::AtomGroup() {}

AtomGroup::~AtomGroup() {}

std::vector<Atom> AtomGroup::get_atoms() const {
    return this->atoms;
}


int AtomGroup::get_size() const {
    return this->atoms.size();
}
//LCOV_EXCL_STOP

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
