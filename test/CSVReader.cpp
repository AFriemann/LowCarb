/* CSVReader.cpp
 * -*- coding: utf-8 -*-
 *
 */

#include <boost/test/unit_test.hpp>

#include <stdexcept>

#include <boost/filesystem.hpp>

#include <Eigen/Dense>

// local includes ==============================================================

#include "../src/utils/CSVReader.hpp"

#include "utils/log.hpp"

BOOST_AUTO_TEST_SUITE(CSVReaderTestSuite)
    struct Setup {
        Setup() { reader = CSVReader(); }

        CSVReader reader;

        std::string test_file = "test/resources/csvreader/matrix.csv";
    };

    BOOST_AUTO_TEST_CASE(read_csv) {
        TEST_MESSAGE("read_csv");

        Setup f;

        Eigen::MatrixXd expected(2,2);

        expected(0,0) = 0.0000000000000000;
        expected(0,1) = 1101.5312671722938376;
        expected(1,0) = 3.8522202257825890;
        expected(1,1) = 1119.2784683923078774;

        Eigen::MatrixXd actual = f.reader.read_matrix(f.test_file, 2);

        BOOST_REQUIRE_EQUAL(expected, actual);
    }

    BOOST_AUTO_TEST_CASE(throw_if_file_nonexistent) {
        TEST_MESSAGE("throw_if_file_nonexistent");

        Setup f;

        BOOST_REQUIRE_THROW(f.reader.read_matrix(boost::filesystem::path("test/resources/csvreader/nonexistent_file.csv"), 2),
                            std::runtime_error);
    }

BOOST_AUTO_TEST_SUITE_END()

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
