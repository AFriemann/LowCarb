/**
 * @file   Reach.cpp
 * @author see AUTHORS
 * @brief  Reach definitions file.
 */

#include "Reach.hpp"

Reach::Reach(const std::vector<std::shared_ptr<ProteinSegment>> & protein_segments,
             const double temperature,
             const int ensemble_size,
             const double average_bin_length,
             const double minimum_length,
             const double maximum_length,
             const bool use_slow_fitting,
             const double slow_minimum_length,
             const double slow_maximum_length,
             const ModelReduction & reduction)
    :  protein_segments(protein_segments),
       average_bin_length(average_bin_length), minimum_length(minimum_length), maximum_length(maximum_length),
       use_slow_fitting(use_slow_fitting),
       slow_minimum_length(slow_minimum_length), slow_maximum_length(slow_maximum_length), model_reduce_selection(reduction),
       temperature(temperature) {}

void Reach::compute_mean_square_fluctuation(const Protein & protein) {
    LOGI << "computing mean square fluctuation";

    KRComputation kr_computation(
        protein,
        this->protein_segments,
        this->minimum_length,
        this->maximum_length,
        this->use_slow_fitting,
        this->slow_minimum_length,
        this->slow_maximum_length,
        this->average_bin_length);

    ForceConstantSelector kk_selector = kr_computation.kk_selector();
    this->average_force_constant_map = kr_computation.get_average_force_constant_map();
    this->kr_complete_protein_map = kr_computation.get_kr_complete_protein_map();

    NormalModeMeanSquareFluctuationCalculator nmodemsf(protein.get_residues());

    nmodemsf.calculate(protein,
                       this->temperature,
                       kk_selector,
                       std::shared_ptr<ProteinSegment>(this->protein_segments[0]),
                       model_reduce_selection);

    this->eigenvalues = nmodemsf.get_eigenvalues();
    this->eigenvectors = nmodemsf.get_eigenvectors();

    this->weighted_eigenvalues = nmodemsf.get_weighted_eigenvalues();
    this->weighted_eigenvector = nmodemsf.get_weighted_eigenvector();
    this->mean_square_fluctuation = nmodemsf.get_mean_square_fluctuation();
    this->hessian_matrix = nmodemsf.get_hessian_matrix();
}


//LCOV_EXCL_START
Eigen::MatrixXd Reach::get_hessian_matrix() const {
    return this->hessian_matrix;
}

std::vector<std::shared_ptr<ProteinSegment>> Reach::get_protein_segments() const {
    return this->protein_segments;
}

Eigen::VectorXd Reach::get_mean_square_fluctuation() const {
    return this->mean_square_fluctuation;
}


Eigen::VectorXd Reach::first_segments_mean_square_fluctuation() const {
    return this->protein_segments[0]->mean_square_fluctuation();
}

Eigen::VectorXd Reach::get_weighted_eigenvalues() const {
    return this->weighted_eigenvalues;
}

Eigen::VectorXd Reach::get_weighted_eigenvector() const {
    return this->weighted_eigenvector;
}

Eigen::VectorXd Reach::get_eigenvalues() const {
    return this->eigenvalues;
}

Eigen::MatrixXd Reach::get_eigenvectors() const { return this->eigenvectors; }

std::map<std::string, double> Reach::get_average_force_constant_map() const {
    return this->average_force_constant_map;
}

std::map<std::string, Eigen::VectorXd> Reach::get_kr_complete_protein_map() const {
    return this->kr_complete_protein_map;
}

Eigen::MatrixXd Reach::get_covariance_matrix() const {
    return this->protein_segments[0]->get_average_covariance_matrix();
}
//COV_EXCL_STOP

// vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
