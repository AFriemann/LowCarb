
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <memory>

#include "../src/FileFactory.hpp"

#include "utils/log.hpp"

BOOST_AUTO_TEST_SUITE(file_factory_test_suite);

    BOOST_AUTO_TEST_CASE(create_xtc_test) {
        TEST_MESSAGE("create_xtc_test");

        TrajectoryFileFactory file_factory;
        boost::filesystem::path xtc_path("test/resources/mb_ca.xtc");
        std::shared_ptr<TrajectoryFile> xtc = file_factory.create(xtc_path);
        BOOST_CHECK_EQUAL(XTC_TYPE, xtc->get_type());
    }


    BOOST_AUTO_TEST_CASE(create_trr_test) {
        TEST_MESSAGE("create_trr_test");

        TrajectoryFileFactory file_factory;
        boost::filesystem::path path("test/resources/pnb_subset.trr");
        std::shared_ptr<TrajectoryFile> trr = file_factory.create(path);
        BOOST_CHECK_EQUAL(TRR_TYPE, trr->get_type());
    }

    BOOST_AUTO_TEST_CASE(create_dcd_test) {
        TEST_MESSAGE("create_dcd_test");

        TrajectoryFileFactory file_factory;
        boost::filesystem::path xtc_path("test/resources/mb_ca.dcd");
        std::shared_ptr<TrajectoryFile> xtc = file_factory.create(xtc_path);
        BOOST_CHECK_EQUAL(DCD_TYPE, xtc->get_type());
    }

    BOOST_AUTO_TEST_CASE(create_trajectory_unsupported_file_format_test) {
        TEST_MESSAGE("create_trajectory_unsupported_file_format_test");

        TrajectoryFileFactory file_factory;
        boost::filesystem::path xtc_path("test/resources/mb_ca.foobar");
        BOOST_REQUIRE_THROW(file_factory.create(xtc_path), std::runtime_error);
    }

    BOOST_AUTO_TEST_CASE(create_protein_test) {
        TEST_MESSAGE("create_protein_test");

        ProteinFileFactory file_factory;
        boost::filesystem::path protein_path("test/resources/integration/trj_1.pdb");
        BOOST_REQUIRE_NO_THROW(file_factory.create(protein_path));
    }

    BOOST_AUTO_TEST_CASE(create_protein_unsupported_file_format_test) {
        TEST_MESSAGE("create_protein_unsupported_file_format_test");

        ProteinFileFactory file_factory;
        boost::filesystem::path protein_path("test/resources/integration/mb_ca.foobar");
        BOOST_REQUIRE_THROW(file_factory.create(protein_path), std::runtime_error);
    }

    BOOST_AUTO_TEST_CASE(create_secondary_structure_test) {
        TEST_MESSAGE("create_secondary_structure_test");

        SecondaryStructureFileFactory file_factory;
        boost::filesystem::path secondary_structure_path("test/resources/integration/dssp.dssp");
        BOOST_REQUIRE_NO_THROW(file_factory.create(secondary_structure_path));
    }

    BOOST_AUTO_TEST_CASE(create_secondary_structure_unsupported_file_format_test) {
        TEST_MESSAGE("create_secondary_structure_unsupported_file_format_test");

        SecondaryStructureFileFactory file_factory;
        boost::filesystem::path secondary_structure_path("test/resources/integration/dssp.foobar");
        BOOST_REQUIRE_THROW(file_factory.create(secondary_structure_path), std::runtime_error);
    }

BOOST_AUTO_TEST_SUITE_END();

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
