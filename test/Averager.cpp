/* Averager.cpp
 * -*- coding: utf-8 -*-
 *
 */

#include <boost/test/unit_test.hpp>

// system includes =============================================================

#include <Eigen/Dense>

// local includes ==============================================================

#include "../src/Averager.hpp"

#include "utils/log.hpp"

BOOST_AUTO_TEST_SUITE(averager_int_test_suite)

BOOST_AUTO_TEST_CASE(one_int_gets_returned) {
    TEST_MESSAGE("one_int_gets_returned");

    Averager<int> uut;
    uut.add(42);
    BOOST_CHECK_EQUAL(uut.get(), 42);
}

BOOST_AUTO_TEST_CASE(multiple_ints_get_averaged) {
    TEST_MESSAGE("multiple_ints_get_averaged");

    Averager<int> uut;
    uut.add(42);
    uut.add(23);
    uut.add(1);
    BOOST_CHECK_EQUAL(uut.get(), 22);
}

BOOST_AUTO_TEST_CASE(multiple_ints_are_floored) {
    TEST_MESSAGE("multiple_ints_are_floored");

    Averager<int> uut;
    uut.add(42);
    uut.add(23);
    BOOST_CHECK_EQUAL(uut.get(), 32);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(averager_double_test_suite)

BOOST_AUTO_TEST_CASE(one_double_gets_returned) {
    TEST_MESSAGE("one_double_gets_returned");

    Averager<double> uut;
    uut.add(42);
    BOOST_CHECK_EQUAL(uut.get(), 42.0);
}

BOOST_AUTO_TEST_CASE(multiple_doubles_get_averaged) {
    TEST_MESSAGE("multiple_doubles_get_averaged");

    Averager<double> uut;
    uut.add(42);
    uut.add(23);
    BOOST_CHECK_EQUAL(uut.get(), 32.5);
}

BOOST_AUTO_TEST_CASE(get_copies_sum_correctly) {
    TEST_MESSAGE("get_copies_sum_correctly");

    Averager<double> uut;
    uut.add(42);
    BOOST_CHECK_EQUAL(uut.get(), 42);
    uut.add(23);
    BOOST_CHECK_EQUAL(uut.get(), 32.5);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(averager_matrix_test_suite)

BOOST_AUTO_TEST_CASE(one_matrix_gets_returned) {
    TEST_MESSAGE("one_matrix_gets_returned");

    Averager<Eigen::Matrix3i> uut;

    Eigen::Matrix3i diagonal(3, 3);
    for (unsigned i = 0; i < diagonal.rows(); ++i) {
        for (unsigned j = 0; j < diagonal.cols(); ++j) {
            if (i == j) {
                diagonal(i, j) = 1;
            } else {
                diagonal(i, j) = 0;
            }
        }
    }

    uut.add(diagonal);

    Eigen::Matrix3i result = uut.get();
    for (unsigned i = 0; i < diagonal.rows(); ++i) {
        for (unsigned j = 0; j < diagonal.cols(); ++j) {
            BOOST_CHECK_EQUAL(result(i, j), diagonal(i, j));
        }
    }
}

BOOST_AUTO_TEST_CASE(two_matrix_get_averaged) {
    TEST_MESSAGE("two_matrix_get_averaged");

    Averager<Eigen::Matrix3i> uut;

    Eigen::Matrix3i m1;
    m1(0, 0) = 1; m1(0, 1) = 0; m1(0, 2) = 0;
    m1(1, 0) = 0; m1(1, 1) = 1; m1(1, 2) = 0;
    m1(2, 0) = 0; m1(2, 1) = 0; m1(2, 2) = 1;

    Eigen::Matrix3i m2;
    m2(0, 0) = 1; m2(0, 1) = 1; m2(0, 2) = 2;
    m2(1, 0) = 0; m2(1, 1) = 3; m2(1, 2) = 0;
    m2(2, 0) = 2; m2(2, 1) = 1; m2(2, 2) = 1;

    Eigen::Matrix3i expected;
    expected(0, 0) = 1; expected(0, 1) = 0; expected(0, 2) = 1;
    expected(1, 0) = 0; expected(1, 1) = 2; expected(1, 2) = 0;
    expected(2, 0) = 1; expected(2, 1) = 0; expected(2, 2) = 1;

    uut.add(m1);
    uut.add(m2);

    Eigen::Matrix3i result = uut.get();

    for (unsigned i = 0; i < result.rows(); ++i) {
        for (unsigned j = 0; j < result.cols(); ++j) {
            BOOST_CHECK_EQUAL(result(i, j), expected(i, j));
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
