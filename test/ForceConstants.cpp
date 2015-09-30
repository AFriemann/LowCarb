/* ForceConstants.cpp
 * -*- coding: utf-8 -*-
 *
 */

#include <boost/test/unit_test.hpp>

// system includes =============================================================

#include <vector>
#include <memory>
#include <Eigen/Dense>

// local includes ==============================================================

#include "../src/TrajectoryAnalyzer.hpp"
#include "../src/TrajectoryFile.hpp"
#include "../src/ProteinFile.hpp"
#include "../src/Protein.hpp"
#include "../src/Trajectory.hpp"
#include "../src/ProteinSegment.hpp"
#include "../src/DCD.hpp"

#include "utils/test.hpp"
#include "utils/log.hpp"

BOOST_AUTO_TEST_SUITE(compare_force_constants)

BOOST_AUTO_TEST_CASE(kk_matrix_global){
    TEST_MESSAGE("kk_matrix_global");

    std::vector<std::shared_ptr<TrajectoryFile>> dcd_files = {std::make_shared<DCD>("test/resources/mb_ca.dcd")};
    auto pdb = std::make_shared<PDB>("test/resources/integration/mb_ca.pdb");
    double temperature   = 120.0;
    int    ensemble_size = 2000;

    Trajectory trajectory(dcd_files);
    Protein protein(pdb);
    std::vector<std::shared_ptr<ProteinSegment>> protein_segments{
        std::shared_ptr<ProteinSegment>(new ProteinSegment(protein.get_residues(), 1, 151, COMPLETE_PROTEIN))};

    TrajectoryAnalyzer analyzer;
    analyzer.analyze(trajectory, protein_segments, temperature, ensemble_size);

    Eigen::MatrixXd force_constant = protein_segments[0]->force_constant();

    // average absolute error: 1.14614e-06
    // most bad values for (i,i+1)
    compare_force_constant_matrix(
                force_constant,
                "test/resources/forceconst/mb_ca_forceconst_kkhex.csv",
                10e-5,
                true,
                true);

    compare_force_constant_matrix(
                force_constant,
                "test/resources/forceconst/mb_ca_forceconst_kk.csv",
                10e-5,
                false,
                true);
}

BOOST_AUTO_TEST_CASE(kk_matrix_segments){
    TEST_MESSAGE("kk_matrix_segments");

    std::vector<std::shared_ptr<TrajectoryFile>> dcd_files = {std::make_shared<DCD>("test/resources/mb_ca.dcd")};
    auto pdb = std::make_shared<PDB>("test/resources/integration/mb_ca.pdb");
    double temperature = 120.0;
    int    ensemble_size = 2000;

    Trajectory trajectory(dcd_files);
    Protein protein(pdb);

    std::vector<std::pair<int,int>> segments = {{1,20},{18,37},{35,54},{52,71},{69,88},{86,105},{103,122},{120,139},{132,151}};
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

    TrajectoryAnalyzer analyzer;
    analyzer.analyze(trajectory, protein_segments, temperature, ensemble_size);

    for (size_t i=0; i<9; ++i)
    {

        Eigen::MatrixXd force_constants = protein_segments.at(i)->force_constant();
        std::stringstream ss;
        ss << "test/resources/forceconst/mb_ca_forceconst_segment0" << i+1 << "_kkhex.csv";
        compare_force_constant_matrix(force_constants, ss.str(), 10e-7, true, true);
    }
}

BOOST_AUTO_TEST_CASE(kk_matrix_helices){
    TEST_MESSAGE("kk_matrix_helices");

    std::vector<std::shared_ptr<TrajectoryFile>> dcd_files = {std::make_shared<DCD>("test/resources/mb_ca.dcd")};
    auto pdb = std::make_shared<PDB>("test/resources/integration/mb_ca.pdb");
    double temperature   = 120.0;
    int    ensemble_size = 2000;

    Trajectory trajectory(dcd_files);
    Protein protein(pdb);

    std::vector<std::pair<int,int>> helices = {{4,17},{21,35},{37,40},{52,56},{59,77},{83,95},{101,118},{125,149}};
    std::vector<std::shared_ptr<ProteinSegment>> protein_segments{
        std::shared_ptr<ProteinSegment>(new ProteinSegment(protein.get_residues(), 4,17, COMPLETE_PROTEIN)),
        std::shared_ptr<ProteinSegment>(new ProteinSegment(protein.get_residues(), 21,35, COMPLETE_PROTEIN)),
        std::shared_ptr<ProteinSegment>(new ProteinSegment(protein.get_residues(), 37,40, COMPLETE_PROTEIN)),
        std::shared_ptr<ProteinSegment>(new ProteinSegment(protein.get_residues(), 52,56, COMPLETE_PROTEIN)),
        std::shared_ptr<ProteinSegment>(new ProteinSegment(protein.get_residues(), 59,77, COMPLETE_PROTEIN)),
        std::shared_ptr<ProteinSegment>(new ProteinSegment(protein.get_residues(), 83,95, COMPLETE_PROTEIN)),
        std::shared_ptr<ProteinSegment>(new ProteinSegment(protein.get_residues(), 101,118, COMPLETE_PROTEIN)),
        std::shared_ptr<ProteinSegment>(new ProteinSegment(protein.get_residues(), 125,149, COMPLETE_PROTEIN)),
    };

    TrajectoryAnalyzer analyzer;
    analyzer.analyze(trajectory, protein_segments, temperature, ensemble_size);

    for (size_t i=0; i<8; ++i)
    {

        Eigen::MatrixXd force_constants = protein_segments.at(i)->force_constant();
        std::stringstream ss;
        ss << "test/resources/forceconst/mb_ca_forceconst_helix0" << i+1 << "_kkhex.csv";
        compare_force_constant_matrix(force_constants, ss.str(), 10e-7, true, true);
    };
}

BOOST_AUTO_TEST_SUITE_END()

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4 fenc=utf-8
