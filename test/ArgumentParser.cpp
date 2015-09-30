/* ArgumentParser.cpp
 * -*- coding: utf-8 -*-
 *
 */

#include <boost/test/unit_test.hpp>

// system includes =============================================================

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

// local includes ==============================================================

#include "../src/cli/ArgumentParser.hpp"

#include "utils/log.hpp"

BOOST_AUTO_TEST_SUITE(argument_parser)

    BOOST_AUTO_TEST_CASE(parse_arguments) {
        TEST_MESSAGE("parse_arguments");

        ArgumentParser ap;

        const char * temp_dir = "test/results/Ko7Woh8igi3koof";
        const char * config_path = "test/resources/integration/config.ini";

        BOOST_REQUIRE(boost::filesystem::exists(config_path));
        BOOST_REQUIRE(!boost::filesystem::exists(temp_dir));

        const char * argv[5] = {"foobar", "-c", config_path, "-o", temp_dir};
        const int    argc    = (sizeof(argv)/sizeof(*argv));

        Arguments args = ap.parse(argc, argv);

        BOOST_CHECK_EQUAL(args.paths.config, config_path);
        BOOST_CHECK_EQUAL(args.paths.output, temp_dir);
        BOOST_CHECK(boost::filesystem::exists(temp_dir));

        boost::filesystem::remove_all(temp_dir);
    }

    BOOST_AUTO_TEST_CASE(parse_failing_arguments) {
        TEST_MESSAGE("parse_failing_arguments");

        ArgumentParser ap;

        const char * existing_config = "test/resources/integration/config.ini";
        const char * missing_file = "test/resources/integration/noconfig.ini";
        const char * existing_file = "test/resources/stub";
        const char * temp_dir = "test/results/Khe2kuhei1uheo21";

        // check files prior to testing
        BOOST_REQUIRE(!boost::filesystem::exists(missing_file));
        BOOST_REQUIRE(boost::filesystem::is_regular_file(existing_config));
        BOOST_REQUIRE(boost::filesystem::is_regular_file(existing_file));
        BOOST_REQUIRE(!boost::filesystem::exists(temp_dir));

        const char * args[5] = {"foobar", "-c", existing_config, "-o", existing_file};

        BOOST_REQUIRE_THROW(ap.parse(5, args), boost::program_options::error);

        args[2] = missing_file;
        args[4] = temp_dir;

        BOOST_REQUIRE_THROW(ap.parse(5, args), boost::program_options::error);

        // everything should be as before
        BOOST_REQUIRE(!boost::filesystem::exists(missing_file));
        BOOST_REQUIRE(boost::filesystem::is_regular_file(existing_config));
        BOOST_REQUIRE(boost::filesystem::is_regular_file(existing_file));
        BOOST_REQUIRE(!boost::filesystem::exists(temp_dir));
    }

BOOST_AUTO_TEST_SUITE_END()

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
