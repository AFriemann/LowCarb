/* Atom.cpp
 * -*- coding: utf-8 -*-
 *
 */

#include <boost/test/unit_test.hpp>

// system includes =============================================================

#include <string>

// local includes ==============================================================

#include "../src/Atom.hpp"

#include "utils/log.hpp"

BOOST_AUTO_TEST_SUITE(Atom_test)

BOOST_AUTO_TEST_CASE(atom) {
    TEST_MESSAGE("atom");

    float x = 1.11f;
    float y = 2.3246f;
    float z = 234.23f;
    std::string long_type = "CA  ";
    unsigned atom_nr = 23;
    unsigned res_nr = 42;
    Atom atom(x, y, z, long_type, atom_nr, res_nr);
    BOOST_CHECK_EQUAL(atom.get_x(), x);
    BOOST_CHECK_EQUAL(atom.get_y(), y);
    BOOST_CHECK_EQUAL(atom.get_z(), z);
}

BOOST_AUTO_TEST_SUITE_END()

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
