/* ConfigParser.cpp
 * -*- coding: utf-8 -*-
 *
 */

#include <boost/test/unit_test.hpp>

// system includes =============================================================

#include <string>
#include <boost/filesystem.hpp>

// local includes ==============================================================

#include "../src/cli/ConfigParser.hpp"

#include "utils/file.hpp"
#include "utils/log.hpp"

BOOST_AUTO_TEST_SUITE(config_parser)

BOOST_AUTO_TEST_CASE(read_config_files){
    TEST_MESSAGE("read_config_files");

    ConfigParser cp;
    Config config = cp.parse("test/resources/integration/config.ini");

    BOOST_CHECK_EQUAL(config.general.ensemble_size, 2000);
    BOOST_CHECK_EQUAL(config.general.segment_size, 20);
    BOOST_CHECK_EQUAL(config.general.temperature, 120.0);
    BOOST_CHECK(boost::filesystem::exists(config.files.protein));

    BOOST_REQUIRE(config.files.secondary_structure);
    BOOST_CHECK(boost::filesystem::exists(*(config.files.secondary_structure)));

    BOOST_REQUIRE(!config.files.nma_covariance);
    BOOST_REQUIRE_EQUAL(config.files.trajectories.size(), 1);
    BOOST_CHECK(boost::filesystem::exists(config.files.trajectories[0]));
}

BOOST_AUTO_TEST_CASE(read_config_nma){
    TEST_MESSAGE("read_config_nma");

    ConfigParser cp;
    Config config = cp.parse("test/resources/integration/config_nma.ini");

    BOOST_CHECK_EQUAL(config.general.ensemble_size, 2000);
    BOOST_CHECK_EQUAL(config.general.segment_size, 20);
    BOOST_CHECK_EQUAL(config.general.temperature, 120.0);
    BOOST_CHECK(boost::filesystem::exists(config.files.protein));

    BOOST_REQUIRE(config.files.secondary_structure);

    BOOST_REQUIRE_EQUAL(config.files.trajectories.size(), 0);

    BOOST_REQUIRE(config.files.nma_covariance);

    BOOST_CHECK(boost::filesystem::exists(*config.files.nma_covariance));
}

BOOST_AUTO_TEST_CASE(read_failing_config_files){
    TEST_MESSAGE("read_failing_config_files");

    ConfigParser cp;

    for (std::string path : glob("test/resources/integration/failing-config-*.ini")) {
        BOOST_CHECK_THROW(cp.parse(path), std::runtime_error);
    }
}

BOOST_AUTO_TEST_SUITE_END()

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
