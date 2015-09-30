/**
 * @file   AtomGroup.hpp
 * @author see AUTHORS
 * @brief  AtomGroup header file.
 */

#ifndef ATOMGROUP_HPP
#define ATOMGROUP_HPP

#include <vector>
#include <iostream>

#include <Eigen/Dense>

#include "Atom.hpp"

/**
 * @class AtomGroup
 * @brief represents a group of atoms
 */
class AtomGroup {
 public:
    explicit AtomGroup();

    ~AtomGroup();

    /**
     * @brief returns all the atoms in the group
     * @return a vector containing all the atoms
     */
    std::vector<Atom> get_atoms() const;

    /**
     * @brief returns all the coordinates of all the atoms in the group
     * @return a vector containing a coordinates for all the atoms
     */
    std::vector<Eigen::Vector3d> get_coordinates() const;

    /**
    * @brief returns the size of the group (which is the number of atoms)
    * @return the size of the group
    */
    int get_size() const;

    /**
    * @brief returns the total mass of the atom group
    * @return the total mass of the atom group
    */
    double get_total_mass() const;

    /**
    * @brief returns the center of mass of the group
    * @return the coordinates of the center of mass
    */
    Eigen::Vector3d get_center_of_mass() const;

    /**
    * @brief returns the positions of the atoms relative to the mass center of the group
    * @return a matrix containing the positions of the atoms relative to the groups center of mass
    */
    Eigen::Matrix3Xd get_relative_positions() const;

 protected:
    std::vector<Atom> atoms;
};

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
