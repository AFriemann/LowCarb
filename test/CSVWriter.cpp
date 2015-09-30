/* CSVWriterTest.cpp
 * -*- coding: utf-8 -*-
 *
 */

#include <boost/test/unit_test.hpp>

// system includes =============================================================

#include <vector>
#include <string>
#include <Eigen/Dense>
#include <boost/filesystem.hpp>

// local includes ==============================================================

#include "../src/utils/CSVWriter.hpp"

#include "utils/log.hpp"

BOOST_AUTO_TEST_SUITE(write_csv)

    BOOST_AUTO_TEST_CASE(write){
        TEST_MESSAGE("write");

        std::string path = "test/results/FOOBAR.csv";

        CSVWriter writer;
        writer.open(path);

        std::vector<std::string> header = {"a", "b", "c"};
        writer.write_line(header);

        std::vector<std::vector<std::string>> lines = {{"1", "2", "3"},{"4", "5", "6"}};
        for(std::vector<std::string> line : lines){
            writer.write_line(line);
        }

        writer.close();

        BOOST_REQUIRE(boost::filesystem::exists(path));
        boost::filesystem::remove(path);
    }

    BOOST_AUTO_TEST_CASE(write_matrix){
        TEST_MESSAGE("write_matrix");

        std::string temp_path = "test/results/FOOBARmatrix.csv";

        Eigen::MatrixXd test_matrix(3, 3);
        test_matrix(0, 0) = 1.0; test_matrix(0, 1) = 1.0; test_matrix(0, 2) = 2.0;
        test_matrix(1, 0) = 0.0; test_matrix(1, 1) = 3.0; test_matrix(1, 2) = 0.0;
        test_matrix(2, 0) = 2.0; test_matrix(2, 1) = 1.0; test_matrix(2, 2) = 1.0;

        CSVWriter writer;
        writer.open(boost::filesystem::path(temp_path));

        writer.write(test_matrix);

        writer.close();

        BOOST_REQUIRE(boost::filesystem::exists(temp_path));
        boost::filesystem::remove(temp_path);
    }

    BOOST_AUTO_TEST_CASE(write_vector_test) {
        TEST_MESSAGE("write_vector_test");

        Eigen::VectorXd test_vector(4);
        test_vector(0) = 0.0;
        test_vector(1) = 1.2;
        test_vector(2) = 3.4;
        test_vector(3) = 4.6;


        std::string temp_path = "test/results/FOOBARvector.csv";

        CSVWriter writer;

        writer.open(temp_path);
        writer.write(test_vector);

        writer.close();

        BOOST_REQUIRE(boost::filesystem::exists(temp_path));
        boost::filesystem::remove(temp_path);
    }


BOOST_AUTO_TEST_SUITE_END()

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
