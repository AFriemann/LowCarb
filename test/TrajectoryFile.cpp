/* TrajectoryFileTests.cpp
 * -*- coding: utf-8 -*-
 *
 */

#include <boost/test/unit_test.hpp>

// system includes =============================================================

#include <string>
#include <csv.h>
#include <boost/filesystem/path.hpp>

// local includes ==============================================================

#include "../src/XTC.hpp"
#include "../src/DCD.hpp"
#include "../src/Frame.hpp"
#include "../src/TRR.hpp"
#include "../src/utils/CSVReader.hpp"

#include "utils/file.hpp"
#include "utils/test.hpp"
#include "utils/log.hpp"
#include "utils/definitions.hpp"

#define XDR_PRECICION 10

BOOST_AUTO_TEST_SUITE(xdr_test)
    BOOST_AUTO_TEST_CASE(trr_test){
        TEST_MESSAGE("trr_test");

        TRR trajectory("test/resources/integration/mb_ca.trr");
        boost::filesystem::path path("test/resources/integration/mb_ca_dump.csv");
        CSVReader reader;

        std::vector<Eigen::Vector3d> vector = reader.read_as_vector(path);
        BOOST_CHECK_EQUAL(trajectory.get_atom_count(), vector.size());

        if (trajectory.has_next()){
            AtomPositions atoms = trajectory.get_next_frame().get_atom_positions();
            BOOST_CHECK_EQUAL(atoms.get_x().size(), vector.size());
            for (int i = 0; i < vector.size(); i++) {
                BOOST_REQUIRE_CLOSE(atoms.get_x()[i], vector[i][0], XDR_PRECICION);
                BOOST_REQUIRE_CLOSE(atoms.get_y()[i], vector[i][1], XDR_PRECICION);
                BOOST_REQUIRE_CLOSE(atoms.get_z()[i], vector[i][2], XDR_PRECICION);
            }
        } else {
            BOOST_FAIL("trr trajectory had no frame");
        }
    }

    BOOST_AUTO_TEST_CASE(xtc_test){
        TEST_MESSAGE("xtc_test");

        XTC trajectory("test/resources/integration/mb_ca.xtc");
        boost::filesystem::path path("test/resources/integration/mb_ca_dump.csv");
        CSVReader reader;
        std::vector<Eigen::Vector3d> vector = reader.read_as_vector(path);
        BOOST_CHECK_EQUAL(trajectory.get_atom_count(), vector.size());

        if (trajectory.has_next()){
            AtomPositions atoms = trajectory.get_next_frame().get_atom_positions();
            BOOST_CHECK_EQUAL(atoms.get_x().size(), vector.size());
            for (int i = 0; i < vector.size(); i++) {
                BOOST_REQUIRE_CLOSE(atoms.get_x()[i], vector[i][0], XDR_PRECICION);
                BOOST_REQUIRE_CLOSE(atoms.get_y()[i], vector[i][1], XDR_PRECICION);
                BOOST_REQUIRE_CLOSE(atoms.get_z()[i], vector[i][2], XDR_PRECICION);
            }
        } else {
            BOOST_FAIL("xtc trajectory had no frame");
        }

    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(DCD_test)

    BOOST_AUTO_TEST_CASE(endiannes_check) {
        TEST_MESSAGE("endiannes_check");

        DCD trajectory("test/resources/integration/mb_ca.dcd");

        BOOST_REQUIRE_EQUAL(trajectory.is_little_endian(), true);
    }

    BOOST_AUTO_TEST_CASE(dcd_head_test) {
        TEST_MESSAGE("dcd_head_test");

        io::CSVReader<3> test_data(
            "test/resources/dcd/dcd_head_testdata.csv");
        test_data.read_header(io::ignore_extra_column,
            "in_filename", "out_natm", "out_nfra");
        std::string in_filename; int out_natm; int out_nfra;
        while (test_data.read_row(in_filename, out_natm, out_nfra)) {
            DCD trajectory("test/resources/"+in_filename);
            BOOST_CHECK_EQUAL(trajectory.get_atom_count(), out_natm);
            BOOST_CHECK_EQUAL(trajectory.get_frame_count(), out_nfra);
        }
    }

    BOOST_AUTO_TEST_CASE(dcd_body_test) {
        TEST_MESSAGE("dcd_body_test");

        io::CSVReader<3> test_data("test/resources/dcd/dcd_frame_testdata.csv");
        test_data.read_header(io::ignore_extra_column, "in_dcdfile", "in_ifra", "out_ref");
        std::string in_dcd_filename; int in_frame_number;
        std::string out_matrix_file;
        test_data.read_row(in_dcd_filename,
            in_frame_number, out_matrix_file);
        DCD dcd_under_test("test/resources/"+in_dcd_filename);
        for (size_t i = 0; i < dcd_under_test.get_frame_count(); i++) {
            Frame out_matrix = get_frame_from_csv("test/resources/dcd_frames/"+out_matrix_file);
            Frame frame_under_test = dcd_under_test.get_next_frame();
            check_frames_for_equality(frame_under_test, out_matrix, PRECISION);
            test_data.read_row(in_dcd_filename, in_frame_number, out_matrix_file);
        }
        BOOST_CHECK(!dcd_under_test.has_next());
    }

    BOOST_AUTO_TEST_CASE(dcd_crystal_information_test) {
        TEST_MESSAGE("dcd_body_test_crystal_information");

        std::string basepath("test/resources/crystal_info/");
        std::vector<std::string> frame_data_files = {"cat_pnb_subset_2_dcd_frame0001.csv", "cat_pnb_subset_2_dcd_frame0002.csv"};
        DCD dcd_under_test(basepath + "cat_pnb_subset_2_frames.dcd", true);
        for (int i = 0; i < 2; ++i){
            Frame frame_from_csv = get_frame_from_csv_not_hex(basepath + frame_data_files[i]);
            Frame frame_from_dcd = dcd_under_test.get_next_frame();
            check_frames_for_equality(frame_from_csv, frame_from_dcd, PRECISION);
        }
    }

BOOST_AUTO_TEST_SUITE_END()

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
