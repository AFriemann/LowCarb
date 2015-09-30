/* SecondaryStructureFile.cpp
 * -*- coding: utf-8 -*-
 *
 */

#include <boost/test/unit_test.hpp>

// system includes =============================================================

#include <string>
#include <csv.h>

// local includes ==============================================================

#include "../src/SecondaryStructureFile.hpp"

#include "utils/file.hpp"
#include "utils/test.hpp"
#include "utils/log.hpp"

BOOST_AUTO_TEST_SUITE(dssp)

BOOST_AUTO_TEST_CASE(dssp_read){
    TEST_MESSAGE("ddsp_read");

    std::string base_path_csv = "test/resources/dssp_csv/";
    std::string base_path_dssp = "test/resources/dssp_files/";
    io::CSVReader<8> test_data(base_path_csv + "dssp.csv");
    std::string filename;
    int npa;
    int numpalpha;
    int npb;
    int numpbeta;
    int numbetapair;
    std::string alpha_ref;
    std::string beta_ref;

    test_data.read_header(io::ignore_extra_column,
        "filename", "npa", "numpalpha", "npb", "numpbeta", "numbetapair", "alpha_ref", "beta_ref");

    while (test_data.read_row(filename, npa, numpalpha, npb, numpbeta, numbetapair, alpha_ref, beta_ref)){
        DSSP dssp_under_test(base_path_dssp + filename);
        BOOST_CHECK_EQUAL(dssp_under_test.get_alpha_helix_count() , npa);
        BOOST_CHECK_EQUAL(dssp_under_test.get_alpha_residue_count(), numpalpha);
        BOOST_CHECK_EQUAL(dssp_under_test.get_beta_strand_count(), npb);
        BOOST_CHECK_EQUAL(dssp_under_test.get_beta_residue_count(), numpbeta);
        std::vector<Range> alpha_ranges = get_ranges_from_csv(base_path_csv + alpha_ref);
        std::vector<Range> beta_ranges = get_ranges_from_csv(base_path_csv + beta_ref);
        compare_ranges(dssp_under_test.get_alpha_helix_ranges(), alpha_ranges);
        compare_ranges(dssp_under_test.get_beta_strand_ranges(), beta_ranges);
    }
}

BOOST_AUTO_TEST_CASE(dssp_beta_pair){
    TEST_MESSAGE("dssp_beta_pair");

    std::string base_path_dssp = "test/resources/dssp_files/";
    std::string base_path_csv = "test/resources/dssp_csv_beta/";

    io::CSVReader<4> test_data(base_path_csv + "2001_beta.csv");
    test_data.read_header(io::ignore_extra_column, "start", "end", "nbetap1", "nbetap2");

    DSSP dssp_under_test(base_path_dssp + "2001.dssp");
    std::vector<std::pair<int,int>> beta_pairs = dssp_under_test.get_beta_pairs();

    int start, end, nbetap1, nbetap2;
    int j = 0;
    while(test_data.read_row(start, end, nbetap1, nbetap2)){
        BOOST_CHECK_EQUAL(nbetap1, beta_pairs[j].first);
        BOOST_CHECK_EQUAL(nbetap2, beta_pairs[j].second);
        j += 1;
    }
}

BOOST_AUTO_TEST_SUITE_END()

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
