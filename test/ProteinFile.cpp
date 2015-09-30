/* ProteinFile.cpp
 * -*- coding: utf-8 -*-
 *
 */

#include <boost/test/unit_test.hpp>

// system includes =============================================================

#include <string>
#include <csv.h>

// local includes ==============================================================

#include "../src/ProteinFile.hpp"

#include "utils/definitions.hpp"
#include "utils/log.hpp"

BOOST_AUTO_TEST_SUITE(PDB_test)

BOOST_AUTO_TEST_CASE(pdb_read_atoms_test) {
    TEST_MESSAGE("pdb_read_atoms_test");

    io::CSVReader<5> test_data(
        "test/resources/pdb/atoms.csv");
    PDB uut("test/resources/integration/mb_ca.pdb");
    test_data.read_header(io::ignore_extra_column,
        "long_type", "res_nr", "x", "y", "z");
    std::string long_type; int res_nr; double x; double y; double z;
    int line_count = 0;
    for (Atom atom : uut.get_atoms()) {
        bool next_line_present = test_data.read_row(long_type, res_nr, x, y, z);
        BOOST_CHECK(next_line_present);
        BOOST_CHECK_EQUAL(atom.get_atom_number(), ++line_count);
        BOOST_CHECK_EQUAL(atom.get_residuum_number(), res_nr);
        BOOST_CHECK_EQUAL("'"+ atom.get_type()+"'", long_type);
        BOOST_CHECK_CLOSE_FRACTION(atom.get_x(), x, PRECISION);
        BOOST_CHECK_CLOSE_FRACTION(atom.get_y(), y, PRECISION);
        BOOST_CHECK_CLOSE_FRACTION(atom.get_z(), z, PRECISION);
    }
    BOOST_CHECK(!test_data.read_row(long_type, res_nr, x, y, z));
}

BOOST_AUTO_TEST_SUITE_END()

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
