/* ReductionFile.cpp
 * -*- coding: utf-8 -*-
 *
 */

#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>
#include <stdexcept>
#include <memory>

#include <Eigen/Dense>

// local includes ==============================================================

#include "../src/FileFactory.hpp"
#include "../src/ReductionFile.hpp"

#include "utils/file.hpp"
#include "utils/log.hpp"

BOOST_AUTO_TEST_SUITE(ReductionFileTestSuite)

    struct Setup {
        Setup() { factory = ReductionFileFactory(); }

        ReductionFileFactory factory;

        std::string test_file_dir = "test/resources/mat_reduce/";
    };

    BOOST_AUTO_TEST_CASE(validate_input) {
        TEST_MESSAGE("validate_input");

        Setup s;

        for (std::string & path : glob(s.test_file_dir + "selection_*_90")) {
            BOOST_REQUIRE_NO_THROW(s.factory.create(path));
        }
    }

    BOOST_AUTO_TEST_CASE(invalidate_invalid_input) {
        TEST_MESSAGE("invalidate_invalid_input");

        Setup s;

        BOOST_REQUIRE_THROW(s.factory.create("test/resources/mat_reduce/failing_selection_str"), std::runtime_error);
    }

    BOOST_AUTO_TEST_CASE(read_selection_input) {
        TEST_MESSAGE("read_selection_input");

        Setup s;

        for (std::string & path : glob(s.test_file_dir + "selection_*_90")) {
            std::shared_ptr<ReductionFile> file = s.factory.create(path, 30);

            std::vector<int> actual = file->get_selection();
            std::vector<int> expected = get_std_vector_from_csv<int>(path + ".csv", "selection");

            BOOST_REQUIRE_EQUAL(actual.size(), expected.size());

            for (size_t i = 0; i < actual.size(); i++) {
                BOOST_REQUIRE_EQUAL(actual.at(i), expected.at(i));
            }
        }
    }

BOOST_AUTO_TEST_SUITE_END()

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
