/**
 * @file   Reach.hpp
 * @author see AUTHORS
 * @brief  Reach header file.
 */

#ifndef REACH_HPP
#define REACH_HPP

#include <vector>
#include <stdexcept>
#include <map>

#include <plog/Log.h>

// local includes
#include "ModelReduction.hpp"
#include "Trajectory.hpp"
#include "TrajectoryAnalyzer.hpp"
#include "Protein.hpp"
#include "KRComputation.hpp"
#include "NormalModeMeanSquareFluctuationCalculator.hpp"

/*
* @class Reach
* @brief Reach is initialized with a trajectory and a protein. Its responsibility is the computing of force constants.
*/
class Reach {
public:
    /**
     * @sideeffect will change protein segments during runtime.
     */
    Reach(const std::vector<std::shared_ptr<ProteinSegment>> & protein_segments,
          const double temperature,
          const int ensemble_size,
          const double average_bin_length,
          const double minimum_length,
          const double maximum_length,
          const bool use_slow_fitting,
          const double slow_minimum_length,
          const double slow_maximum_length,
          const ModelReduction & reduction);

    std::vector<std::shared_ptr<ProteinSegment>> get_protein_segments() const;

    Eigen::VectorXd first_segments_mean_square_fluctuation() const;
    Eigen::VectorXd get_mean_square_fluctuation() const;
    Eigen::VectorXd get_eigenvalues() const;
    Eigen::VectorXd get_weighted_eigenvalues() const;

    Eigen::VectorXd get_weighted_eigenvector() const;
    Eigen::MatrixXd get_eigenvectors() const;

    Eigen::MatrixXd get_covariance_matrix() const;

    Eigen::MatrixXd get_hessian_matrix() const;

    std::map<std::string, double> get_average_force_constant_map() const;
    std::map<std::string, Eigen::VectorXd> get_kr_complete_protein_map() const;

    /**
     * @brief this is the main method for calculating the force constants of a protein as done in the REACH method.
     * The force constants are calculated from the trajectory and stored in the protein segment.
     * @param protein
     */
    void compute_mean_square_fluctuation(const Protein & protein);

 private:
    std::vector<std::shared_ptr<ProteinSegment>> protein_segments;
    double temperature;

    // fitting options
    double average_bin_length;
    double minimum_length;
    double maximum_length;
    bool use_slow_fitting;
    double slow_minimum_length;
    double slow_maximum_length;
    ModelReduction model_reduce_selection;

    Eigen::VectorXd weighted_eigenvalues;
    Eigen::VectorXd weighted_eigenvector;
    Eigen::VectorXd mean_square_fluctuation;

    Eigen::VectorXd eigenvalues;
    Eigen::MatrixXd eigenvectors;

    Eigen::MatrixXd hessian_matrix;

    std::map<std::string, double> average_force_constant_map;
    std::map<std::string, Eigen::VectorXd> kr_complete_protein_map;

};

#endif

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
