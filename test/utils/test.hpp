/* test.hpp
 * -*- coding: utf-8 -*-
 *
 *
 */

#ifndef TEST_HPP
#define TEST_HPP

// system includes =============================================================

#include <stdexcept>

#include <boost/filesystem.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <Eigen/Dense>

// local includes ==============================================================

#include "../src/Atom.hpp"
#include "../src/Frame.hpp"
#include "../src/SecondaryStructureFile.hpp"

#include "type.hpp"

void check_atoms_for_equality(const Atom& atom1, const Atom& atom2);

void check_atoms_for_equality(const Atom& atom1, const Atom& atom2, float error);

void check_frames_for_equality(const Frame& frame1, const Frame& frame2, float error);

void compare_two_sym_matrices(const Eigen::MatrixXd & mat_a, const Eigen::MatrixXd & mat_b,
                              double precision);

void compare_ranges(std::vector<Range> ranges1, std::vector<Range> & ranges2);

void compare_force_constant_matrix(
        Eigen::MatrixXd & low_carb_result,
        const std::string & filename_reach_results,
        double precision,
        bool is_hex,
        bool toggle_row_and_col);

bool close_to_zero(double a, double eps);

double get_precision(double a, double b);

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
