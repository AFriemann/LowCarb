/* NMAAnalyzer.cpp
 * -*- coding: utf-8 -*-
 *
 */

#include <boost/test/unit_test.hpp>

// system includes =============================================================

#include <memory>
#include <vector>

// local includes ==============================================================

#include "../src/utils/CSVReader.hpp"
#include "../src/TrajectoryAnalyzer.hpp"
#include "../src/ProteinFile.hpp"
#include "../src/Protein.hpp"

#include "utils/test.hpp"
#include "utils/log.hpp"

BOOST_AUTO_TEST_SUITE(nma_analyzer_test_suite)

BOOST_AUTO_TEST_CASE(kk_matrix_global){
    TEST_MESSAGE("write");

    auto pdb = std::make_shared<PDB>("test/resources/integration/mb_ca.pdb");
    double temperature = 120.0;

    Protein protein(pdb);
    std::vector<std::shared_ptr<ProteinSegment>> protein_segments{
        std::shared_ptr<ProteinSegment>(new ProteinSegment(protein.get_residues(), 1, 151, COMPLETE_PROTEIN))};

    CSVReader csv_reader;
    Eigen::MatrixXd nma_covariance = csv_reader.read_matrix(
        "test/resources/integration/covariance.csv",
        protein.residue_count() * 3);

    TrajectoryAnalyzer analyzer;
    analyzer.analyze(nma_covariance, protein_segments, temperature);

    Eigen::MatrixXd force_constant = protein_segments[0]->force_constant();
    compare_force_constant_matrix(
                force_constant,
                "test/resources/forceconst_nma/force_constant_complete_protein.csv",
                10e-5,
                false,
                false);
}

BOOST_AUTO_TEST_CASE(kk_matrix_segments){
    TEST_MESSAGE("write");

    auto pdb = std::make_shared<PDB>("test/resources/integration/mb_ca.pdb");
    double temperature = 120.0;

    Protein protein(pdb);

    std::vector<std::shared_ptr<ProteinSegment>> protein_segments{
        std::shared_ptr<ProteinSegment>(new ProteinSegment(protein.get_residues(), 1, 20, COMPLETE_PROTEIN)),
        std::shared_ptr<ProteinSegment>(new ProteinSegment(protein.get_residues(), 18, 37, COMPLETE_PROTEIN)),
        std::shared_ptr<ProteinSegment>(new ProteinSegment(protein.get_residues(), 35, 54, COMPLETE_PROTEIN)),
        std::shared_ptr<ProteinSegment>(new ProteinSegment(protein.get_residues(), 52, 71, COMPLETE_PROTEIN)),
        std::shared_ptr<ProteinSegment>(new ProteinSegment(protein.get_residues(), 69, 88, COMPLETE_PROTEIN)),
        std::shared_ptr<ProteinSegment>(new ProteinSegment(protein.get_residues(), 86, 105, COMPLETE_PROTEIN)),
        std::shared_ptr<ProteinSegment>(new ProteinSegment(protein.get_residues(), 103, 122, COMPLETE_PROTEIN)),
        std::shared_ptr<ProteinSegment>(new ProteinSegment(protein.get_residues(), 120, 139, COMPLETE_PROTEIN)),
        std::shared_ptr<ProteinSegment>(new ProteinSegment(protein.get_residues(), 132, 151, COMPLETE_PROTEIN))
    };

    CSVReader csv_reader;
    Eigen::MatrixXd nma_covariance = csv_reader.read_matrix(
        "test/resources/integration/covariance.csv",
        protein.residue_count() * 3);

    TrajectoryAnalyzer analyzer;
    analyzer.analyze(nma_covariance, protein_segments, temperature);

    for (size_t i=0; i<9; ++i)
    {
        Eigen::MatrixXd force_constants = protein_segments.at(i)->force_constant();
        std::stringstream ss;
        ss << "test/resources/forceconst_nma/force_constant_segment0" << i+1 << ".csv";
        compare_force_constant_matrix(force_constants, ss.str(), 10e-5, false, false);
    }
}

BOOST_AUTO_TEST_SUITE_END()

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
