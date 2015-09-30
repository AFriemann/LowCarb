/* NonlinearFitter.cpp
 * -*- coding: utf-8 -*-
 *
 */

#include <boost/test/unit_test.hpp>

// system includes =============================================================

#include <string>
#include <csv.h>
#include <Eigen/Dense>

// local includes ==============================================================

#include "../src/NonlinearFitter.hpp"

#include "utils/file.hpp"
#include "utils/log.hpp"

BOOST_AUTO_TEST_SUITE(nonlinear_fitting)

BOOST_AUTO_TEST_CASE(exp_fit){
    TEST_MESSAGE("exp_fit");

    std::string base_path = "test/resources/nonlinearfit/";
    io::CSVReader<9> test_data(base_path + "mb_ca_nonlinearexpfit.csv");
    test_data.read_header(io::ignore_extra_column,
        "in_maxn", "in_nlen", "in_x", "in_y", "in_yerr2",
        "out_a", "out_b", "out_erra", "out_errb"
    );
    std::string in_x, in_y, in_yerr2, out_a, out_b, out_erra, out_errb;
    int in_maxn, in_nlen;
    while (test_data.read_row(in_maxn, in_nlen, in_x, in_y, in_yerr2, out_a, out_b, out_erra, out_errb)){
        Eigen::VectorXd x = get_vector_from_csv(base_path + in_x, "x");
        Eigen::VectorXd y = get_vector_from_csv(base_path + in_y, "y");
        Eigen::VectorXd yerr2 = get_vector_from_csv(base_path + in_yerr2, "yerr2");

        double expected_a = cast_hex_to_double(out_a);
        double expected_b = cast_hex_to_double(out_b);
        double expected_erra = cast_hex_to_double(out_erra);
        double expected_errb = cast_hex_to_double(out_errb);

        NonlinearFitter fitter;
        fitter.exponential_fit(x, y, yerr2);

        BOOST_CHECK_EQUAL(fitter.get_a(), expected_a);
        BOOST_CHECK_EQUAL(fitter.get_b(), expected_b);
        BOOST_CHECK_EQUAL(fitter.get_err_a(), expected_erra);
        BOOST_CHECK_EQUAL(fitter.get_err_b(), expected_errb);

    }
    //fitter.exponential_fit(Eigen::VectorXd &x, Eigen::VectorXd y)
}

BOOST_AUTO_TEST_SUITE_END()

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
