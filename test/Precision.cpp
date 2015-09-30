/* PrecisionTest.cpp
 * -*- coding: utf-8 -*-
 *
 */

#include <boost/test/unit_test.hpp>

// system includes =============================================================

#include <iostream>
#include <memory>
#include <Eigen/Dense>

// local includes ==============================================================

#include "../src/Reach.hpp"
#include "../src/FileFactory.hpp"
#include "../src/TrajectoryFile.hpp"
#include "../src/ProteinFile.hpp"
#include "../src/ModelReduction.hpp"
#include "../src/ProteinSegmentFactory.hpp"
#include "../src/ProteinSegment.hpp"
#include "../src/SecondaryStructureFile.hpp"
#include "../src/cli/ConfigParser.hpp"

#include "utils/file.hpp"
#include "utils/test.hpp"
#include "utils/definitions.hpp"
#include "utils/log.hpp"

BOOST_AUTO_TEST_SUITE(precision)

BOOST_AUTO_TEST_CASE(complete_precision_test) {
    TEST_MESSAGE("complete_precision_test");

    ConfigParser cp;
    Config config = cp.parse("test/resources/integration/config.ini");

    ProteinFileFactory pf;
    std::shared_ptr<ProteinFile> protein_file(pf.create(config.files.protein));

    TrajectoryFileFactory tf;
    std::vector<std::shared_ptr<TrajectoryFile>> trajectory_files;


    for (auto path : config.files.trajectories) {
        trajectory_files.push_back(tf.create(path));
    }

    SecondaryStructureFileFactory ssf;
    std::shared_ptr<SecondaryStructureFile> secondary_structure_file(ssf.create(*config.files.secondary_structure));

    Protein protein(protein_file, secondary_structure_file);

    ProteinSegmentFactory protein_segment_factory;
    std::vector<std::shared_ptr<ProteinSegment>> protein_segments = protein_segment_factory.generate_protein_segments_for_analysis(protein);

    Trajectory trajectory(trajectory_files);

    TrajectoryAnalyzer trajectory_analyzer;
    trajectory_analyzer.analyze(trajectory,
                                protein_segments,
                                config.general.temperature,
                                config.general.ensemble_size);

    Reach reach(protein_segments,
                config.general.temperature,
                config.general.ensemble_size,
                config.fitting.average_bin_length,
                config.fitting.minimum_length,
                config.fitting.maximum_length,
                config.fitting.use_slow_fitting,
                config.fitting.slow_minimum_length,
                config.fitting.slow_maximum_length,
                ModelReduction());

    reach.compute_mean_square_fluctuation(protein);

    Eigen::VectorXd mean_square_fluctuation = reach.get_mean_square_fluctuation();

    std::string test_dir = "test/resources/calnmodemsfss_fixed/";
    std::string test_prefix = "mb_ca";
    Eigen::VectorXd mean_square_fluctuation_reach = get_vector_from_csv(test_dir + test_prefix + "_calnmodemsfss_xxcom.csv","xxcom");

    BOOST_REQUIRE_EQUAL(mean_square_fluctuation.rows(), mean_square_fluctuation_reach.rows());

    for (size_t i=0; i < mean_square_fluctuation.rows(); i++) {
        BOOST_REQUIRE_CLOSE_FRACTION(mean_square_fluctuation(i), mean_square_fluctuation_reach(i), PRECISION);
    }
}

BOOST_AUTO_TEST_SUITE_END()


// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
