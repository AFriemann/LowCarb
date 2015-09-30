/* Frame.cpp
 * -*- coding: utf-8 -*-
 *
 */

#include <boost/test/unit_test.hpp>

// system includes =============================================================

#include <vector>

// local includes ==============================================================

#include "../src/Frame.hpp"

#include "utils/log.hpp"

BOOST_AUTO_TEST_SUITE(Frame_test)

BOOST_AUTO_TEST_CASE(frame_successful_creation) {
    TEST_MESSAGE("frame_successful_creation");

    std::vector<double> x = {0, 1};
    std::vector<double> y = {2, 3};
    std::vector<double> z = {3, 4};
    Frame frame(x, y, z);

    BOOST_CHECK_EQUAL(frame.get_atom_positions().get_x()[0], x[0]);
    BOOST_CHECK_EQUAL(frame.get_atom_positions().get_y()[0], y[0]);
    BOOST_CHECK_EQUAL(frame.get_atom_positions().get_z()[0], z[0]);
    BOOST_CHECK_EQUAL(frame.get_atom_positions().get_x()[1], x[1]);
    BOOST_CHECK_EQUAL(frame.get_atom_positions().get_y()[1], y[1]);
    BOOST_CHECK_EQUAL(frame.get_atom_positions().get_z()[1], z[1]);
}

BOOST_AUTO_TEST_CASE(frame_failed_creation) {
    TEST_MESSAGE("frame_failed_creation");

    std::vector<double> x = {0};
    std::vector<double> y = {0, 1};
    std::vector<double> z = {0, 1, 2};
    BOOST_REQUIRE_THROW(Frame frame(x, y, z), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
