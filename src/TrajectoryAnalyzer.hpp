/**
 * @file   TrajectoryAnalyzer.hpp
 * @author see AUTHORS
 * @brief  TrajectoryAnalyzer header file.
 */

#ifndef TRAJECTORYANALYZER_HPP
#define TRAJECTORYANALYZER_HPP

#include <memory>

#include <plog/Log.h>
#include <Eigen/Dense>

#include "Trajectory.hpp"
#include "ProteinSegment.hpp"
#include "ProteinSegmentEnsemble.hpp"

/*
* @class TrajectoryAnalyzer
*/
class TrajectoryAnalyzer {
public:
    /**
     * @param trajectory
     * @param protein_segments
     * @param temperature
     * @param ensemple_size
     */
    void analyze(Trajectory & trajectory,
                 std::vector<std::shared_ptr<ProteinSegment>> & protein_segments,
                 const double & temperature,
                 const int ensemble_size);

    /**
     * @param nma_covariance
     * @param protein_segments
     * @param temperature
     */
    void analyze(const Eigen::MatrixXd & nma_covariance,
                 std::vector<std::shared_ptr<ProteinSegment>> & protein_segments,
                 const double & temperature);

};

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
