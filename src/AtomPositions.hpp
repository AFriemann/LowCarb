/**
 * @file   AtomPositions.hpp
 * @author see AUTHORS
 * @brief  AtomPositions header file.
 */

#ifndef ATOMPOSITIONS_HPP
#define ATOMPOSITIONS_HPP

#include <vector>
#include <iostream>
#include <stdexcept>

#include <Eigen/Dense>

/**
 * @class AtomPositions
 * @brief holds a series of positions of atoms
 */
class AtomPositions {
 public:
    AtomPositions();

    /**
     * @param x the x coordinates of all the atoms
     * @param y the y coordinates of all the atoms
     * @param z the z coordinates of all the atoms
     */
    AtomPositions(
        const std::vector<double> & x,
        const std::vector<double> & y,
        const std::vector<double> & z);
    ~AtomPositions();
    /**
     * @brief returns the x position
     * @return the x position
     */
    std::vector<double> get_x() const;
    /**
     * @brief returns the y position
     * @return the y position
     */
    std::vector<double> get_y() const;
    /**
     * @brief returns the z position
     * @return the z position
     */
    std::vector<double> get_z() const;
    /**
     * @brief returns the atom position of the selected atom
     * @param atom_number the number of the selected atom
     * @return returns the atom position
     */
    Eigen::Vector3d operator () (const int atom_number) const;


 private:
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;
};

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
