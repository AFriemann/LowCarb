/* TypeUtilTest.cpp
 * -*- coding: utf-8 -*-
 *
 */

#include <boost/test/unit_test.hpp>

#include "../src/utils/TypeUtils.hpp"

#include "utils/log.hpp"

BOOST_AUTO_TEST_SUITE(typeUtil_test)

    BOOST_AUTO_TEST_CASE(char_to_int_test){
        TEST_MESSAGE("char_to_int");

        char mem[4];
        mem[0] = 0x31;
        mem[1] = 0xa5;
        mem[2] = 0x7b;
        mem[3] = 0xc1;

        BOOST_CHECK_EQUAL(char_to_int(mem,  true), -1048861391);
        BOOST_CHECK_EQUAL(char_to_int(mem,  false), 832928705);
    }

    BOOST_AUTO_TEST_CASE(char_to_double_test){
        TEST_MESSAGE("char_to_double_test");

        char mem[4];
        mem[0] = 0x31;
        mem[1] = 0xa5;
        mem[2] = 0x7b;
        mem[3] = 0xc1;

        BOOST_CHECK_CLOSE_FRACTION(char_to_double(mem,  true), -15.72783, 10e-08);
        BOOST_CHECK_CLOSE_FRACTION(char_to_double(mem,  false), 4.816201e-09, 10e-08);
    }


BOOST_AUTO_TEST_SUITE_END()




// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
