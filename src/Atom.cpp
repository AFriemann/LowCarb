/**
 * @file   Atom.cpp
 * @author see AUTHORS
 * @brief  Atom definitions file.
 */

#include "Atom.hpp"

Atom::Atom(const double x, const double y, const double z,
           const std::string & type, const int atom_number, const int residuum_number)
    : x(x), y(y), z(z), type(type), atom_number(atom_number), residuum_number(residuum_number) {}

Eigen::Vector3d Atom::get_position() const {
    Eigen::Vector3d position(
        this->x,
        this->y,
        this->z);
    return position;
}

void Atom::set_position(const Eigen::Vector3d & position) {
    this->x = position(0);
    this->y = position(1);
    this->z = position(2);
}

double Atom::get_mass() const {
    switch (this->type[0]) {
      case 'C': return 12.011;
      case 'O': return 15.999;
      case 'N': return 14.077;
      case 'S': return 32.06;
      default: return 1.008;
    }
}

bool Atom::is_c_alpha() const {
    return this->type.compare("CA A") == 0 ||
           this->type.compare("CA  ") == 0;
}

//LCOV_EXCL_START
double Atom::get_x() const {
    return this->x;
}

double Atom::get_y() const {
    return this->y;
}

double Atom::get_z() const {
    return this->z;
}

int Atom::get_atom_number() const {
    return this->atom_number;
}

int Atom::get_residuum_number() const {
    return this->residuum_number;
}

std::string Atom::get_type() const {
    return this->type;
}
//LCOV_EXCL_STOP

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
