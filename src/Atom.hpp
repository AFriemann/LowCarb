/**
 * @file   Atom.hpp
 * @author see AUTHORS
 * @brief  Atom header file.
 */

#ifndef ATOM_HPP
#define ATOM_HPP

#include <string>

#include <Eigen/Dense>

/**
 * @class Atom
 * @brief represents one Atom
 */
class Atom {
 public:
    /**
    * @param x
    * @param y
    * @param z
    * @param type
    * @param atom_number
    * @param residuum_number
    */
    Atom(const double x, const double y, const double z,
         const std::string &type, const int atom_number, const int residuum_number);

    /**
    * @brief returns the atoms x coordinate
    * @return x the x coordinate
    */
    double get_x() const;

    /**
    * @brief returns the atoms y coordinate
    * @return x the y coordinate
    */
    double get_y() const;

    /**
    * @brief returns the atoms z coordinate
    * @return x the z coordinate
    */
    double get_z() const;

    /**
    * @brief returns the atoms position
    * @return position the position of the atom as 3d vector
    */
    Eigen::Vector3d get_position() const;

    /**
    * @brief sets the atoms position
    */
    void set_position(const Eigen::Vector3d & position);

    /**
    * @brief returns the atoms number
    * @return the atoms number
    */
    int get_atom_number() const;

    /**
    * @brief returns the number of the residuum containing the atom
    * @return the residuum number
    */
    int get_residuum_number() const;

    /**
    * @brief returns the atoms mass
    * @return mass the mass of the atom
    */
    double get_mass() const;

    /**
    * @brief returns wether the atom is a residuum
    * @return true if atom is a c alpha elso false
    */
    bool is_c_alpha() const;

    /**
    * @brief returns the atoms type
    * @return the atoms type
    */
    std::string get_type() const;

    Atom() {};//LCOV_EXCL_LINE
 protected:
    std::string type;
    unsigned atom_number;
    unsigned residuum_number;
    double x;
    double y;
    double z;
};

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
